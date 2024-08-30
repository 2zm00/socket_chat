#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>

#define BUFFSIZE 100
#define NAMESIZE 20

int StoHEX(char fi, char sc);
void error_handling(char *msg);
void * send_message(void * arg);
void * recv_message(void* arg);

char message[BUFFSIZE];


int main(int argc, char **argv){

	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_result;

 	//소켓 생성
	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		printf("socket() error\n");
	}	
	else {
		printf("socket ok\n");
	}

	//들어갈 수조
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;

	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(7989);

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){

	printf("connection error\n");
	}
	else{
		printf("connection success\n");
	}		

	unsigned char msg[100] = {0x01, 2,3,4,5,6,1,2,3,4,2,1,2,3,0x0c};
	
	printf("while before\n");
	while(1){

	printf("send : ");

	write(sock,msg,15);
		sleep(2);
	}

	printf("while end\n");
	close(sock);
	return 0;

}//end main

	


