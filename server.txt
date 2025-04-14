#include <sys/types.h>   /* basic system data types */
#include <sys/socket.h>  /* basic socket definitions */
#include <sys/time.h>    /* timeval{} for select() */
#include <time.h>        /* timespec{} for pselect() */
#include <netinet/in.h>  /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>   /* inet(3) functions */
#include <errno.h>
#include <fcntl.h>       /* for nonblocking */
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>		/* for OPEN_MAX */
#include <poll.h>
#include <unistd.h>
#include "cards.h"
#include "mechanism.h"


#define MAXLINE 1024
#define SA struct sockaddr
#define LISTENQ 2
#define INFTIM -1


#define MAX_LIMIT 20


int game_in_progress    = 0;
int end_game            = 1;
int end_round           = 0;
int number_of_cards     = 2;
int highscore           = 0;
int not_string_index    = 0;
int round_number        = 1;


struct Player{
    char    name[20];
    char*   pname;
    char*   two_cards[10];
    int     two_cards_number[2];
    int*    ptwo_cards_number;
    int     two_cards_value[2];
    int*    ptwo_cards_value;
    int     score;
    int     status;
    int*    pstatus;
};


void Fputs(const char *ptr, FILE *stream){
    if (fputs(ptr, stream) == EOF)
        perror("fputs error");
}


void FastSort(struct Player* player){
    int temp;

    temp = player->two_cards_value[0];
    if(temp > player->two_cards_value[1]) {
        player->two_cards_value[0] = player->two_cards_value[1];
        player->two_cards_value[1] = temp;
    }
}


int CheckScore(struct Player* player, int const* pcards_on_the_table_number, int const* pcards_on_the_table_value){
    int value;


    if (IsStraightFlush(player->ptwo_cards_number, pcards_on_the_table_number) > 0) {
        player->score = 1600;
        return 0;
    }
    else if ((value = IsFourOfKind(player->ptwo_cards_value, pcards_on_the_table_value)) > 0) {
        player->score = 1400 + value;
        return 0;
    }
    else if (IsFullHouse(player->ptwo_cards_value, pcards_on_the_table_value) > 0) {
        player->score = 1200;
        return 0;
    }
    else if (IsFlush(player->ptwo_cards_number, pcards_on_the_table_number) > 0) {
        player->score = 1000;
        return 0;
    }
    else if ((value = IsStraight(player->ptwo_cards_value, pcards_on_the_table_value)) > 0) {
        player->score = 800 + value;
        return 0;
    }
    else if ((value = IsThreeOfKind(player->ptwo_cards_value, pcards_on_the_table_value)) > 0) {
        player->score = 600 + value;
        return 0;
    }
    else if ((value = IsTwoPairs(player->ptwo_cards_value, pcards_on_the_table_value)) > 0) {
        player->score = 400 + value;
        return 0;
    }
    else if ((value = IsOnePair(player->ptwo_cards_value, pcards_on_the_table_value)) > 0) {
        player->score = 200 + value;
        return 0;
    }
    else {
        int array[7];
        player->score = SortAllCards(player->two_cards_value, pcards_on_the_table_value, array);
        return 0;
    }

}


ssize_t writen(int fd, const void *vptr, size_t n){
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else
				return(-1);			/* error */
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n);
}


void Writen(int fd, void *ptr, size_t nbytes){
	if (writen(fd, ptr, nbytes) != nbytes)
		perror("writen error");
}


int main(int argc, char **argv){
	int	listenfd;
    int connfd;
    int sockfd;
    int	i, j;
    int maxi;
    int n;
    int	nready;

	socklen_t	clilen;

	void		sig_chld(int);
    char        addr_buf[INET6_ADDRSTRLEN+1];

	struct pollfd	            client[FOPEN_MAX];
    struct sockaddr_in6	        cliaddr;
    struct sockaddr_in6         servaddr;

    struct Player               player[2];

    int cl_count = 2;


    for(i = 0; i < cl_count; i++){

        player[i].score = 0;

        player[i].pname = (char *) &player[i].name;

        player[i].two_cards_number[0] = 0;
        player[i].two_cards_number[1] = 0;
        player[i].ptwo_cards_number = &player[i].two_cards_number[0];

        player[i].two_cards_value[0] = 0;
        player[i].two_cards_value[1] = 0;
        player[i].ptwo_cards_value = &player[i].two_cards_value[0];

        player[i].pstatus = &player[i].status;

    }



	if ( (listenfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0){
	       fprintf(stderr,"socket error : %s\n", strerror(errno));
	       return 1;
	}


	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin6_family = AF_INET6;
	servaddr.sin6_addr   = in6addr_any;
	servaddr.sin6_port   = htons(7);	/* echo server */

	if ( bind( listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
	        fprintf(stderr,"bind error : %s\n", strerror(errno));
	        return 1;
	}
	
	if ( listen(listenfd, LISTENQ) < 0){
	        fprintf(stderr,"listen error : %s\n", strerror(errno));
	        return 1;
	}


	client[0].fd = listenfd;
	client[0].events = POLLIN;
	for (i = 1; i < FOPEN_MAX; i++)
		client[i].fd = -1;		/* -1 indicates available entry */
	maxi = 0;					/* max index into client[] array */


    int ready = 0;


	for ( ; ; ) {

        if ((nready = poll(client, maxi + 1, INFTIM)) < 0) {
            perror("poll error");
            exit(1);
        }
        if (client[0].revents & POLLIN) {    /* new client connection */
            clilen = sizeof(cliaddr);
            ready++;
            if ((connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
                perror("accept error");
                exit(1);
            }


            bzero(addr_buf, sizeof(addr_buf));
            inet_ntop(AF_INET6, (struct sockaddr *) &cliaddr.sin6_addr, addr_buf, sizeof(addr_buf));

            printf("new client: %s, port %d\n", addr_buf, ntohs(cliaddr.sin6_port));


            for (i = 1; i < FOPEN_MAX; i++)
                if (client[i].fd < 0) {
                    client[i].fd = connfd;    /* save descriptor */
                    break;
                }
            if (i == FOPEN_MAX) {
                perror("too many clients");
                continue;
            }


            client[i].events = POLLIN;
            if (i > maxi) {
                maxi = i;
            }

            if (ready == cl_count) {
                game_in_progress = 1;
                break;
            }
        }
    }

    printf("Let the game begin!\n");
    sleep(2);

    for(i = 0; i < cl_count; i++) {
        if ((sockfd = client[i+1].fd) < 0)
            continue;

        Writen(sockfd, "Let the game begin!\nEnter your nickname:", 100);
        sleep(1);

        char recvline[MAXLINE];
        int check_point = 0;
        int round = 1;
        while (check_point != round) {
            if ((n = read(sockfd, recvline, MAXLINE)) > 0) {
                recvline[n] = 0;
                Fputs(recvline, stdout);
                strcpy(player[i].name, recvline);
                ++check_point;
            }
        }
    }

    sleep(2);

    if(game_in_progress) {
        int tag = 0;
        int *ptag;
        ptag = &tag;


        end_round = 0;


        ShuffleTheDeck();

        for (i = 0; i < cl_count; i++) {
            DealTheCards(player[i].two_cards, player[i].ptwo_cards_number, ptag);
            GetTheCardValue(player[i].ptwo_cards_number, player[i].ptwo_cards_value, number_of_cards);
            FastSort(&player[i]);
        }

        sleep(2);

        /* Sending cards to players */
        for(i = 0; i < cl_count; i++) {
            if ((sockfd = client[i+1].fd) < 0)
                continue;

            Writen(sockfd, "Your cards:", 100);
            sleep(1);
        }

        for(i = 0; i < cl_count; i++) {
            for(j = 0; j < 2; j++){

                if ((sockfd = client[i+1].fd) < 0)
                    continue;

                Writen(sockfd, player[i].two_cards[j], 100);
                sleep(1);
            }
        }




        char* cards_on_the_table[10];


        int cards_on_the_table_number[5] = {0, 0, 0, 0, 0};
        int* pcards_on_the_table_number = cards_on_the_table_number;


        int cards_on_the_table_value[5] = {0, 0, 0, 0, 0};
        int* pcards_on_the_table_value = cards_on_the_table_value;


        number_of_cards = 5;


        CardsOnTheTable(cards_on_the_table, pcards_on_the_table_number, number_of_cards, ptag);
        GetTheCardValue(pcards_on_the_table_number, pcards_on_the_table_value, number_of_cards);


        sleep(2);

        for(i = 0; i < cl_count; i++) {
            if ((sockfd = client[i+1].fd) < 0)
                continue;

            Writen(sockfd, "Cards on the table:", 100);
            sleep(1);
        }

        for(i = 0; i < cl_count; i++) {
            for(j = 0; j < 5; j++){

                if ((sockfd = client[i+1].fd) < 0)
                    continue;

                Writen(sockfd, *(cards_on_the_table + j), 100);
                sleep(1);
            }
        }


        for (i = 0; i < cl_count; i++) {
            printf("\nPlayer %d:\n", i + 1);
            for (j = 0; j < 2; j++) {
                printf("%s", *(player[i].two_cards + j));
                printf("\n");
            }
            printf("\n");
        }


        printf("\n\nCards on the table:\n");
        for (i = 0; i < number_of_cards; ++i) {
            printf("%s", *(cards_on_the_table + i));
            printf("\n");
        }


        for (i = 0; i < cl_count; i++) {
            CheckScore(&player[i], pcards_on_the_table_number, pcards_on_the_table_value);
            printf("\n");

            if (player[i].score >= highscore) {
                highscore = player[i].score;
                not_string_index = i;
                printf("Score = %d\n", highscore);
            }
        }

        printf("\nThe winner is!\n");
        printf("%s", player[not_string_index].name);

        sleep(2);

        for(i = 0; i < cl_count; i++) {
            if ((sockfd = client[i+1].fd) < 0)
                continue;

            Writen(sockfd, "The winner is!", 100);
            sleep(1);
        }

        sleep(2);

        for(i = 0; i < cl_count; i++) {
            if ((sockfd = client[i+1].fd) < 0)
                continue;

            Writen(sockfd, player[not_string_index].name, 100);
            sleep(1);
        }


        for (i = 1; i <= maxi; i++) {	/* check all clients for data */
            if ( (sockfd = client[i].fd) < 0)
                continue;

            printf("client[%d] closed connection\n", i);
            close(sockfd);
            client[i].fd = -1;

            }
    }
}
