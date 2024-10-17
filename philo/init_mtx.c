/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mtx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:58:04 by med-dahr          #+#    #+#             */
/*   Updated: 2024/10/17 10:30:41 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
------> Function to initialize several mutexes,
        one for each philosopher fork and other shared
        resources like `p_lock`, `t_check`, and `t_success`.
*/
int init_several_mtx(t_philo *philo)
{
    int i;

    i = 0;
    // pthread_mutex_init(&(philo->info->lock_meal), NULL);
    // pthread_mutex_init(&(philo->info->dead_lock), NULL);
    // pthread_mutex_init(&(philo->info->lst_meal_lock), NULL);
    while(i < philo->info->num_of_philo)
    {
        pthread_mutex_init(&(philo->info->forks[i]), NULL);
        i++;
    }
    pthread_mutex_init(&(philo->info->prt_lock), NULL);
    pthread_mutex_init(&(philo->info->philo_dead), NULL);
    return (1);
}

/*
 -----> Function to check if a philosopher is dead by
        inspecting the shared `dead_philo` variable with proper mutex locking.
*/
int Is_dead(t_philo *philo)
{
    pthread_mutex_lock(&(philo->info->philo_dead));
    if(philo->info->_exit == true)
    {
        pthread_mutex_unlock(&(philo->info->philo_dead));
        return (0);
    }
    pthread_mutex_unlock(&(philo->info->philo_dead));
    return (1);
}

void    sleep_philo(int time)
{
    long    s_time;

    s_time = get_current_time_ms();
    while((get_current_time_ms() - s_time) < time)
    {
        usleep(500);
    }
}

