/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hna <hna@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 16:02:18 by hna               #+#    #+#             */
/*   Updated: 2021/01/06 03:26:55 by hna              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>

typedef struct	s_info
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	struct timeval	birth;
}				t_info;

typedef struct	s_philo
{
	int		num;
	int		last_eat;
	int		eat_count;
	int		status;
}				t_philo;

# define EATING 0
# define SLEEPING 1
# define THINKING 2
# define TAKE_FORK 3
# define DIED 4
# define WAIT_FORK 5

sem_t			*g_fork_sem;
sem_t			*g_sp_nfork;
sem_t			*g_sp_print;
int				g_num_available_forks;
int				g_finish;
t_info			g_info;
int				*g_order;
int				g_now;

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
