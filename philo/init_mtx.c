/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mtx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:58:04 by med-dahr          #+#    #+#             */
/*   Updated: 2024/10/14 13:55:45 by med-dahr         ###   ########.fr       */
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
    pthread_mutex_init(&(philo->info->prt_lock), NULL);
    pthread_mutex_init(&(philo->info->lock_meal), NULL);
    pthread_mutex_init(&(philo->info->dead_lock), NULL);
    pthread_mutex_init(&(philo->info->philo_dead), NULL);
    pthread_mutex_init(&(philo->info->lst_meal_lock), NULL);
    while(i < philo->info->num_of_philo)
    {
        if(pthread_mutex_init(&(philo->info->forks[i]), NULL) != 0)
            return (0);
        i++;
    }
    return (1);
}

/*
 -----> Function to check if a philosopher is dead by
        inspecting the shared `dead_philo` variable with proper mutex locking.
*/
int Is_dead(t_philo *philo)
{
    int _time;

    pthread_mutex_lock(&(philo->info->lst_meal_lock));
    _time = get_current_time_ms() - philo->info->T_last_meal;
    pthread_mutex_unlock(&(philo->info->lst_meal_lock));

    if (_time >= philo->info->t_to_die)
    {
        pthread_mutex_lock(&(philo->info->dead_lock));
        if (philo->info->dead_philo == 0) // Avoid race on multiple death prints.
        {
            philo->info->dead_philo = 1;
            pthread_mutex_lock(&(philo->info->prt_lock));
            printf(RED"%ld %d is dead\n"NC, get_current_time_ms() - philo->info->t_start, philo->id);
            pthread_mutex_unlock(&(philo->info->prt_lock));
        }
        pthread_mutex_unlock(&(philo->info->dead_lock));
        return 0;
    }
    return 1;
}

void    sleep_philo(int time)
{
    long    s_time;

    s_time = get_current_time_ms();
    while(get_current_time_ms() - s_time < time)
    {
        usleep(200);
    }
}

