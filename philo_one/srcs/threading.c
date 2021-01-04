/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hna <hna@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 09:15:58 by hna               #+#    #+#             */
/*   Updated: 2021/01/04 09:16:03 by hna              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

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
			if (philos[i].status == DIED || philos[i].eat_count >= g_info.must_eat)
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

t_philo		*make_philos(int nb_philos)
{
	int			i;
	t_philo		*philos;

	i = 0;
	if (!(philos = malloc(sizeof(t_philo) * nb_philos)))
		return (0);
	memset((void *)philos, 0, sizeof(t_philo) * nb_philos);
	while (i < nb_philos)
	{
		philos[i].num = i + 1;
		i++;
	}
	return (philos);
}

int		set_pthreads(pthread_t *threads, t_philo *philos)
{
	int			i;
	pthread_t	trd;

	gettimeofday(&(g_info.birth), NULL);
	if (g_info.must_eat != -1)
	{
		pthread_create(&trd, NULL, check_finish_option, (void *)philos);
		pthread_detach(trd);
	}
	i = -1;
	while (++i < g_info.number_of_philosophers)
		pthread_create(&threads[i], NULL, life, (void *)&philos[i]);
	i = -1;
	while (++i < g_info.number_of_philosophers)
		pthread_join(threads[i], NULL);
	return (0);
}

int		make_forks(t_philo *philos, int n)
{
	int		i;

	i = 0;
	while (i < n)
	{
		philos[i].num = i + 1;
		if (!(philos[i].right_fork = malloc(sizeof(t_fork) * 1)))
		{
			while (--i >= 0)
				free(philos[i].right_fork);
			return (1);
		}
		philos[i].right_fork->status = 1;
		i++;
	}
	i--;
	philos[i].left_fork = philos[0].right_fork;
	while (--i >= 0)
		philos[i].left_fork = philos[i + 1].right_fork;
	return (0);
}

void	clear_all(t_philo *philos, pthread_t *threads)
{
	int		i;

	i = 0;
	while (i < g_info.number_of_philosophers)
	{
		free(philos[i].left_fork);
		i++;
	}
	free(threads);
	free(philos);
}

int		threading(void)
{
	pthread_t	*threads;
	t_philo		*philos;

	if (!(philos = make_philos(g_info.number_of_philosophers)))
		return (0);
	if (!(threads = malloc(sizeof(pthread_t) * g_info.number_of_philosophers)))
	{
		free(philos);
		return (0);
	}
	if (make_forks(philos, g_info.number_of_philosophers) != 0)
	{
		free(philos);
		free(threads);
		return (0);
	}
	set_pthreads(threads, philos);
	clear_all(philos, threads);
	return (1);
}
