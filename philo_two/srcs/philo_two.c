#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(long nb)
{
	long long int n;
	long long int div;
	long long int mod;

	n = (long long int)nb;
	if (n < 0)
	{
		n = -n;
		ft_putchar('-');
	}
	div = n / 10;
	mod = n % 10;
	if (n >= 10)
	{
		ft_putnbr(div);
	}
	ft_putchar(mod + '0');
}

int		ft_strlen(char *str)
{
	int		i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

int		ft_atoi(const char *s)
{
	int		i;
	int		ret;
	int		sign;

	i = 0;
	ret = 0;
	sign = 1;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -sign;
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		ret = ret * 10 + (s[i] - '0') * sign;
		i++;
	}
	return (ret);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dst_size)
{
	size_t	src_len;
	size_t	dst_len;
	size_t	i;

	src_len = 0;
	dst_len = 0;
	i = 0;
	while (src[src_len])
		src_len++;
	while (dst[dst_len])
		dst_len++;
	if (dst_size < dst_len)
		return (dst_size + src_len);
	while (dst_size > dst_len + i + 1 && i < src_len)
	{
		dst[i + dst_len] = src[i];
		i++;
	}
	dst[i + dst_len] = '\0';
	return (dst_len + src_len);
}

char	*ft_itoa(int n)
{
	int		div;
	int		len;
	char	*ret;

	div = n;
	len = (n <= 0) ? 1 : 0;
	while (div != 0)
	{
		div = div / 10;
		len++;
	}
	if (!(ret = malloc(sizeof(char) * (len + 1))))
		return (0);
	ret[len] = '\0';
	if (n <= 0)
		ret[0] = (n == 0) ? '0' : '-';
	while (n != 0)
	{
		len--;
		ret[len] = (n > 0) ? (n % 10) + '0' : -(n % 10) + '0';
		n = n / 10;
	}
	return (ret);
}

typedef struct		s_info
{
	int		number_of_philosophers;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		number_of_times_each_philosopher_must_eat;
	struct timeval	birth;
}					t_info;

t_info	info;

int		set_info(char **argv)
{
	info.number_of_philosophers = ft_atoi(argv[1]);
	info.time_to_die = ft_atoi(argv[2]);
	info.time_to_eat = ft_atoi(argv[3]);
	info.time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		info.number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	return (0);
}

typedef struct	s_philo
{
	int		num;
	int		last_eat;
	int		eat_count;
	int		status;
	char	msg[100];
}				t_philo;

# define EATING 0
# define SLEEPING 1
# define THINKING 2
# define TAKE_FORK 3
# define DIED 4
# define WAIT_FORK 5

sem_t	*fork_sem;
sem_t	*wait_sem;
sem_t	*mutex;
int		num_available_forks;


int		get_timestamp_ms()
{
	int		ret;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	ret = (int)(tv.tv_sec - info.birth.tv_sec) * 1000 + (tv.tv_usec - info.birth.tv_usec) / 1000;
	return (ret);
}

void	set_status(t_philo *philo, int status)
{
	int		timestamp;
	char	*temp;

	philo->status = status;
	timestamp = get_timestamp_ms();
	
	// temp = ft_itoa(timestamp);
	// ft_strlcat(philo->msg, temp, 100);
	// free(temp);
	// ft_strlcat(philo->msg, " ", 100);
	// temp = ft_itoa(philo->num);
	// ft_strlcat(philo->msg, temp, 100);
	// free(temp);
	// if (philo->status == TAKE_FORK)
	// 	ft_strlcat(philo->msg, " has taken a fork\n", 100);
	// else if (philo->status == EATING)
	// 	ft_strlcat(philo->msg, " is eating\n", 100);
	// else if (philo->status == SLEEPING)
	// 	ft_strlcat(philo->msg, " is sleeping\n", 100);
	// else if (philo->status == THINKING)
	// 	ft_strlcat(philo->msg, " is thinking\n", 100);
	// else if (philo->status == DIED)
	// 	ft_strlcat(philo->msg, " died\n", 100);
	// ft_putstr(philo->msg);

	ft_putnbr(timestamp);
	ft_putstr(" ");
	ft_putnbr((long)philo->num);
	if (philo->status == TAKE_FORK)
		ft_putstr(" has taken a fork\n");
	else if (philo->status == EATING)
		ft_putstr(" is eating\n");
	else if (philo->status == SLEEPING)
		ft_putstr(" is sleeping\n");
	else if (philo->status == THINKING)
		ft_putstr(" is thinking\n");
	else if (philo->status == DIED)
		ft_putstr(" died\n");
	// philo->msg[0] = '\0';
}

int		am_i_dead(t_philo *philo)
{
	int		no_eat_duration;

	no_eat_duration = get_timestamp_ms() - philo->last_eat;
	if (no_eat_duration >= info.time_to_die)
	{
		sem_wait(mutex);
		set_status(philo, DIED);
		sem_post(mutex);
		return (1);
	}
	return (0);
}

int		dead_check_sleep(t_philo *philo, int sleep_time)
{
	int		sleep_start;

	sleep_start = get_timestamp_ms();
	while (get_timestamp_ms() - sleep_start < sleep_time)
	{
		if (am_i_dead(philo) == 1)
			return (1);
	}
	return (0);
}

typedef struct	s_thread_queue
{
	int							num;
	struct s_thread_queue		*prev;
	struct s_thread_queue		*next;
}				t_thread_queue;

t_thread_queue	*order;

void	del_queue(t_thread_queue *queue, int num)
{
	int		i;

	i = 0;
	while (i < info.number_of_philosophers)
	{
		if (queue->num == num)
		{
			(queue->prev)->next = queue->next;
			(queue->next)->prev = queue->prev;
			return ;
		}
		queue = queue->next;
		i++;
	}
}

void	*life(void *data)
{
	t_philo		*philo;

	philo = (t_philo *)data;
	philo->status = WAIT_FORK;
	if (philo->num % 2 == 0)
		usleep(100);
	while (1)
	{
		if (philo->status == WAIT_FORK)
		{
			sem_wait(wait_sem);
			if (num_available_forks >= 2)
			{
				num_available_forks -= 2;
				order = order->next;
				sem_post(wait_sem);
				sem_wait(fork_sem);
				sem_wait(fork_sem);
				sem_wait(mutex);
				set_status(philo, TAKE_FORK);
				sem_post(mutex);
			}
			else
				sem_post(wait_sem);
		}
		if (philo->status == TAKE_FORK)
		{
			sem_wait(mutex);
			set_status(philo, EATING);
			sem_post(mutex);
		}
		if (philo->status == EATING)
		{
			if (dead_check_sleep(philo, info.time_to_eat) == 1)
			{
				sem_post(fork_sem);
				sem_post(fork_sem);
				sem_wait(wait_sem);
				del_queue(order, philo->num);
				num_available_forks += 2;
				sem_post(wait_sem);
				return (0);
			}
			sem_post(fork_sem);
			sem_post(fork_sem);
			sem_wait(wait_sem);
			num_available_forks += 2;
			sem_post(wait_sem);
			sem_wait(mutex);
			philo->last_eat = get_timestamp_ms();
			set_status(philo, SLEEPING);
			sem_post(mutex);
		}
		if (philo->status == SLEEPING)
		{
			if (dead_check_sleep(philo, info.time_to_sleep) == 1)
			{
				sem_wait(wait_sem);
				del_queue(order, philo->num);
				sem_post(wait_sem);
				return (0);
			}
			sem_wait(mutex);
			set_status(philo, THINKING);
			sem_post(mutex);
		}
		if (philo->status == THINKING)
		{
			philo->status = WAIT_FORK;
		}
		if (am_i_dead(philo) == 1)
		{
			sem_wait(wait_sem);
			del_queue(order, philo->num);
			sem_post(wait_sem);
			return (0);
		}
	}
	return (0);
}

int				set_order(t_thread_queue *ret, int num)
{
	int		i;

	i = 0;
	if (num == 1)
	{
		ret[i].num = 1;
		ret[i].prev = ret;
		ret[i].next = ret;
		return (0);
	}
	while (i < num)
	{
		ret[i].num = i + 1;
		if ((num % 2 == 0 && i == num - 1))
			ret[i].next = &(ret[0]);
		else if (num % 2 == 0 && i == num - 2)
			ret[i].next = &(ret[1]);
		else if (num % 2 == 1 && i == num - 1)
			ret[i].next = &(ret[1]);
		else if (num % 2 == 1 && i == num - 2)
			ret[i].next = &(ret[0]);
		else
			ret[i].next = &(ret[i + 2]);
		i++;
	}
	t_thread_queue	*temp;
	t_thread_queue	*cur;
	i = 0;
	cur = ret;
	while (i < num)
	{
		temp = cur;
		cur = cur->next;
		cur->prev = temp;
		i++;
	}
	return (0);
}

t_thread_queue	*make_order(int num)
{
	t_thread_queue	*ret;

	if (!(ret = malloc(sizeof(t_thread_queue) * num)))
		return (0);
	set_order(ret, num);
	return (ret);
}

int		philo_two(char **argv)
{
	int		i;
	pthread_t	*threads;
	t_thread_queue	*temp;
	t_philo		*philos;

	set_info(argv);
	temp = make_order(info.number_of_philosophers);
	order = temp;
	num_available_forks = info.number_of_philosophers;

	if (!(threads = malloc(sizeof(pthread_t) * info.number_of_philosophers)))
	{
		free(order);
		return (1);
	}
	if (!(philos = malloc(sizeof(t_philo) * info.number_of_philosophers)))
	{
		free(order);
		free(threads);
		return (1);
	}
	memset((void *)philos, 0, sizeof(t_philo) * info.number_of_philosophers);
	i = 0;
	while (i < info.number_of_philosophers)
	{
		philos[i].num = i + 1;
		i++;
	}
	fork_sem = sem_open("forks", O_CREAT | O_EXCL, 0644, info.number_of_philosophers);
	wait_sem = sem_open("wait", O_CREAT | O_EXCL, 0644, 1);
	mutex = sem_open("mutex", O_CREAT | O_EXCL, 0644, 1);

	sem_unlink("forks");
	sem_unlink("wait");
	sem_unlink("mutex");

	gettimeofday(&(info.birth), NULL);
	printf("thread created\n");
	i = 0;
	while (i < info.number_of_philosophers)
	{
		pthread_create(&threads[i], NULL, life, (void *)&philos[i]);
		i++;
	}
	i = 0;
	while (i < info.number_of_philosophers)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	printf("thread end\n");
	
	sem_close(fork_sem);
	sem_close(wait_sem);
	sem_close(mutex);
	free(philos);
	free(threads);
	free(temp);
	return (0);
}

int		main(int argc, char **argv)
{
	if (argc == 5 || argc == 6)
	{
		if (philo_two(argv) != 0)
		{
			ft_putstr("philo_two error\n");
			return (1);
		}
		return (0);
	}
	ft_putstr("argument error\n");
	return (1);
}