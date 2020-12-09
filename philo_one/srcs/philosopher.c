#include "philosophers.h"

t_info				info;
pthread_mutex_t		mutx;

int		set_philo_info(t_info *info,char **argv)
{
	info->number_of_philosophers = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		info->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	return (0);
}

int		check_philo_info(t_info *info)
{
	if (info->number_of_philosophers < 0 || info->time_to_die < 0 || \
		info->time_to_eat < 0 || info->time_to_sleep < 0 || \
		info->number_of_times_each_philosopher_must_eat < 0)
		return (1);
	printf("number_of_philosophers: %d\n", info->number_of_philosophers);
	printf("time_to_die:%d\n", info->time_to_die);	
	printf("time_to_eat:%d\n", info->time_to_eat);
	printf("time_to_sleep:%d\n", info->time_to_sleep);
	return (0);
}

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

/*
** 철학자 상태 출력
** 여러 쓰레드가 서로 write를 하기때문에 문자 출력이 섞이는 경우가 있는데
** 이를 막기위해서 문자열을 한번에 출력해야한다.
*/

long	print_philo(t_philosopher *philo)
{
	char			*str;
	char			*temp;
	struct timeval	current;
	long			timestamp;

	gettimeofday(&current, NULL);
	timestamp = get_timestamp_ms(&current, &philo->birth);
	str = ft_itoa(timestamp);
	str = ft_strjoin(str, " ");
	temp = ft_itoa(philo->num);
	str = ft_strjoin(str, temp);
	free(temp);
	if (philo->status == TAKE_FORK)
		str = ft_strjoin(str, " has taken a fork\n");
	else if (philo->status == EATING)
		str = ft_strjoin(str, " is eating\n");
	else if (philo->status == SLEEPING)
		str = ft_strjoin(str, " is sleeping\n");
	else if (philo->status == THINKING)
		str = ft_strjoin(str, " is thinking\n");
	else if (philo->status == DIED)
		str = ft_strjoin(str, " died\n");
	ft_putstr(str);
	free(str);
	return (timestamp);
}

void	*life(void *data)
{
	struct timeval		current;
	t_philosopher		*philo;
	long				time;
	long				stamp;

	philo = (t_philosopher *)data;
	gettimeofday(&(philo->birth), NULL);
	philo->status = THINKING;
	philo->last_eat = 0;
	time = 0;
	if (philo->num % 2 == 0)
	{
		usleep(1000);
	}
	while (1)
	{
		gettimeofday(&current, NULL);
		// if (time != 0)
		// {
		// 	printf("while loop time: %ldms\n", get_timestamp_ms(&current, &philo->birth) - time);
		// }
		time = get_timestamp_ms(&current, &philo->birth);
		if (time - philo->last_eat >= (long int)info.time_to_die)
		{
			philo->status = DIED;
			print_philo(philo);
			printf("[delay: %ldms]\n", time);
			break;
		}

		if (philo->status == THINKING)
		{
			pthread_mutex_lock(&mutx);
			if (philo->left_fork->status == 1 && philo->right_fork->status == 1)
			{
				philo->left_fork->status = 0;
				philo->right_fork->status = 0;
				philo->status = TAKE_FORK;
			}
			pthread_mutex_unlock(&mutx);
		}
		if (philo->status == TAKE_FORK)
		{
			print_philo(philo);
			philo->status = EATING;
		}
		else if (philo->status == EATING)
		{
			stamp = print_philo(philo);
			usleep(info.time_to_eat * 1000);

			gettimeofday(&current, NULL);
			philo->last_eat = get_timestamp_ms(&current, &philo->birth);
			
			// drop fork
			pthread_mutex_lock(&mutx);
			philo->left_fork->status = 1;
			philo->right_fork->status = 1;
			pthread_mutex_unlock(&mutx);

			philo->status = SLEEPING;
			stamp = print_philo(philo);
		}
		else if (philo->status == SLEEPING)
		{
			usleep(info.time_to_sleep * 1000);

			philo->status = THINKING;
			print_philo(philo);
		}
		usleep(10000);
	}
	return (0);
}

int		philosophers(char **argv)
{
	int				i;
	t_philosopher	*philos;
	pthread_t		*ptds;

	i = 0;
	set_philo_info(&info, argv);
	check_philo_info(&info);

	// 쓰레드 생성
	if (!(ptds = malloc(sizeof(pthread_t) * info.number_of_philosophers)))
		return (1);

	// 철학자 생성
	if (!(philos = malloc(sizeof(t_philosopher) * info.number_of_philosophers)))
	{
		free(ptds);
		return (1);
	}
	// 포크 생성
	while (i < info.number_of_philosophers)
	{
		philos[i].num = i + 1;
		if (!(philos[i].right_fork = malloc(sizeof(t_fork) * 1)))
			return (1);
		philos[i].right_fork->status = 1;
		i++;
	}
	i--;
	philos[i].left_fork = philos[0].right_fork;
	while (--i >= 0)
	{
		philos[i].left_fork = philos[i + 1].right_fork;
	}
	// 쓰레드 시작
	i = 0;
	while (i < info.number_of_philosophers)
	{
		pthread_create(&(ptds[i]), NULL, life, (void *)&(philos[i]));
		i++;
	}

	// 쓰레드 기다림
	i = 0;
	while (i < info.number_of_philosophers)
	{
		pthread_join(ptds[i], NULL);
		i++;
	}

	return (0);
}

int		main(int argc, char **argv)
{
	if (argc == 5 || argc == 6)
	{
		//check argv;
		pthread_mutex_init(&mutx, NULL);
		if (philosophers(argv))
		{
			ft_putstr("philosophers error\n");
			pthread_mutex_destroy(&mutx);
			return (1);
		}
		pthread_mutex_destroy(&mutx);
		return (0);
	}
	ft_putstr("Argument error\n");
	return (1);
}