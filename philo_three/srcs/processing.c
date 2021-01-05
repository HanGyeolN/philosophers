/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hna <hna@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/05 16:15:15 by hna               #+#    #+#             */
/*   Updated: 2021/01/05 16:15:16 by hna              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

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

int			wait_child_process(pid_t *pids)
{
	int		i;
	int		ret;

	i = 0;
	while (i < g_info.number_of_philosophers)
	{
		waitpid(pids[i], &ret, 0);
		i++;
	}
	return (0);
}

int			processing(void)
{
	int			i;
	pid_t		*pids;
	t_philo		*philos;

	if (!(philos = make_philos(g_info.number_of_philosophers)))
		return (0);
	if (!(pids = malloc(sizeof(pid_t) * g_info.number_of_philosophers)))
		return (0);
	i = -1;
	gettimeofday(&(g_info.birth), NULL);
	while (++i < g_info.number_of_philosophers)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			life(&philos[i]);
			exit(0);
		}
	}
	wait_child_process(pids);
	free(philos);
	free(pids);
	return (1);
}
