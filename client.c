#include <stdio.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <math.h>

void encode(int mapping[256],char message[1024],char *encoded_final)
{
    int num_char=0;
    for(int i=0;i<256;i++)
        mapping[i]=-1;

    for(int i=0;i<strlen(message);i++)
    {
        if(mapping[(int)message[i]]==-1)
        {
            mapping[(int)message[i]] = num_char;
            num_char++;
        }
    }

    int num_bits=0;
    while (pow(2,num_bits)<num_char)
        num_bits++;

    char bits[num_bits*(strlen(message)) + 1];
    for(int i=0;i<strlen(message);i++)
    {
        int conv=mapping[(int)message[i]];
        for(int j=0;j<num_bits;j++)
        {
            bits[num_bits*i+(num_bits-1-j)] = conv%2 ? '1' : '0';
            conv/=2;
        }
    }
    bits[num_bits*(strlen(message))] = '\0';
    strcpy(encoded_final,bits);

}

int main()
{
    int sd,n;
    char store[1024] ="";
    FILE *f1 = fopen("in.txt","r");

    sd = socket(AF_INET,SOCK_STREAM,0);
    if(sd==-1)
        return(printf("Error socket not opened!\n"));

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("0");

    if(connect(sd,(const struct sockaddr*)&server,sizeof(server)))
        return(printf("Unable to connect to server!\n"));


    char message[1024];
    fgets(message,1024,f1);
    printf("Message to encode: %s\n",message);
    message[strlen(message)] = '\0';
    int mapping[256]={0};
    char encoded_final[8*strlen(message)];
    encode(mapping,message,encoded_final);

    write(sd,&encoded_final,sizeof(encoded_final));
    usleep(100);
    write(sd,&mapping,sizeof(mapping));
    close(sd);    
}