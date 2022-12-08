//header files
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<pwd.h>

int main(void)
{
    int clientSocket; //Socket Descriptor for Client
    struct sockaddr_in server_addr;
    struct hostent *ptrh;//to sore the address details of host
    //communication message
    char sending_message[256],recieved_message[256];
    char *name;

    int n = 0;

    clientSocket=socket(AF_INET, SOCK_STREAM, 0);

    memset((char*)&server_addr, 0, sizeof(server_addr));//setting server_addr to zero

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9001);


    ptrh=gethostbyname("192.168.0.102");//private ip address
    memcpy(&server_addr.sin_addr,ptrh->h_addr,ptrh->h_length);//copying the host address it to server_addr

    //connecting to the host address
    if(connect(clientSocket,(struct sockaddr *)&server_addr,sizeof(server_addr))==-1){
        printf("There was an error making a connection\n\n");
        exit(1);
    }
    char server_response[256];

    //first response that server is connected 
    recv(clientSocket,&server_response,sizeof(server_response),0);
    

    printf("%s: ",server_response);

    //chat room program
   bzero(sending_message,256);
   bzero(recieved_message,256);

   //initially sending the client name
   //get the client username
    register struct passwd *pw;
    register uid_t uid;
    uid = geteuid ();
    pw = getpwuid (uid);
    name=pw->pw_name;
    send(clientSocket,name,sizeof(name),0);

    while(1){
        printf("\n[+]%s:",name);
        fgets(sending_message,sizeof(sending_message),stdin);
        //sending message to server
        send(clientSocket,&sending_message,sizeof(sending_message),0);
        // printf("%s",sending_message);
        if(strcmp(sending_message,"exit\n")==0){
            break;
        }
        fputs("SERVER typing...\n",stdout);
        //receieving message from server
        recv(clientSocket,&recieved_message,sizeof(recieved_message),0);
        fputs("[\033[A\033[2K",stdout);
        printf("\n[+]SERVER: %s",recieved_message);
    }
    close(clientSocket);
    printf("<--SERVER DISCONNECTED-->");
    return 0;
}
