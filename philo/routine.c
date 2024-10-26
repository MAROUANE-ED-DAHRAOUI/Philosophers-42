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

void print_moves(t_philo *philo, char *str)
{
    pthread_mutex_lock(&(philo->info->prt_lock));
    if (Is_dead(philo) == 0)
    {
        philo->info->_exit = false;
        pthread_mutex_unlock(&(philo->info->prt_lock));
        return ;
    }
    // printf("philo time %lld\n%lld\n", get_current_time_ms(), philo->t_start);
    printf(BLUE "%lld %d %s\n" NC, 
           get_current_time_ms() - philo->t_start, 
           philo->id, str);
    pthread_mutex_unlock(&(philo->info->prt_lock));  // Unlock after printing
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
    if (Is_dead(philo) == 0 || One_thread(philo) == 0)
        return;

    pthread_mutex_lock(philo->left_fork);  // Lock left fork
    print_moves(philo, "has taken a fork");
    pthread_mutex_lock(philo->right_fork);  // Lock right fork
    print_moves(philo, "has taken a fork");
    print_moves(philo, "is eating"); 

    pthread_mutex_lock(&philo->mutex_time);
    philo->last_meal = get_current_time_ms();
    pthread_mutex_unlock(&philo->mutex_time);

    pthread_mutex_lock(&philo->lock_meal);  // Lock before modifying num_meal
    philo->num_meal++;  // Increment meal count safely
    pthread_mutex_unlock(&philo->lock_meal);  // Unlock after modifying num_meal

    sleep_philo(philo->info->t_to_eat);

    pthread_mutex_unlock(philo->left_fork);  // Release forks
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