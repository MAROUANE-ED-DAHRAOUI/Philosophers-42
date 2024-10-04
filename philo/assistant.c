/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assistant.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:06:29 by med-dahr          #+#    #+#             */
/*   Updated: 2024/09/28 19:55:46 by med-dahr         ###   ########.fr       */
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

int     ft_meals(t_philo *philo)
{
    int i;
    int cnt;

    i = 0;
    cnt = 0;
    while(i < philo->info->num_of_philo)
    {
        pthread_mutex_lock(&(philo)->info->lock_meal);
        if(philo->info->philos[i].info->meal_eaten >= philo->info->round_meals)
               cnt++;
        pthread_mutex_unlock(&(philo)->info->lock_meal);
        i++;
    }
    if(cnt == (philo)->info->num_of_philo)
    {
        pthread_mutex_lock(&(philo)->info->dead_lock);
        philo->info->dead_philo = 1;
        pthread_mutex_unlock(&(philo)->info->dead_lock);
        return (0);
    }
    return (1);
}

int    philos_infinite_loop(t_philo *philo)
{
    int i;

    i = 0;
    while(1)
    {
        if(Is_dead(&(philo)->info->philos[i]) == 0)
            break ;
        if(philo->info->round_meals != -1 && philo->info->cnt_meals == 0)
        {
            if(ft_meals(philo) == 0)
                break ;
        }
        i++;
        if(i == philo->info->num_of_philo - 1)
            i = 0;
    }
    return (1);
}

int     initialize_philos(t_philo *philo)
{
    int i;
    t_philo *philos;
    i = 0;
    while(i < philo->info->num_of_philo)
    {
        philos = &(philo)->info->philos[i];
        philos->info = philo->info;
        philos->id = i + 1;
        philos->info->t_to_die = philo->info->t_to_die;
        philos->info->t_to_eat = philo->info->t_to_eat;
        philos->info->t_to_sleep = philo->info->t_to_sleep;
        philos->info->t_start = get_current_time_ms();
        philos->info->meal_eaten = 0;
        philos->info->T_last_meal = get_current_time_ms();
        philos->left_fork = &philo->info->forks[i];
        philos->right_fork = &philo->info->forks[(i + 1) % philo->info->num_of_philo];        
        i++;
    }
    return (1);
}

int      Lets_Go_Threads(t_philo *philo)
{
    int i;

    i = 0;
    if(initialize_philos(philo) == 0)
        return (0);
    while(i < philo->info->num_of_philo)
    {
        printf("ana hna\n");
        if(philo->info->philos[i].id % 2 == 0)
            usleep(200);
        if(pthread_create(philo->info->philos[i].info->threads, NULL, &routine_Multi_thread, &philo->info->philos[i]) != 0)
            return (0);
        i++;
    }
    philos_infinite_loop(philo);
    i = 0;
    while(i < philo->info->num_of_philo)
    {
        pthread_detach(*philo->info->philos[i].info->threads);
        i++;
    }
    return (1);
    
}
