//header files

#include<sys/socket.h>//used to include accept(),bind(),socket()
#include<netinet/in.h>//internet address family
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>//for the close function 
#include<string.h>//for handling strings
//#include<netdb.h> used in client side


int main(void)
{
//various communication messages
char server_message[256]="SERVER CONNECTED";
char sending_message[256],recieved_message[256],client_name[256];

//creating server side socket 
int server_socket,len;
server_socket=socket(AF_INET,SOCK_STREAM,0);
if(server_socket<0)
  printf("server socket creation failed\n");
else{
  printf("server socket created successfully\n");
}

//creating the server side address

struct sockaddr_in server_address,client_address;
server_address.sin_family=AF_INET;//communication domain
server_address.sin_port=htons(9001);//port where server is active
server_address.sin_addr.s_addr=htonl(INADDR_ANY);//connecting to any client IP address


len=sizeof(client_address);

if(bind(server_socket,(struct sockaddr*)&server_address,sizeof(server_address))!=0){
    printf("socket binding failed\n");
}
else{
  printf("socket Binding successfull\n");
}

if(listen(server_socket,5)!=0){//creates a listening queue with max_clients equal to 5
  printf("Listening failed\n");
}
else{
  printf("server Listening\n");
}



    int client_socket;


    client_socket=accept(server_socket,(struct sockaddr*)&client_address,&len);
    if(client_socket<0){
      printf("Server accept failed\n");
    }
    else{
      printf("server accepted the client\n");
    }
    //send the first server message that server is connected 
    send(client_socket,&server_message,sizeof(server_message),0);

    //chat room program
    bzero(sending_message,256);
    bzero(recieved_message,256);
   
   FILE *fptr;
   fptr=fopen("ChatHistory.txt","a");
    
    fprintf(fptr,"\t\t\t\t%s","CHAT HISTORY\n\n");

    //initially recieving the client name
    recv(client_socket,&client_name,sizeof(client_name),0);



while(1){
  fputs("CLIENT typing...\n",stdout);
  recv(client_socket,&recieved_message,sizeof(recieved_message),0);
  fputs("[\033[A\033[2K",stdout);//this clears the current console line
  printf("\n[+]%s:%s",client_name,recieved_message);

  //writing client message to the file
  fprintf(fptr,"%s:",client_name);
  fprintf(fptr,"%s\n",recieved_message);


  if(strcmp(recieved_message,"exit\n")==0){
    printf("<--%s DISCONNECTED-->",client_name);
    break;
  }

  printf("\n[+]SERVER:");
  fgets(sending_message,sizeof(sending_message),stdin);
  send(client_socket,&sending_message,sizeof(sending_message),0);


  //writing server message to file
  fprintf(fptr,"%s","SERVER:");
  fprintf(fptr,"%s\n",sending_message);

}
fprintf(fptr,"\n\n");
fclose(fptr);
close(client_socket);
close(server_socket);
return 0;
}
