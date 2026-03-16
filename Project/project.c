#include <sys/time.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#define max(A, B) ((A) >= (B) ? (A) : (B))
typedef struct _Files{
    /* Files que variam para um dado país */
    char *file;
    struct _Files *nextF;
} Files;
typedef struct _Intr{
    /* dados que variam para um dado país */
    int interior;
    struct _Intr *nextI;
} Intr;
typedef struct _Tabexp{
    /* dados que variam para um dado país */
    int destino;
    int vizinho;
    struct _Tabexp *nextT;
} Tabexp;
typedef struct _No{
    /* dados fixos para um dado país */
    int id;
    int ext;
    int bck;
    Intr *nextI;
    struct _No *nextNO;
} No;
int main(void)
{

    int fd, newfd, afd = 0, fdudp, connectfd;
    int na = 0;
    int x = 0;
    int clientes[30];
    fd_set rfds, ready;
    enum
    {
        idle,
        busy
    } state;
    int maxfd, numerosockets;
    struct addrinfo hints, *res;
    int errcode;
    ssize_t n, nw;
    ssize_t nbytes,nleft,nwritten,nread;
    struct sockaddr addr;
    socklen_t addrlen;
    char *ptr, buffer[128];
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        exit(1); // error
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // UDP socket
    hints.ai_flags = AI_PASSIVE;

    if ((errcode = getaddrinfo(NULL, "30120", &hints, &res)) != 0) /*error*/
        exit(1);

    if (bind(fd, res->ai_addr, res->ai_addrlen) == -1) /*error*/
        exit(1);
    if (listen(fd, 10) == -1) /*error*/
        exit(1);
    FD_ZERO(&rfds);
    FD_ZERO(&ready);
    FD_SET(fd, &rfds);
    FD_SET(0, &rfds);
    maxfd = fd;
    printf("maxfd = %d\n", maxfd);
    fprintf(stdout, "maxfd = %d\n", maxfd);
    
    while (1)
    {

        ready = rfds;
        numerosockets = select(maxfd+1, &ready, (fd_set *)NULL, (fd_set *)NULL, (struct timeval *)NULL);

        if (numerosockets <= 0) /*error*/
        {
            exit(1);
        }
        for (int i = maxfd+1; i >= 0; i--)
        {
            if (FD_ISSET(i, &ready)&&(i!=0))
            {
                if (i == fd)
                { // novo cliente

                    if ((newfd = accept(fd, &addr, &addrlen)) == -1)
                        exit(1); /*error*/
                    fprintf(stdout, "newfd = %d\n", newfd);
                    maxfd = max(maxfd, newfd);
                    FD_SET(newfd, &rfds);
                }

                else
                { // ja existente
                    fprintf(stdout, "ta certo%d\n",i);
                    n = read(i, buffer, 128);
                    
                    fprintf(stdout, "2ta certo%d\n",i);

                    if (n == -1) /*error*/
                        exit(1);
                    ptr = &buffer[0];
                    while (n > 0)
                    {
                        if ((nw = write(i, ptr, n)) <= 0) /*error*/
                            exit(1);
                        n -= nw;
                        ptr += nw;
                    }
                    if ((strncmp(buffer, "ola\n", 3)) != 0){
                        FD_CLR(i,&rfds);
                        close(i);
                    }

                }
            }
            if (FD_ISSET(i, &ready)&&(i==0))
            {

                fgets(buffer, sizeof(buffer), stdin);
                    if ((strncmp(buffer, "join", 4))==0){
                        printf("hello");
                        if((errcode=getaddrinfo("193.136.138.142","59000",&hints,&res))!=0)/*error*/exit(1);
                        if((fdudp=socket(AF_INET,SOCK_DGRAM,0))==-1)exit(1);//error
                        memset(&hints,0,sizeof hints);
                        hints.ai_family=AF_INET;//IPv4
	                    hints.ai_socktype=SOCK_DGRAM;//UDP socket
                        hints.ai_flags=AI_PASSIVE;
                        addrlen=sizeof(addr);
    
    
                        n=sendto(fdudp,"NODES 072",9,0,res->ai_addr,res->ai_addrlen);
                        addrlen=sizeof(addr);
                        n=recvfrom(fdudp,buffer,128,0,&addr,&addrlen);
                        if(n==-1)exit(1);
                        printf("hello");
                        buffer[n] = '\0';
                        fprintf(stdout, "echo: %s\n", buffer);
                        addrlen=sizeof(addr);
                        n=sendto(fdudp,"REG 072 51 172.17.127.197 58005",31,0,res->ai_addr,res->ai_addrlen);
                        addrlen=sizeof(addr);
                        n=recvfrom(fdudp,buffer,128,0,&addr,&addrlen);
                        if(n==-1)exit(1);
                        buffer[n] = '\0';
                        fprintf(stdout, "echo: %s\n", buffer);
                        memset(&hints,0,sizeof hints);
                        hints.ai_family=AF_INET;//IPv4
	                    hints.ai_socktype=SOCK_STREAM;//TCP socket
                        hints.ai_flags=AI_PASSIVE;
                    }
                    if((strncmp(buffer, "ola", 3))==0){
                        fflush(stdout);
                        fprintf(stdout, "naoconectou");
                        n=getaddrinfo("82.154.83.68","59000",&hints,&res);
	                    if(n!=0)exit(1);
                        if((connectfd=socket(AF_INET,SOCK_STREAM,0))==-1)exit(1);//error
	                    n=connect(connectfd,res->ai_addr,res->ai_addrlen);
	                    fprintf(stdout, "conectou");
                        if(n==-1)exit(1);
                        ptr=strcpy(buffer,"ola\n");

                        nbytes=7;
                        nleft=nbytes;
                        
                        while(nleft>0){
                            nwritten=write(connectfd,ptr,nleft);
                            if(nwritten<=0)exit(1);
                            nleft-=nwritten;
                            ptr+=nwritten;
                        }

                        nleft=nbytes; ptr=buffer;
                        
                        while(nleft>0){nread=read(connectfd,ptr,nleft);
                            if(nread==-1)exit(1);
                            else if(nread==0)break;//closed by peer
                            fprintf(stdout,"ola");
                            nleft-=nread;
                            ptr+=nread;
                        }
                        nread=nbytes-nleft;
                        buffer[nread] = '\0';
                        
                        fprintf(stdout,"echo: %s\n", buffer);
                    }
                    if((strncmp(opcao, "create name", 11))==0){
				    //create_name();
			        }else if((strncmp(opcao, "delete name", 11))==0){
				    //delete_name();
                    }else if((strncmp(opcao, "get dest name", 13))==0){
                        //delete_name();
                    }else if ((strncmp(opcao, "show topology", 13)||(strncmp(opcao, "st", 2)))==0){
                        //show_topology();
                    }else if ((strncmp(opcao, "show names", 10)||(strncmp(opcao, "sn", 2)))==0){
                        //show_names();
                    }else if ((strncmp(opcao, "show routing", 12)||(strncmp(opcao, "sr", 2)))==0){
                        //show_routing();
                    }else if ((strncmp(opcao, "leave", 4))==0){
                        freeaddrinfo(res);
                        /* remove nodo da rede */
                        close(fd);
                        //leave();
                        left = 1;
                    }else if ((strncmp(opcao, "exit", 4))==0){
                        if (left == 1)
                        {
                            //leave();
                        }
                        exit(0);
                    }else
                        fprintf(stdout, "you are alredy connected\n");
                    }
        }
    }
    /*close(fd);exit(0);*/
    close(newfd);
} // main
