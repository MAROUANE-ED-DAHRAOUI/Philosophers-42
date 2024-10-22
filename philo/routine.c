#include "philo.h"

void sleep_philo(int time_to_sleep);

void     _thinking(t_philo *philo)
{
    if(Is_dead(philo) == 0)
    {
        philo->info->_exit = false;
        return ;
    }
    print_moves(philo, "Is thinking");
    // sleep_philo(philo->info->t_to_sleep);
}

void    print_moves(t_philo *philo, char *str)
{
    pthread_mutex_lock(&(philo->info->prt_lock));
    if(Is_dead(philo) == 0)
    {
        philo->info->_exit = false;
        pthread_mutex_unlock(&(philo->info->prt_lock));
        return ;
    }
    printf(BLUE"%ld %d %s\n"NC, (get_current_time_ms() - philo->t_start), philo->id, str);
    pthread_mutex_unlock(&(philo->info->prt_lock));
}

int     One_thread(t_philo *philo)
{
    if(philo->left_fork == philo->right_fork)
    {
        pthread_mutex_lock((philo->left_fork));
        print_moves(philo, "has taken a fork");
        philo->info->_exit = false;
        pthread_mutex_unlock((philo->left_fork));
        return 0;
    }
    return 1;
}

void _eating(t_philo *philo)
{
    if(Is_dead(philo) == 0 || One_thread(philo) == 0)
    {
        philo->info->_exit = false;
        return ;
    }
    pthread_mutex_lock(philo->left_fork);
    print_moves(philo, "has taken a fork");
    pthread_mutex_lock((philo->right_fork));
    print_moves(philo, "has taken a fork");
    print_moves(philo, "Is_eating");
    pthread_mutex_lock(&(philo->mutex_time));
    philo->last_meal = get_current_time_ms();
    pthread_mutex_unlock(&(philo->mutex_time));
    sleep_philo(philo->info->t_to_eat);
    pthread_mutex_lock(&philo->lock_meal);
    philo->num_meal+=1;
    pthread_mutex_unlock(&philo->lock_meal);
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
}

int     unlocking_forks(t_philo *philo)
{
    pthread_mutex_unlock(philo->left_fork);
    pthread_mutex_unlock(philo->right_fork);
    return (1);
}

void     _sleeping(t_philo *philo)
{
    if(Is_dead(philo) == 0)
    {
        philo->info->_exit = false;
        return ;
    }
    print_moves(philo, "Is sleeping");
    sleep_philo(philo->info->t_to_sleep);
}

// int     _routine(t_philo *philo)
// {
//     while(1)
//     {
//         if(Is_dead(philo) == 0)
//             return (0);
//         if(_thinking(philo) == 0)
//             return (0);
//         if(_eating(philo) == 0)
//             return (0);
//         if(_sleeping(philo) == 0)
//             return (0);
//     }
//     return (1);
// }

void    *routine_Multi_thread(void *arg)
{
    t_philo     *philo;
    t_info      *info;

    philo = (t_philo *)arg;
    info = (t_info *)philo->info;
    if(philo->id % 2 != 0 && info->num_of_philo != 1)
        _sleeping(philo);
    while(1)
    {
        if(Is_dead(philo) == 0)
        {
            philo->info->_exit = false;
            break ;
        }
        _thinking(philo);
        _eating(philo);
        _sleeping(philo);
    }
   return (NULL);
}