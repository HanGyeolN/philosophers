#include "philo_two.h"

void    wait_fork(t_philo *philo)
{
    sem_wait(sp_nfork);
    if (num_available_forks >= 2)
    {
        num_available_forks -= 2;
        sem_post(sp_nfork);
        sem_wait(fork_sem);
        sem_wait(sp_print);
        set_status(philo, TAKE_FORK);
        sem_post(sp_print);
        sem_wait(fork_sem);
        sem_wait(sp_print);
        set_status(philo, TAKE_FORK);
        sem_post(sp_print);
    }
    else
        sem_post(sp_nfork);
}

int    take_fork(t_philo *philo)
{
    if (g_finish == 1)
        return (1);
    sem_wait(sp_print);
    set_status(philo, EATING);
    sem_post(sp_print);
    return (0);
}

int     eating(t_philo *philo)
{
    if (dead_check_sleep(philo, info.time_to_eat) == 1)
    {
        sem_post(fork_sem);
        sem_post(fork_sem);
        sem_wait(sp_nfork);
        num_available_forks += 2;
        sem_post(sp_nfork);
        return (1);
    }
    sem_post(fork_sem);
    sem_post(fork_sem);
    sem_wait(sp_nfork);
    num_available_forks += 2;
    sem_post(sp_nfork);
    sem_wait(sp_print);
    philo->last_eat = get_timestamp_ms();
    set_status(philo, SLEEPING);
    sem_post(sp_print);
    philo->eat_count += 1;
    return (0);
}

int     sleeping(t_philo *philo)
{
    if (dead_check_sleep(philo, info.time_to_sleep) == 1 || g_finish == 1)
        return (1);
    sem_wait(sp_print);
    set_status(philo, THINKING);
    sem_post(sp_print);
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