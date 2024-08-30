#include <stdio.h>
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



//글로벌 함수로 클라이언트 소켓 선언
int g_clnt_socks[CLNT_MAX];
int g_clnt_count = 0;


int main(int argc, char ** argv){

	
	//소켓선언 server, clinet
	int serv_sock;
	int clnt_sock;


	//클라이언트 주소와 크기 지정
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;

	//서버주소 선언
	struct sockaddr_in serv_addr;


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



		//스레드 생성, 
		
		while(1){
		//받기
		recv_len = read(clnt_sock, buff, 200);

		printf("recv : ");
		for(int i =0; i < recv_len ; i++){

			printf("%02X ", (unsigned char)buff[i]);
		}
		printf("\n");
		}
	}


}
