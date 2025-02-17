/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hna <hna@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 16:15:06 by hna               #+#    #+#             */
/*   Updated: 2021/01/05 16:15:54 by hna              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

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
		sem_wait(g_sp_print);
		set_status(philo, DIED);
		sem_post(g_sp_print);
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
