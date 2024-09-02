#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>


//클라이언트 10명까지  제한하는 
#define CLNT_MAX 10

//소켓선언


#define BUFFSIZE 200

//글로벌 함수로 클라이언트 소켓 선언
int g_clnt_socks[CLNT_MAX];
int g_clnt_count = 0;

//mutex type, global
pthread_mutex_t g_mutex;


//아래 읽었던 내용을 모든 클라이언트에게 전달하는 함수 생성
void send_all_clnt(char * msg, int my_sock){
		
		//고객 동시 조인 아웃시 오류 발생 처리를 위한 mutex
		pthread_mutex_lock(&g_mutex);
		for (int i = 0 ; i < g_clnt_count ; i++){
			//나한테는 보내지 마시오
			if(g_clnt_socks[i] != my_sock){
				printf("send msg : %s", msg);
				write(g_clnt_socks[i], msg, strlen(msg)+1);
			}
		}
		pthread_mutex_unlock(&g_mutex);
}





//고객 전용 read함수
void *clnt_connection(void *arg){


	//클라이언트 소켓을 여기로 불러옴
	int clnt_sock = (int)arg;
	int str_len = 0;


	//200의 버프사이즈 배열 선언
	char msg[BUFFSIZE];
	int i;
	
	//clnt sock으로 read
	while(1){


		//읽으면
	str_len = read(clnt_sock,msg, sizeof(msg));
		if(str_len == -1){
			printf("client[%d] closed\n",clnt_sock);
			
			
			break;
		}
		send_all_clnt(msg,clnt_sock);
			
		//read한 것 뿌리기
		printf("%s\n",msg);
	}
	
	pthread_mutex_lock(&g_mutex);
	for(i=0; i<g_clnt_count; i++){
		if(clnt_sock == g_clnt_socks[i]){
			for(;i<g_clnt_count-1;i++)
				g_clnt_socks[i]=g_clnt_socks[i+1];
			break;
		}
	}
	pthread_mutex_unlock(&g_mutex);

		
	close(clnt_sock);

	pthread_exit(0);
	return NULL;
		
}

int main(int argc, char **argv){

	
	//소켓선언 server, clinet
	int serv_sock;
	int clnt_sock;

	
	pthread_t t_thread;


	//클라이언트 주소와 크기 지정
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;

	//서버주소 선언
	struct sockaddr_in serv_addr;
	

	//mutex 생성
	pthread_mutex_init(&g_mutex,NULL);
	
	
	//빈껍데기 소켓생성
	//PF_INE 함수 설정 IPV4의 형태로 생성 ( TCPIPv4 )
	//SOCK_STREAM 설정하여 TCP 전송 선언)
	//0 : 변수를 자동으로 할당되도록 설정
	serv_sock = socket(PF_INET,SOCK_STREAM, 0);
	
	//bind 설정
	serv_addr.sin_family = AF_INET;
	

	//소켄 바인드 시 주소 할당
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//포트설정
	serv_addr.sin_port = htons(7989);

	//bind 진행
	//bind error 처리까지 진행
	//man bind 
	if(bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) == -1){
		printf("bind error\n");
	}


	//linsten 설정
	//listen 오류 설정
	if(listen(serv_sock, 5) == -1){
		printf("listen error\n");
	}


	char buff[200];
	int recv_len = 0;
	while(1){

		
		//클라이언트 소켓 accept, 무한루프 형태로 계속 시도함
		clnt_addr_size = sizeof(clnt_addr);
		//client ip와 size 허용
		clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_addr, &clnt_addr_size);
		//clnt 카운트 추가, 소켓도
		//g_clnt_sock[g_clnt_count++] = clnt_sock;

		pthread_mutex_lock(&g_mutex);
		g_clnt_socks[g_clnt_count++] = clnt_sock;
		pthread_mutex_unlock(&g_mutex);


		//스레드 생성, 
		pthread_create(&t_thread, NULL, clnt_connection, (void *)clnt_sock);
		pthread_detach(t_thread); // 쓰레드 분리

		}

}
