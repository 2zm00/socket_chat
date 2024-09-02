#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>



int a=0;
pthread_mutex_t mutex;

//스레드 생성함수
void * thread1(void * arg){
	
	//인자 넘기기
	printf("arg : %d\n", (int)arg);

	while(1){
		
		//아래 mutex init한 것을 사용함
		pthread_mutex_lock(&mutex);
		//쓰레드 할당한걸 사용한다
		printf("thread1%d a[%d]\n", (int)arg, ++a);		
		//mutex lock한것을 언락함
		pthread_mutex_unlock(&mutex);

		sleep(2);



	}

	return NULL;

}





int main(){

	pthread_t s_thread[2];
	
	//스레드 넘길인자
	int id1 = 77;
	int id2 = 88;
	

	//mutex init (NULL:기본옵션 사용)
	pthread_mutex_init(&mutex,NULL);


	//쓰레드 생성 핸들을 만들어서 조인 또는 메인루프에서 클로즈)
	pthread_create(&s_thread[0],NULL,thread1, (void *)id1);
	pthread_create(&s_thread[1],NULL,thread1, (void *)id2);
	
	//join을 사용한 쓰레드
	//pthread_join(s_thread[0],NULL);
	//pthread_join(s_thread[1],NULL);


	//루프를 활용한 쓰레드 
	while(1){
		printf("main loop\n");
		sleep(1);



}

}

