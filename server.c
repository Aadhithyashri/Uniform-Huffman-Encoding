#include <stdio.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 
#include <math.h>

void decode(int mapping[256],char encoded_final[],char message[])
{
    int max=-1,num_bits=0,tmp;
    for(int i=0;i<256;i++)
        if(mapping[i]>max)
            max=mapping[i];

    char lookup[max+1];
    tmp=max;
    while(tmp>0)
    {
        tmp/=2;
        num_bits++;
    }

    for(int i=0;i<256;i++)
        if(mapping[i]>-1)
            lookup[mapping[i]]=(char)i;

        for(int i=0;i<strlen(encoded_final)/num_bits;i++)
    {
        int num=0;
        for(int j=num_bits*i;j<num_bits*(i+1);j++)
        {
            num*=2;
            num+= encoded_final[j]=='0'? 0 : 1;
        }
        message[i] = lookup[num];
    }
}

int main()
{
    int sd,conn_sd;
    char buffer[1024] = "",encoded_final[8*1024];  
    FILE *f1;

    sd = socket(AF_INET,SOCK_STREAM,0);
    if(sd==-1)
        return(printf("Socket creation unsucessfull\n"));

    struct sockaddr_in Server,Client;
    memset(&Server,0,sizeof(Server));

    Server.sin_family = AF_INET;
    Server.sin_port = htons(8080);
    Server.sin_addr.s_addr = inet_addr("0");

    if(bind(sd,(const struct sockaddr*)&Server,sizeof(Server)))
        return(printf("Could not bind!\n"));

    if(listen(sd,5))
        return(printf("Too many connections!"));

    int cli_len = sizeof(Client);
    conn_sd = accept(sd,(struct sockaddr*)&Client,&cli_len);

    if(conn_sd < 1)
        return(printf("Error, can't accept connection!"));

    int n;
    int mapping[256];

    read(conn_sd,&encoded_final,sizeof(encoded_final));
    read(conn_sd,&mapping,sizeof(mapping));

    decode(mapping,encoded_final,buffer);

    printf("ENCODED MESSAGE: %s\nORIGINAL MESSAGE: %s\n",encoded_final,buffer);

    close(sd);
    close(conn_sd);
}
