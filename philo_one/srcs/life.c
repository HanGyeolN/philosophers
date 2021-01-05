/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hna <hna@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 09:18:48 by hna               #+#    #+#             */
/*   Updated: 2021/01/06 03:21:36 by hna              ###   ########seoul.kr  */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	wait_fork(t_philo *philo)
{
	pthread_mutex_lock(&g_fork_lock);
	if (g_order[g_now] == 0)
	{
		g_now++;
		if (g_now == g_info.number_of_philosophers)
			g_now = 0;
		pthread_mutex_unlock(&g_fork_lock);
	}
	else if (philo->left_fork->status == 1 && philo->right_fork->status == 1 \
		&& g_order[g_now] == philo->num)
	{
		philo->left_fork->status = 0;
		philo->right_fork->status = 0;
		g_now++;
		if (g_now == g_info.number_of_philosophers)
			g_now = 0;
		pthread_mutex_lock(&g_print_lock);
		set_status(philo, TAKE_FORK);
		set_status(philo, TAKE_FORK);
		pthread_mutex_unlock(&g_print_lock);
		pthread_mutex_unlock(&g_fork_lock);
	}
	else
		pthread_mutex_unlock(&g_fork_lock);
}

int		take_fork(t_philo *philo)
{
	if (g_finish == 1)
		return (1);
	pthread_mutex_lock(&g_print_lock);
	set_status(philo, EATING);
	pthread_mutex_unlock(&g_print_lock);
	return (0);
}

int		eating(t_philo *philo)
{
	if (dead_check_sleep(philo, g_info.time_to_eat) == 1)
	{
		pthread_mutex_lock(&g_fork_lock);
		philo->left_fork->status = 1;
		philo->right_fork->status = 1;
		pthread_mutex_unlock(&g_fork_lock);
		return (1);
	}
	pthread_mutex_lock(&g_fork_lock);
	philo->left_fork->status = 1;
	philo->right_fork->status = 1;
	pthread_mutex_unlock(&g_fork_lock);
	pthread_mutex_lock(&g_print_lock);
	philo->last_eat = get_timestamp_ms();
	set_status(philo, SLEEPING);
	pthread_mutex_unlock(&g_print_lock);
	philo->eat_count += 1;
	return (0);
}

int		sleeping(t_philo *philo)
{
	if (dead_check_sleep(philo, g_info.time_to_sleep) == 1 || g_finish == 1)
		return (1);
	pthread_mutex_lock(&g_print_lock);
	set_status(philo, THINKING);
	pthread_mutex_unlock(&g_print_lock);
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
		if (g_finish == 1)
			return (0);
		if (philo->status == WAIT_FORK)
			wait_fork(philo);
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
