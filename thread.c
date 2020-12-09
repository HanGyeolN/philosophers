#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

long int		get_timestamp_ms(struct timeval *current, struct timeval *birth)
{
	long int	dsec;
	int			dusec;
	long int	dmsec;

	dsec = current->tv_sec - birth->tv_sec;
	dusec = (current->tv_usec - birth->tv_usec);
	dmsec = dsec * 1000 + (long int)(dusec / 1000);
	return (dmsec);
}

void	*t_function(void *data)
{
	int					num;
	struct timeval		birth;
	struct timeval		current;
	
	gettimeofday(&birth, NULL);
	num = (*(int *)data);
	while (1)
	{
		gettimeofday(&current, NULL);
		printf("%ld %d is eating\n", get_timestamp_ms(&current, &birth), num);
		
		gettimeofday(&current, NULL);
		printf("%ld %d is sleeping\n", get_timestamp_ms(&current, &birth), num);
		usleep(1000000);
		
		gettimeofday(&current, NULL);
		printf("%ld %d is thiking\n", get_timestamp_ms(&current, &birth), num);
	
		if (get_timestamp_ms(&current, &birth) > 100000)
			break;
	}
	return (0);
}

int main()
{
    pthread_t p_thread[2];
    int thr_id;
    int status;
    int	p[3];

	p[0] = 0;
	p[1] = 1;
	p[2] = 2;
 
    thr_id = pthread_create(&p_thread[0], NULL, t_function, (void *)&p[0]);
	// pthread_detach(p_thread[0]);
    // pthread_create() 으로 성공적으로 쓰레드가 생성되면 0 이 리턴됩니다
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
 
    // ② 2번 쓰레드 생성
    thr_id = pthread_create(&p_thread[1], NULL, t_function, (void *)&p[1]);
	// pthread_detach(p_thread[1]);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
 
    // 쓰레드 종료를 기다린다. 
    pthread_join(p_thread[0], (void **)&status);
    pthread_join(p_thread[1], (void **)&status);
	
    printf("언제 종료 될까요?\n");
 
    return 0;
}