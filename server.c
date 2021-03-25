#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int room_2[4]= {-1,-1,-1,-1};
int room_3[4]= {-1,-1,-1,-1};
int room_4[4]= {-1,-1,-1,-1};
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void add_to_room(int room_number, int sockfd)
{
    switch(room_number)
    {
        case 2:
            for (int i=0;i<2;i++)
                if (room_2[i] == -1)
                {
                    room_2[i] = sockfd;
                    break;
                }
        case 3:
            for (int i=0;i<3;i++)
                if (room_3[i] == -1)
                {
                    room_3[i] = sockfd;
                    break;
                }
        case 4:
            for (int i=0;i<4;i++)
                if (room_4[i] == -1)
                {
                    room_4[i] = sockfd;
                    break;
                }
    }
}

int team_ready(int room_number)
{
    switch (room_number)
    {
        case 2:
            if (room_2[1] != -1)
                return 1;
        case 3: 
            if (room_3[2] != -1)
                return 1;
        case 4:            
            if (room_4[3] != -1)
                return 1;
        return 0;
    }
}

void make_room_empty(int room_number)
{
    switch (room_number)
    {
        case 2:
            for (int i=0; i< 2; i++)
                room_2[i] = -1;
        case 3: 
            for (int i=0; i< 3; i++)
                room_3[i] = -1;
        case 4:            
            for (int i=0; i< 4; i++)
                room_4[i] = -1;
    }
}

void set_broadcast_port(int bp,int room_number,int sockfd_server)
{
    int n;
    switch (room_number)
    {
        case 2:
            for (int i=0 ; i < 2 ; i++)
            {
                n = write(room_2[i],&bp,sizeof(int));
                if (n < 0) {
                    write(2,"write",5);
                    exit(1);
                }
                n = write(room_2[i],room_2,sizeof(room_2));
                if (n < 0) {
                    write(2,"write",5);
                    exit(1);
                }
                n = write(room_2[i],&room_2[i],sizeof(room_2));
                if (n < 0) {
                    write(2,"write",5);
                    exit(1);
                }
            }
            break;
        case 3:
            for (int i=0 ; i < 3 ; i++)
            {
                n = write(room_3[i],&bp,sizeof(int));
                if (n < 0) {
                    write(2,"write",5);
                    exit(1);
                }
                n = write(room_3[i],room_3,sizeof(room_3));
                if (n < 0) {
                    write(2,"write",5);
                    exit(1);
                }
                n = write(room_3[i],&room_3[i],sizeof(room_3));
                if (n < 0) {
                    write(2,"write",5);
                    exit(1);
                }
            }
            break;

        case 4:
            for (int i=0 ; i < 4 ; i++)
            {    
                n = write(room_4[i],&bp,sizeof(int));
                if (n < 0) {
                    write(2,"write",5);
                    exit(1);
                }
                n = write(room_4[i],room_4,sizeof(room_4));
                if (n < 0) {
                    write(2,"write",5);
                    exit(1);
                }
                n = write(room_4[i],&room_4[i],sizeof(room_4));
                if (n < 0) {
                    write(2,"write",5);
                    exit(1);
                }
            }
            break;
    }
}
int main(int argc,char *argv[])
{
    if (argc < 2)
    {
        write(2,"Enter port number\n",18);
        exit(1);
    }

    int maximum_fd,serverfd,newfd,n,i, j, rv;
    
    int broadcast_ports = 40000;

    struct sockaddr_storage remoteaddr; // client address
    
    socklen_t addrlen;

    char buf[256]; 

    char remoteIP[INET6_ADDRSTRLEN];

    int flag = 1;        

    struct addrinfo hints, *ai, *p;

    fd_set mainfds;
    fd_set read_fds;
    FD_ZERO(&mainfds); 
    FD_ZERO(&read_fds);

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, argv[1], &hints, &ai)) != 0) {
        write(2, "select error\n", 13);
        exit(1);
    }
    
    for(p = ai; p != NULL; p = p->ai_next) 
    {
        serverfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (serverfd < 0) 
            continue;
        
        setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));

        if (bind(serverfd, p->ai_addr, p->ai_addrlen) < 0) 
        {
            close(serverfd);
            continue;
        }

        break;
    }

    if (p == NULL) 
    {
        write(1, "binding failed\n",15);
        exit(2);
    }

    freeaddrinfo(ai); 

    if (listen(serverfd, 10) == -1) 
    {
        write(2,"listen",6);
        exit(3);
    }

    FD_SET(serverfd, &mainfds);

    maximum_fd = serverfd; 


    char sentence[256];
    for(;;)
    {
        read_fds = mainfds; 
        if (select(maximum_fd+1, &read_fds, NULL, NULL, NULL) == -1) 
        {
            write(2,"select",6);
            exit(4);
        }

        for(i = 0; i <= maximum_fd; i++) 
        {
            if (FD_ISSET(i, &read_fds)) 
            { 
                if (i == serverfd) 
                {
                    addrlen = sizeof (remoteaddr);

                    newfd = accept(serverfd,(struct sockaddr *)&remoteaddr,&addrlen);

                    if (newfd == -1) 
                    {
                        write(2,"accept",6);
                    } 
                    else 
                    {
                        FD_SET(newfd, &mainfds); 
                        if (newfd > maximum_fd) 
                            maximum_fd = newfd;

                        sprintf(sentence,"selectserver: new connection from %s on socket %d\n",
                            inet_ntop(remoteaddr.ss_family,get_in_addr((struct sockaddr*)&remoteaddr),remoteIP, INET6_ADDRSTRLEN),
                            newfd);
                        puts(sentence);

                        n = write(newfd,"Enter number of players you want to play with : \n2 players\n3 players\n4 players\n",
                        strlen("Enter number of players you want to play with : \n2 players\n3 players\n4 players\n"));
                        if (n < 0) 
                            write(2,"write",5);
                    }
                } 
                else 
                {
                    int room_number,bytes_received;
                    if ((bytes_received = recv(i, &room_number, sizeof (int), 0)) <= 0) 
                    {
                        if (bytes_received == 0) 
                        {
                            sprintf(sentence,"selectserver: socket %d hung up\n", i);
                            puts(sentence);
                        }
                        else 
                            write(2,"recv",4);
                        close(i); 
                        FD_CLR(i, &mainfds); 
                    } 
                    else 
                    {
                        sprintf(sentence,"cilent %d - player number is : %d\n",i,room_number);
                        puts(sentence);
                        add_to_room(room_number , i);
                        
                        if(team_ready(room_number) == 1)
                        {
                            set_broadcast_port(broadcast_ports,room_number,serverfd);
                            make_room_empty(room_number);
                            broadcast_ports++;
                        }
                        else
                            write(1,"Searching for players...\n",25);                                                                

                    }
                } 
            } 
        } 
    } 
    
    return 0;
}
