#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
char map2[11][7] = {' '};
char map3[16][10] = {' '};
char map4[21][13] = {' '};
int scores[4] = {0};
char sentence[512];
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
void map_init(int players_num)
{
    switch (players_num)
    {
        case 2:
            for(int i=0;i<11;i++)
            {
                for(int j=0;j<7;j++)
                {
                    if((i%5 == 0) && (j%3 == 0))
                        map2[i][j] = '*';
                    else    
                        map2[i][j] = ' ';
                }
            }
            break;
        case 3:
            for(int i=0;i<16;i++)
            {
                for(int j=0;j<10;j++)
                {
                    if((i%5 == 0) && (j%3 == 0))
                        map3[i][j] = '*';
                    else    
                        map3[i][j] = ' ';
                }
            }
            break;
        case 4:
            for(int i=0;i<21;i++)
            {
                for(int j=0;j<13;j++)
                {
                    if((i%5 == 0) && (j%3 == 0))
                        map4[i][j] = '*';
                    else    
                        map4[i][j] = ' ';
                }
            }
            break;
    }
}
void print_map(int players_num,int players [4])
{
    write(1,"\n",1);
    switch (players_num)
    {
        case 2:
            sprintf(sentence,"scores-> player%d: %d player%d: %d\n",players[0],scores[0],players[1],scores[1]);
            puts(sentence);
            for(int j =0 ;j<7;j++)
            {
                for(int i=0;i<11;i++)
                    printf("%c",map2[i][j]);
                printf("\n");
            }
            break;
        case 3:
            sprintf(sentence,"scores-> player %d: %d player%d: %d player%d: %d\n",players[0],scores[0],players[1],scores[1],players[2],scores[2]);
            puts(sentence);
            for(int j =0; j<10;j++)
            {
                for(int i=0;i<16;i++)
                    printf("%c",map3[i][j]);
                printf("\n");
            }
            break;
        case 4:
            sprintf(sentence,"scores-> player%d: %d player%d: %d player%d: %d player%d: %d\n",players[0],scores[0],players[1],scores[1],players[2],scores[2],players[3],players[3]);
            puts(sentence);
            for(int j =0 ;j<13;j++)
            {
                for(int i=0;i<21;i++)
                    printf("%c",map4[i][j]);
                printf("\n");
            }
            break;
    }
    write(1,"\n",1);
}
void draw_line(int players_num,int row,int column,int vertical)
{
    switch (players_num)
    {
        case 2:
            if(vertical == 1)
            {
                for (int i = 5*column + 1 ; i<5*(column+1); i++ )
                    map2[i][row*3] = '-';
            }
            else if(vertical == 2)
            {
                for (int j = 3*row + 1 ; j<3*(row+1); j++ )
                    map2[5*column][j] = '|';
            }
            break;
        case 3:
            if(vertical == 1)
            {
                for (int i = 5*column + 1 ; i<5*(column+1); i++ )
                    map3[i][row*3] = '-';
            }
            else if(vertical == 2)
            {
                for (int j = 3*row + 1 ; j<3*(row+1); j++ )
                    map3[5*column][j] = '|';
            }
            break;
        case 4:
            if(vertical == 1)
            {
                for (int i = 5*column + 1 ; i<5*(column+1); i++ )
                    map4[i][row*3] = '-';
            }
            else if(vertical == 2)
            {
                for (int j = 3*row + 1 ; j<3*(row+1); j++ )
                    map4[5*column][j] = '|';
            }
            break;
    }
}

int above_box_made(int players_num,int row,int column)
{
    switch (players_num)
    {
        case 2:
            if (row == 0) return 0;
            if(map2[5*column][3*row - 1] == '|' && map2[5*column + 5][3*row - 1] == '|' && map2[5*column + 1][3*row - 3] == '-')
                return 1;
        case 3:
            if (row == 0) return 0;
            if(map3[5*column][3*row - 1] == '|' && map3[5*column + 5][3*row - 1] == '|' && map3[5*column + 1][3*row - 3] == '-')
                return 1;
        case 4:
            if (row == 0) return 0;
            if(map4[5*column][3*row - 1] == '|' && map4[5*column + 5][3*row - 1] == '|' && map4[5*column + 1][3*row - 3] == '-')
                return 1;
    }
}
int below_box_made(int players_num,int row,int column)
{
    switch (players_num)
    {
        case 2:
            if(row == 2) return 0;
            if(map2[5*column][3*row + 1] == '|' && map2[5*column + 5][3*row + 1] == '|' && map2[5*column + 1][3*row + 3] == '-')
                return 1;
        case 3:
            if(row == 3) return 0;
            if(map3[5*column][3*row + 1] == '|' && map3[5*column + 5][3*row + 1] == '|' && map3[5*column + 1][3*row + 3] == '-')
                return 1;
        case 4:
            if(row == 4) return 0;
            if(map4[5*column][3*row + 1] == '|' && map4[5*column + 5][3*row + 1] == '|' && map4[5*column + 1][3*row + 3] == '-')
                return 1;
    }
}
int right_box_made(int players_num,int row,int column)
{
    switch (players_num)
    {
        case 2:
            if (column == 2) return 0;
            if(map2[5*column + 1][3*row ] == '-' && map2[5*column + 1][3*row + 3] == '-' && map2[5*column + 5][3*row + 1] == '|')
                return 1;
        case 3:
            if (column == 3) return 0;
            if(map3[5*column + 1][3*row ] == '-' && map3[5*column + 1][3*row + 3] == '-' && map3[5*column + 5][3*row + 1] == '|')
                return 1;
        case 4:
            if (column == 4) return 0;
            if(map4[5*column + 1][3*row ] == '-' && map4[5*column + 1][3*row + 3] == '-' && map4[5*column + 5][3*row + 1] == '|')
                return 1;
    }
}

int left_box_made(int players_num,int row,int column)
{
    switch (players_num)
    {
        case 2:
            if(column == 0) return 0;
            if(map2[5*column - 1][3*row ] == '-' && map2[5*column - 1][3*row + 3] == '-' && map2[5*column - 5][3*row + 1] == '|')
                return 1;
        case 3:
            if(column == 0) return 0;
            if(map3[5*column - 1][3*row ] == '-' && map3[5*column - 1][3*row + 3] == '-' && map3[5*column - 5][3*row + 1] == '|')
                return 1;
        case 4:
            if(column == 0) return 0;
            if(map4[5*column - 1][3*row ] == '-' && map4[5*column - 1][3*row + 3] == '-' && map4[5*column - 5][3*row + 1] == '|')
                return 1;
    }
}

int check_for_boxes(int players_num,int row,int column,int vertical,char player_id)
{
    int score=0;
    switch (players_num)
    {
        case 2:
            if(vertical == 1)
            {
                if(above_box_made(players_num,row,column) == 1)
                {
                    map2[5*column + 2][3*row - 2] = player_id;
                    score = 1;
                }
                if(below_box_made(players_num,row,column) == 1)
                {
                    map2[5*column + 2][3*row + 1] = player_id;
                    score = 1;
                }
            }
            else if(vertical == 2)
            {
                if(right_box_made(players_num,row,column) == 1)
                {
                    map2[5*column + 2][3*row + 1] = player_id;
                    score = 1;
                }
                if(left_box_made(players_num,row,column) == 1)
                {
                    map2[5*column - 3][3*row + 1] = player_id;
                    score = 1;
                }
            }
            break;

        case 3:
            if(vertical == 1)
            {
                if(above_box_made(players_num,row,column) == 1)
                {
                    map3[5*column + 2][3*row - 2] = player_id;
                    score = 1;
                }
                if(below_box_made(players_num,row,column) == 1)
                {
                    map3[5*column + 2][3*row + 1] = player_id;
                    score = 1;
                }
            }
            else if(vertical == 2)
            {
                if(right_box_made(players_num,row,column) == 1)
                {
                    map3[5*column + 2][3*row + 1] = player_id;
                    score = 1;
                }
                if(left_box_made(players_num,row,column) == 1)
                {
                    map3[5*column - 3][3*row + 1] = player_id;
                    score = 1;
                }
            }
            break;

        case 4:
            if(vertical == 1)
            {
                if(above_box_made(players_num,row,column) == 1)
                {
                    map4[5*column + 2][3*row - 2] = player_id;
                    score = 1;
                }
                if(below_box_made(players_num,row,column) == 1)
                {
                    map4[5*column + 2][3*row + 1] = player_id;
                    score = 1;
                }
            }
            else if(vertical == 2)
            {
                if(right_box_made(players_num,row,column) == 1)
                {
                    map4[5*column + 2][3*row + 1] = player_id;
                    score = 1;
                }
                if(left_box_made(players_num,row,column) == 1)
                {
                    map4[5*column - 3][3*row + 1] = player_id;
                    score = 1;
                }
            }
            break;
    }
    return score;
}
int next_turn(int playernum,int players [4])
{
    if (players[0] == playernum)
        return 1;

    if (players[1] == playernum)
    {
        if(players[2] != -1)
            return 2;
        else    
            return 0;
    }

    if (players[2] == playernum)
    {
        if(players[3] != -1)
            return 3;
        else    
            return 0;
    }

    if(players[3] == playernum)
        return 0;
}
void itoa(char buf[32],int val, int base)
{
	int i = 30;
	for(; val && i ; --i, val /= base)
    {
		buf[i] = "0123456789abcdef"[val % base];
    }
}	

int who_has_got_score(int now_turn, int players [4])
{
    if(now_turn == players[0])
    {
        if(players[3] != -1)
            return players[3];
        if(players[2] != -1)
            return players[2];
        return players[1];
    }
    for(int i=1;i<4;i++)
    {   
        if(players[i] == now_turn)
            return players[i-1];
    }
}

void handle_scores(int got_score,int players [4])
{
    for(int i=0;i<4;i++)
        if(players[i] == got_score)
            scores[i]++;
}
void print_winner(int players [4])
{
    if (players[2] == -1)
    {
        if(scores[0] == 2 && scores[1] ==2)
        {
            write(1,"Draw!\n",6);
            return;
        }
    }
    if (players[3] == -1)
    {
        if(scores[0] == 3 && scores[1] ==3 && scores[2] == 3)
        {
            write(1,"Draw!\n",6);
            return;
        }
    }
    if (players[3] != -1)
    {
        if(scores[0] == 4 && scores[1] ==4 && scores[2] == 4 && scores[3] == 4)
        {
            write(1,"Draw!\n",6);
            return;
        }
    }
    int most_score = scores[0];
    int index = 0;
    for (int i=1;i<4;i++)
    {
        if(scores[i] > most_score)
        {
            most_score = scores[i];
            index = i;
        }
    }
    sprintf(sentence,"The winner is player %d \n ",players[index]);
    puts(sentence);
}
void sig_handler(int signum)
{
    write(1,"Time's up\n",strlen("Time's up\n"));
}

int main(int argc,char *argv[])
{
    siginterrupt(SIGALRM,1);
    signal(SIGALRM,sig_handler);
    if (argc < 2)
    {
        write(2 , "No ports\n",9);
        exit(1);
    }

    int sockfd , portNo ,n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[255];

    portNo = atoi(argv[1]);
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd < 0) 
        write(2,"Error opening socket\n",21);

    char *srvr = "127.0.0.1"  ;
    int players [4];
    server = gethostbyname(srvr);

    bzero((char *)&serv_addr , sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr , (char *) &serv_addr.sin_addr.s_addr , server->h_length);
    serv_addr.sin_port = htons(portNo);

    if(connect(sockfd , (struct sockaddr *) &serv_addr , sizeof(serv_addr)) < 0)
        write(2,"Connection failed\n",18);
    
    int players_num , portnum;
    bzero(buffer,255);

    n = read(sockfd , buffer , 255);
    if(n<0) 
        write(2,"Error reading\n",14);

    sprintf(sentence,"server - %s",buffer);
    puts(sentence);


    read(0,buffer,255);
    players_num = atoi(buffer);
    n = write(sockfd,&players_num,sizeof(int));

    if (n < 0)
        write(2,"Error writing\n",14);
    
    n = read(sockfd,&portnum,sizeof(int));
    if (n < 0)
        write(2,"Error reading\n",14);

    sprintf(sentence,"Your game's port number is : %d\n",portnum);
    puts(sentence);

    n = read(sockfd,&players,sizeof(players));
    if (n < 0)
        write(2,"reading err\n",12);
    
    write(1,"Players are : \n",strlen("Players are : \n"));
    for (int i=0 ; i <4; i++)
        if (players[i] != -1)
        {
            sprintf(sentence,"%d ",players[i]);
            puts(sentence);
        }

    int my_sockid;
    n = read(sockfd,&my_sockid,sizeof(my_sockid));
    if (n < 0)
        write(2,"reading error\n",14);

    close(sockfd);


//UDP starts
    int sockfd2,numbytes;
    struct sockaddr_in UDP_addr; 
    int broadcast = 1;

    if ((sockfd2 = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {
        write(2,"error opening socket\n",21);
        exit(1);
    }

    if (setsockopt(sockfd2, SOL_SOCKET, SO_BROADCAST, &broadcast,sizeof broadcast) == -1) 
    {
        write(2,"error setsockopt (SO_BROADCAST)\n",32);
        exit(1);
    }

    if (setsockopt(sockfd2, SOL_SOCKET, SO_REUSEPORT, &broadcast,sizeof broadcast) == -1)
    {
        write(2,"error setsockopt (SO_REUSEPORT)\n",32);
        exit(1);
    }


    UDP_addr.sin_family = AF_INET;     
    UDP_addr.sin_port = htons(portnum); 
    UDP_addr.sin_addr.s_addr = inet_addr("255.255.255.255");

    memset(UDP_addr.sin_zero, '\0', sizeof UDP_addr.sin_zero);

    if(bind(sockfd2 , (struct sockaddr *) &UDP_addr , sizeof(UDP_addr)) < 0 ) 
    {
        write(2,"Binding failed\n",15);
        exit(1);
    }

    if ((numbytes=sendto(sockfd2, &portnum, sizeof(int), 0,
        (struct sockaddr *)&UDP_addr, sizeof UDP_addr)) == -1) 
    {
        write(2,"error sendto\n",13);
        exit(1);
    }

    map_init(players_num);

    switch(players_num)
    {
        case 2: print_map(2,players);
                break;
        case 3: print_map(3,players);
                break;
        case 4: print_map(4,players);
                break;
    }

    int turn = 0 ;
    int game_is_on = 5;
    struct sockaddr_in  cli_addr;

    socklen_t clilen;
    clilen = sizeof(cli_addr);
    int len = sizeof(struct sockaddr_in);
    char buf[32];
    bzero(buf,32);
    itoa(buf,players[turn],10);
    char rowbuf[32];
    char colbuf[32];
    char vertbuf[32];
    char turn_in_char[32];
    int boxes=0;
    while(1)
    {
        if(players_num == 2){
            if(boxes == 4) 
                break;
        }
        if(players_num == 3){
            if(boxes == 9) 
                break;
        }
        if(players_num == 4){
            if(boxes == 16) 
                break;
        }   

        if (buf[30] - '0' == my_sockid )
        {
            int row,column,vertical;

            turn = next_turn(buf[30] - '0',players);
            bzero(buf,32);
            itoa(buf,players[turn],10);
            write(1,"Its your turn \nEnter 3 numbers(row column vertical(1)horizontal(2)) : \n",strlen("Its your turn \nEnter 3 numbers(row column vertical(1)horizontal(2)) : \n"));
            read(0,buffer,256);
            row = atoi(buffer);
            read(0,buffer,256);
            column = atoi(buffer);
            read(0,buffer,256);
            vertical = atoi(buffer);

            if ((sendto(sockfd2, buf, 32, 0,
                (struct sockaddr *)&UDP_addr, sizeof UDP_addr)) == -1) 
            {
                write(2,"error sending to\n",17);
                exit(1);
            }
            itoa(rowbuf,row,10);
            if ((sendto(sockfd2, rowbuf, 32, 0,
                (struct sockaddr *)&UDP_addr, sizeof UDP_addr)) == -1) 
            {
                write(2,"error sending to\n",17);
                exit(1);
            }
            itoa(colbuf,column,10);
            if ((sendto(sockfd2, colbuf, 32, 0,
                (struct sockaddr *)&UDP_addr, sizeof UDP_addr)) == -1) 
            {
                write(2,"error sending to\n",17);
                exit(1);
            }
            itoa(vertbuf,vertical,10);
            if ((sendto(sockfd2, vertbuf, 32, 0,
                (struct sockaddr *)&UDP_addr, sizeof UDP_addr)) == -1) 
            {
                write(2,"error sending to\n",17);
                exit(1);
            }

        }
        else
        {
            int newrow,newcol,newvert;
            sprintf(sentence,"waiting for player %d's turn\n",buf[30] - '0');
            puts(sentence);
            numbytes = 0;
            while(numbytes != 32)
            {
                if (( numbytes = recv(sockfd2,buf,32, 0)) < 0)
                {
                    write(2,"error recieving from\n",21);
                    exit(1);
                } 
            }
            numbytes = 0;
            while(numbytes != 32)
            {
                if (( numbytes = recvfrom(sockfd2,rowbuf,32, 0,NULL, NULL)) ==-1)
                {
                    write(2,"error recieving from\n",21);
                    exit(1);
                } 
            }
            numbytes = 0;

            while(numbytes != 32)
            {
                if (( numbytes = recvfrom(sockfd2,colbuf,32, 0,NULL, NULL)) ==-1)
                {
                    write(2,"error recieving from\n",21);
                    exit(1);
                } 
            }
            numbytes = 0;

            while(numbytes != 32)
            {
                if (( numbytes = recvfrom(sockfd2,vertbuf,32, 0,NULL, NULL)) ==-1)
                {
                    write(2,"error recieving from\n",21);
                    exit(1);
                } 
            }
            int got_score = who_has_got_score(buf[30] - '0',players);
            itoa(turn_in_char,got_score,10);
            draw_line(players_num,rowbuf[30]-'0' -1,colbuf[30] - '0' - 1,vertbuf[30] - '0' );
            int score = check_for_boxes(players_num,rowbuf[30]-'0' -1,colbuf[30] - '0' - 1,vertbuf[30] - '0',turn_in_char[30] );
            if (score == 1)
            {
                itoa(buf,got_score,10);
                handle_scores(got_score,players);
                boxes++;
            }
            print_map(players_num,players);
        }  
    }

    print_winner(players);
    
    close(sockfd2);
    return 0;
}