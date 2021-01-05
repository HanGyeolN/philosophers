#include "philo_two.h"

int		set_semaphores(void)
{
	fork_sem = sem_open("forks", O_CREAT | O_EXCL, 0644, g_info.number_of_philosophers);
	g_sp_nfork = sem_open("g_sp_nfork", O_CREAT | O_EXCL, 0644, 1);
	g_sp_print = sem_open("g_sp_print", O_CREAT | O_EXCL, 0644, 1);
	sem_unlink("forks");
	sem_unlink("g_sp_nfork");
	sem_unlink("g_sp_print");
	return (0);
}

int		close_semaphores(void)
{
	sem_close(fork_sem);
	sem_close(g_sp_nfork);
	sem_close(g_sp_print);
	return (0);
}

int		init_options(char **argv)
{
	g_info.number_of_philosophers = ft_atoi(argv[1]);
	g_info.time_to_die = ft_atoi(argv[2]);
	g_info.time_to_eat = ft_atoi(argv[3]);
	g_info.time_to_sleep = ft_atoi(argv[4]);
	g_info.must_eat = -1;
	if (argv[5])
		g_info.must_eat = ft_atoi(argv[5]);
	g_num_available_forks = g_info.number_of_philosophers;
	g_finish = 0;
	return (0);
}

int		philo_two(char **argv)
{
	if (init_options(argv) != 0)
		return (1);
	if (set_semaphores() != 0)
		return (1);
	if (!(threading()))
		return (1);
	if (close_semaphores() != 0)
		return (1);
	return (0);
}

int		main(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
	{
		ft_putstr("argument error\n");
		return (1);
	}
	if (philo_two(argv) != 0)
	{
		ft_putstr("philo_two error\n");
		return (1);
	}
	return (0);
}