/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hna <hna@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 16:16:40 by hna               #+#    #+#             */
/*   Updated: 2021/01/05 16:16:57 by hna              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int		wait_fork(t_philo *philo)
{
	if (g_info.number_of_philosophers == 1)
	{
		usleep(g_info.time_to_die);
		return (0);
	}
	if (am_i_dead(philo) == 1)
		return (1);
	sem_wait(g_fork_sem);
	sem_wait(g_sp_print);
	set_status(philo, TAKE_FORK);
	sem_post(g_sp_print);
	sem_wait(g_fork_sem);
	sem_wait(g_sp_print);
	set_status(philo, TAKE_FORK);
	sem_post(g_sp_print);
	return (0);
}

int		take_fork(t_philo *philo)
{
	sem_wait(g_sp_print);
	set_status(philo, EATING);
	sem_post(g_sp_print);
	return (0);
}

int		eating(t_philo *philo)
{
	if (dead_check_sleep(philo, g_info.time_to_eat) == 1)
	{
		sem_post(g_fork_sem);
		sem_post(g_fork_sem);
		return (1);
	}
	sem_post(g_fork_sem);
	sem_post(g_fork_sem);
	sem_wait(g_sp_print);
	philo->last_eat = get_timestamp_ms();
	set_status(philo, SLEEPING);
	sem_post(g_sp_print);
	philo->eat_count += 1;
	return (0);
}

int		sleeping(t_philo *philo)
{
	if (dead_check_sleep(philo, g_info.time_to_sleep) == 1)
		return (1);
	sem_wait(g_sp_print);
	set_status(philo, THINKING);
	sem_post(g_sp_print);
	return (0);
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
		if (g_info.must_eat != -1 && philo->eat_count >= g_info.must_eat)
			return (0);
		if (philo->status == WAIT_FORK && wait_fork(philo))
			return (0);
		if (philo->status == TAKE_FORK && take_fork(philo))
			return (0);
		if (philo->status == EATING && eating(philo))
			return (0);
		if (philo->status == SLEEPING && sleeping(philo))
			return (0);
		if (philo->status == THINKING)
			philo->status = WAIT_FORK;
		if (am_i_dead(philo) == 1)
			return (0);
	}
	return (0);
}
