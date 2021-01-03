/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hna <hna@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 08:47:22 by hna               #+#    #+#             */
/*   Updated: 2021/01/04 08:52:32 by hna              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>

# define EATING 0
# define SLEEPING 1
# define THINKING 2
# define TAKE_FORK 3
# define DIED 4
# define WAIT_FORK 5

typedef struct	s_info
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	struct timeval	birth;
}				t_info;

typedef struct	s_fork
{
	int		status;
}				t_fork;

typedef struct	s_philo
{
	t_fork	*left_fork;
	t_fork	*right_fork;
	int		num;
	int		last_eat;
	int		eat_count;
	int		status;
}				t_philo;

pthread_mutex_t	g_print_lock;
pthread_mutex_t	g_fork_lock;
int				g_finish;
t_info			g_info;

void			ft_putchar(char c);
void			ft_putnbr(long nb);
int				ft_strlen(char *str);
void			ft_putstr(char *str);
int				ft_atoi(const char *s);

int				threading(void);
void			*life(void *data);
void			*check_finish_option(void *data);

int				get_timestamp_ms(void);
void			set_status(t_philo *philo, int status);
int				am_i_dead(t_philo *philo);
int				dead_check_sleep(t_philo *philo, int sleep_time);

#endif
