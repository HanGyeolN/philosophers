/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hna <hna@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 08:45:15 by hna               #+#    #+#             */
/*   Updated: 2021/01/04 08:45:17 by hna              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int		set_mutex(void)
{
	pthread_mutex_init(&print_lock, NULL);
	pthread_mutex_init(&fork_lock, NULL);
	return (0);
}

int		close_mutex(void)
{
	pthread_mutex_destroy(&print_lock);
	pthread_mutex_destroy(&fork_lock);
	return (0);
}

int		init_options(char **argv)
{
	info.number_of_philosophers = ft_atoi(argv[1]);
	info.time_to_die = ft_atoi(argv[2]);
	info.time_to_eat = ft_atoi(argv[3]);
	info.time_to_sleep = ft_atoi(argv[4]);
	info.must_eat = -1;
	if (argv[5])
		info.must_eat = ft_atoi(argv[5]);
	g_finish = 0;
	return (0);
}

int		philo_one(char **argv)
{
	init_options(argv);
	set_mutex();
	if (!(threading()))
	{
		close_mutex();
		return (1);
	}
	close_mutex();
	return (0);
}

int		main(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		ft_putstr("argument error\n");
		return (1);
	}
	if (philo_one(argv) != 0)
	{
		ft_putstr("philo_one error\n");
		return (1);
	}
	return (0);
}
