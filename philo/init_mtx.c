/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mtx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:58:04 by med-dahr          #+#    #+#             */
/*   Updated: 2024/10/30 12:31:50 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 -----> Function to initialize mutexes for forks, philosophers, and print locks.
*/
int init_several_mtx(t_philo *philo)
{
    int i;

    i = 0;
    while(i < philo->info->num_of_philo)
    {
        pthread_mutex_init(&philo->info->forks[i], NULL);
        pthread_mutex_init(&philo->info->philos[i].meal_mutex, NULL);
        pthread_mutex_init(&philo->info->philos[i].mutex_time, NULL);
        pthread_mutex_init(&philo->info->philos[i].mutex, NULL);
        i++;
    }
    pthread_mutex_init(&philo->info->prt_lock, NULL);
    pthread_mutex_init(&philo->info->philo_dead, NULL);
    return 1;
}

/*
 -----> Function to check if a philosopher is dead by
        inspecting the shared `dead_philo` variable with proper mutex locking.
*/  
bool Is_dead(t_philo *philo) 
{
    bool dead;

    pthread_mutex_lock(&philo->mutex);
    dead = philo->info->_exit; // Access shared state
    pthread_mutex_unlock(&philo->mutex);
    return (dead);
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

