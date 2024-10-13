#include "philo.h"

void sleep_philo(int time_to_sleep);

int     _thinking(t_philo *philo)
{
    pthread_mutex_lock(&(philo->info->dead_lock));
    if(philo->info->dead_philo == 1)
    {
        pthread_mutex_unlock(&(philo->info->dead_lock));
        return (0);
    }
    else
    {
        pthread_mutex_unlock(&(philo->info->dead_lock));
        pthread_mutex_lock(&(philo->info->prt_lock));
        printf(BLUE"%ld %d is thinking\n"NC, (get_current_time_ms() - philo->info->t_start), philo->id);
        pthread_mutex_unlock(&(philo->info->prt_lock));
        return (1);
    }
}

int _forks(t_philo *philo)
{
    pthread_mutex_lock(&(philo->info->dead_lock));
    if (philo->info->dead_philo == 1)
    {
        pthread_mutex_unlock(&(philo->info->dead_lock));
        return 0;
    }
    pthread_mutex_unlock(&(philo->info->dead_lock));

    // Avoid deadlock by changing the order for even and odd philosophers.
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->right_fork);
        pthread_mutex_lock(&(philo->info->prt_lock));
        printf(BLUE"%ld %d has taken a fork\n"NC, get_current_time_ms() - philo->info->t_start, philo->id);
        pthread_mutex_unlock(&(philo->info->prt_lock));

        pthread_mutex_lock(philo->left_fork);
    }
    else
    {
        pthread_mutex_lock(philo->left_fork);
        pthread_mutex_lock(&(philo->info->prt_lock));
        printf(BLUE"%ld %d has taken a fork\n"NC, get_current_time_ms() - philo->info->t_start, philo->id);
        pthread_mutex_unlock(&(philo->info->prt_lock));

        pthread_mutex_lock(philo->right_fork);
    }
    return 1;
}

int _eating(t_philo *philo)
{
    pthread_mutex_lock(&(philo->info->dead_lock));
    if (philo->info->dead_philo == 1)
    {
        pthread_mutex_unlock(&(philo->info->dead_lock));
        return 0;
    }
    pthread_mutex_unlock(&(philo->info->dead_lock));

    pthread_mutex_lock(&(philo->info->prt_lock));
    printf(BLUE"%ld %d is eating\n"NC, get_current_time_ms() - philo->info->t_start, philo->id);
    pthread_mutex_unlock(&(philo->info->prt_lock));

    pthread_mutex_lock(&(philo->info->lst_meal_lock));
    philo->info->T_last_meal = get_current_time_ms();
    pthread_mutex_unlock(&(philo->info->lst_meal_lock));

    pthread_mutex_lock(&(philo->info->lock_meal));
    philo->info->meal_eaten++;
    pthread_mutex_unlock(&(philo->info->lock_meal));

    sleep_philo(philo->info->t_to_eat);
    return 1;
}

int     unlocking_forks(t_philo *philo)
{
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
    return (1);
}

int     _sleeping(t_philo *philo)
{
    pthread_mutex_lock(&(philo->info->dead_lock));
    if(philo->info->dead_philo == 1)
    {
        pthread_mutex_unlock(&(philo->info->dead_lock));
        return (0);
    }
    else
    {
        pthread_mutex_unlock(&(philo->info->dead_lock));
        pthread_mutex_lock(&(philo->info->prt_lock));
        printf(BLUE"%ld %d is sleeping\n"NC, get_current_time_ms() - philo->info->t_start, philo->id);
        pthread_mutex_unlock(&(philo->info->prt_lock));
        sleep_philo(philo->info->t_to_sleep);
        return (1);
    }
}

int     _routine(t_philo *philo)
{
    if(_thinking(philo) == 0)
        return (0);
    if(_forks(philo) == 0)
        return (0);
    if(_eating(philo) == 0)
        return (0);
    if(unlocking_forks(philo) == 0)
        return (0);
    if(_sleeping(philo) == 0)
        return (0);
    return (1);
}

void    *routine_Multi_thread(void *arg)
{
    t_philo     *philo;

    philo = (t_philo *)arg;
    while(1)
    {
        pthread_mutex_lock(&(philo->info->dead_lock));
        if(philo->info->dead_philo == 1)
        {
            pthread_mutex_unlock(&(philo->info->dead_lock));
            return (NULL);
        }
        else
        {
            pthread_mutex_unlock(&(philo->info->dead_lock));
            if(_routine(philo) == 0)
                return (NULL);
        }
    }
    printf("ana hahna\n");
    return (NULL);
}