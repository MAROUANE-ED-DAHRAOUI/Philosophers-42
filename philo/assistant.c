/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assistant.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:06:29 by med-dahr          #+#    #+#             */
/*   Updated: 2024/10/18 13:46:15 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* ---->  Function to get the current time in milliseconds. 
        This is used for time tracking in the simulation.
*/
long get_current_time_ms(void)
{
    struct timeval  tm;

    gettimeofday(&tm, NULL);
    return (tm.tv_sec * 1000 + tm.tv_usec / 1000);
}

// int     ft_meals(t_philo *philo)
// {
//     int i;
//     int cnt;

//     i = 0;
//     cnt = 0;
//     while(i < philo->info->num_of_philo)
//     {
//         pthread_mutex_lock(&philo->info->lock_meal);
//         if(philo->info->philos[i].info->num_meal >= philo->info->limit_meals)
//                cnt++;
//         pthread_mutex_unlock(&philo->info->lock_meal);
//         i++;
//     }
//     if(cnt == philo->info->num_of_philo)
//     {
//         pthread_mutex_lock(&philo->info->dead_lock);
//         philo->info->dead_philo = 1;
//         // pthread_mutex_unlock(&(philo->info->dead_lock));
//         return (0);
//     }
//     return (1);
// }

// int    philos_infinite_loop(t_philo *philo)
// {
//     int i;

//     i = 0;
//     while(1)
//     {
//         if(Is_dead(&(philo)->info->philos[i]) == 0)
//             break ;
//         if(philo->info->limit_meals != -1 && philo->info->cnt_meals == 0)
//         {
//             if(ft_meals(philo) == 0)
//                 break ;
//         }
//         i++;
//         if(i == philo->info->num_of_philo - 1)
//             i = 0;
//     }
//     return (1);
// }

int initialize_philos(t_philo **philo)
{
    int i;

    i = 0;
    (*philo)->info->philos[i].info->t_start = get_current_time_ms();
    while (i < (*philo)->info->num_of_philo)
    {
        (*philo)->info->philos[i].info = (*philo)->info;
        (*philo)->info->philos[i].id = i + 1;
        (*philo)->info->philos[i].info->t_to_die = (*philo)->info->t_to_die;
        (*philo)->info->philos[i].info->t_to_eat = (*philo)->info->t_to_eat;
        (*philo)->info->philos[i].info->t_to_sleep = (*philo)->info->t_to_sleep;
        (*philo)->info->philos[i].info->num_meal = 0;
        (*philo)->info->philos[i].info->last_meal = get_current_time_ms();
        (*philo)->info->philos[i].left_fork = &(*philo)->info->forks[i];
        (*philo)->info->philos[i].right_fork = &(*philo)->info->forks[(i + 1) % (*philo)->info->num_of_philo];
        pthread_mutex_init(&(*philo)->info->philos[i].lock_meal, NULL);
        pthread_mutex_init(&(*philo)->info->philos[i].mutex_time, NULL);
        pthread_create(&(*philo)->info->philos[i].threads, NULL, &routine_Multi_thread, &(*philo)->info->philos[i]);
        i++;
    }
    return (1);
}

int     Num_philos(t_philo *philo)
{
    if(philo->info->num_of_philo == 1)
    {
        printf("1 1 thinking\n");
        printf("1 1 has taken a fork\n");
        sleep_philo(philo->info->t_to_die);
        printf("1 1 is dead\n");
        return (0);
    }
    return 1;
}

void looking_mutex(t_philo *philo)
{
    pthread_mutex_lock(&philo->lock_meal);
    pthread_mutex_unlock(&philo->mutex_time);
    pthread_mutex_unlock(&philo->info->dead_lock);
}

void unlocking_mutex(t_philo *philo)
{
    pthread_mutex_unlock(&philo->lock_meal);
    pthread_mutex_unlock(&philo->mutex_time);
    pthread_mutex_unlock(&philo->info->dead_lock);
}

int state_philos(t_philo *philo)
{
   int  _time;

    looking_mutex(philo);
   _time = (get_current_time_ms() - philo->info->last_meal);
    while(_time > philo->info->t_to_die)
    {
        pthread_mutex_lock(&philo->info->prt_lock);
        return 1;
    }
    unlocking_mutex(philo);
    return (1);
}

int monitor_state_philo(t_philo *philo)
{
    int i;
    int max;

    i = 0;
    while(1)
    {
        max = INT_MIN;
        while(i < philo->info->num_of_philo)
        {
            if(state_philos(&philo->info->philos[i]) == 0)
                return 0;
            if(philo->limit_meals != -1 && philo->info->num_meal < max)
                max = philo->info->num_meal;
            pthread_mutex_unlock(&philo->info->philo_dead);
            pthread_mutex_unlock(&philo[i].mutex_time);
            pthread_mutex_unlock(&philo[i].lock_meal);
        }
        if (philo->limit_meals != -1 && philo->limit_meals <= max)
        {
            philo->info->_exit = true;
            return 2;
        }
        usleep(700);
    }
    return (1);
}

int Lets_Go_Threads(t_philo *philo)
{
    int i;
   
    if(Num_philos(philo) == 0)
        return 0;
    i = 0;
    if(monitor_state_philo(philo))
        printf("%ld %d %s\n", (get_current_time_ms() - philo->info->t_start), philo->id, "is dead");
    pthread_mutex_lock(&philo->info->philo_dead);
    philo->info->_exit = true;
    pthread_mutex_unlock(&philo->info->philo_dead);
    while(i < philo->info->num_of_philo)
    {
        pthread_join(philo->info->philos[i].threads, NULL);
        i++;
    }
    ft_free(philo);
    return 1;
}

