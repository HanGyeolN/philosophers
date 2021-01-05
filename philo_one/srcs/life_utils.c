/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hna <hna@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 15:55:59 by hna               #+#    #+#             */
/*   Updated: 2021/01/05 15:56:12 by hna              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int		get_timestamp_ms(void)
{
	int				ret;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	ret = (int)(tv.tv_sec - g_info.birth.tv_sec) * 1000 + \
			(tv.tv_usec - g_info.birth.tv_usec) / 1000;
	return (ret);
}

void	set_status(t_philo *philo, int status)
{
	int		timestamp;

	philo->status = status;
	timestamp = get_timestamp_ms();
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
}

int		am_i_dead(t_philo *philo)
{
	int		no_eat_duration;

	no_eat_duration = get_timestamp_ms() - philo->last_eat;
	if (no_eat_duration >= g_info.time_to_die)
	{
		pthread_mutex_lock(&g_print_lock);
		set_status(philo, DIED);
		pthread_mutex_unlock(&g_print_lock);
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

void	*check_finish_option(void *data)
{
	int			i;
	int			sum;
	t_philo		*philos;

	philos = (t_philo *)data;
	while (1)
	{
		i = 0;
		sum = 0;
		while (i < g_info.number_of_philosophers)
		{
			if (philos[i].status == DIED || \
				philos[i].eat_count >= g_info.must_eat)
				sum += 1;
			i++;
		}
		if (sum == g_info.number_of_philosophers)
		{
			g_finish = 1;
			return (0);
		}
		usleep(100000);
	}
}
