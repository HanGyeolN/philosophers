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

int		processing(void)
{
	pid_t		*pids;
	t_philo		*philos;

	if (!(philos = make_philos(g_info.number_of_philosophers)))
		return (0);
	if (!(pids = malloc(sizeof(pid_t) * g_info.number_of_philosophers)))
		return (0);
	int			i;
	i = 0;
	gettimeofday(&(g_info.birth), NULL);
	while (i < g_info.number_of_philosophers)
	{
		pids[i] = fork();
		if (pids[i] == 0)
		{
			life(&philos[i]);
			exit(0);
		}
		i++;
	}
	i = 0;
	int		ret;
	while (i < g_info.number_of_philosophers)
	{
		waitpid(pids[i], &ret, 0);
		i++;
	}
	free(philos);
	free(pids);
	return (1);
}