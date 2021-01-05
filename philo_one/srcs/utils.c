/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hna <hna@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 09:15:58 by hna               #+#    #+#             */
/*   Updated: 2021/01/06 03:15:45 by hna              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int			make_order(void)
{
	int		i;
	int		idx;

	if (!(g_order = malloc(sizeof(int) * g_info.number_of_philosophers)))
		return (0);
	g_now = 0;
	idx = 0;
	i = 1;
	while (i <= g_info.number_of_philosophers)
	{
		g_order[idx] = i;
		i += 2;
		idx++;
	}
	i = 2;
	while (i <= g_info.number_of_philosophers)
	{
		g_order[idx] = i;
		i += 2;
		idx++;
	}
	return (1);
}
