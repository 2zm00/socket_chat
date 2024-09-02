#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define BUFFSIZE 100
#define NAMESIZE 20

int StoHEX(char fi, char sc);
void error_handling(char *msg);
void * send_message(void * arg);
void * recv_message(void* arg);

char message[BUFFSIZE];

//thread 생성 
void * rcv(void * arg){

	printf("rcv thread created\n");
	int sock = *(int *)arg;
	char buff[500];
	int len;
	while(1){
		len = read(sock,buff,sizeof(buff));

		if(len == -1){
			printf("sock close\n");
			break;
		}
		printf("%s",buff);
	}

	pthread_exit(0);
	return 0;
}

int main(int argc, char **argv){

	int sock;
	struct sockaddr_in serv_addr;
	pthread_t rcv_thread;
	void * thread_result;
	
	//clnt id 생성
	char id[100];
	
	printf("argc : %d\n",argc);
	if(argc < 2){
		printf("you have to enter ID\n");
		return 0;
	}

	strcpy(id, argv[1]);
 	printf("id : %s\n",id);

		
	//소켓 생성
	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		printf("socket() error\n");
	}	
	else {
		printf("socket ok\n");
	}

	//들어갈 주소
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	
	//서버IP, Port 지정
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(7989);

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){

	printf("connection error\n");
	}
	else{
		printf("connection success\n");
	}	

	//쓰레드 생성 
	pthread_create(&rcv_thread, NULL, rcv,(void *)&sock);	


	//메세지 내용 크기할당
	char chat[BUFFSIZE];
	char msg[BUFFSIZE + NAMESIZE];
	
	printf("while before\n");
	while(1){
	//채팅입력
	printf("채팅 입력 : ");
	fgets(chat, sizeof(chat), stdin);
	chat[strcspn(chat, "\n")] = '\0';
	snprintf(msg, sizeof(msg),"[%s] : %s\n", id, chat);
	printf("send : %s", msg);
	write(sock,msg,strlen(msg)+1);
	//전송시간 조절 2초
	sleep(1);

	}

	pthread_cancel(rcv_thread);
	pthread_join(rcv_thread, NULL);
	printf("while end\n");
	close(sock);
	return 0;

}//end main

	


