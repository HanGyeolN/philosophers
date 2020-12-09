#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

# define EATING 0
# define SLEEPING 1
# define THINKING 2
# define TAKE_FORK 3
# define DIED 4

typedef struct	s_info
{
	int		number_of_philosophers;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		number_of_times_each_philosopher_must_eat;
}				t_info;

typedef struct	s_fork
{
	int		status;
}				t_fork;

typedef struct	s_philosopher
{
	t_fork				*left_fork;
	t_fork				*right_fork;
	int					num;
	struct timeval		birth;
	long int			last_eat;
	int					eat_count;
	int					status;
}				t_philosopher;

void	ft_putstr(char *str);
int		ft_strlen(char *str);
int		ft_atoi(const char *s);
char	*ft_itoa(int n);
char	*ft_strjoin(char *s1, char *s2);

#endif