/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assistant.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:06:29 by med-dahr          #+#    #+#             */
/*   Updated: 2024/10/20 18:15:23 by med-dahr         ###   ########.fr       */
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
    return (tm.tv_sec * 1000) + (tm.tv_usec / 1000);
}

int initialize_philos(t_philo **philo)
{
    int i;

    i = 0;
    if (philo == NULL || *philo == NULL) {
        fprintf(stderr, "Philo pointer is NULL\n");
        return 1;
    }
    while (i < (*philo)->info->num_of_philo)
    {
        (*philo)->info->philos[i].id = i + 1;
        (*philo)->info->philos[i].left_fork = &(*philo)->info->forks[i];
        (*philo)->info->philos[i].right_fork = &(*philo)->info->forks[(i + 1) % (*philo)->info->num_of_philo];
        (*philo)->info->philos[i].info = (*philo)->info;
        (*philo)->info->philos[i].num_meal = 0;
        (*philo)->info->philos[i].last_meal = get_current_time_ms();
        (*philo)->info->philos[i].t_start = get_current_time_ms();
        pthread_mutex_init(&(*philo)->info->philos[i].lock_meal, NULL);
        pthread_mutex_init(&(*philo)->info->philos[i].mutex_time, NULL);
        pthread_create(&(*philo)->info->philos[i].threads, NULL, &routine_Multi_thread, &(*philo)->info->philos[i]);
        i++;
    }
    return (1);
}

void looking_mutex(t_philo **philo)
{
    pthread_mutex_lock(&(*philo)->lock_meal);
    pthread_mutex_unlock(&(*philo)->mutex_time);
    pthread_mutex_unlock(&(*philo)->info->dead_lock);
}

void unlocking_mutex(t_philo **philo)
{
    pthread_mutex_unlock(&(*philo)->lock_meal);
    pthread_mutex_unlock(&(*philo)->mutex_time);
    pthread_mutex_unlock(&(*philo)->info->dead_lock);
}

static int state_philos(t_philo *philo)
{
    static int i;

    // while(i < philo->info->num_of_philo)
    // {
    //         printf("id %d = %d\n", i, philo->info->philos[i].id);
    //         i++;
    // }
    i = 0;
    looking_mutex(&philo);
    pthread_mutex_lock(&philo->mutex);
    if((get_current_time_ms() - philo->last_meal) >= philo->info->t_to_die)
    {
        philo->info->dead_id = philo->info->philos[i].id;
        philo->info->_exit = false;
        pthread_mutex_unlock(&philo->mutex);
        unlocking_mutex(&philo);
        // i++;
        return 0;
    }
    pthread_mutex_unlock(&philo->mutex);
    unlocking_mutex(&philo);
    return (1);
}

int monitor_state_philo(t_philo *philo)
{
    int i;
    int max;

    while(1)
    {
        max = INT_MAX;
        i = -1;
        while(++i < philo->info->num_of_philo)
        {
            if(state_philos(&philo[i]) == 0)
                return 0;
            if(philo->limit_meals != -1 && philo[i].num_meal < max)
                max = philo[i].num_meal;
            pthread_mutex_unlock(&philo->info->philo_dead);
            pthread_mutex_unlock(&philo[i].mutex_time);
            pthread_mutex_unlock(&philo[i].lock_meal);
        }

        if (philo->limit_meals != -1 && philo->limit_meals <= max)
        {
            philo->info->_exit = false;
            return 0;
        }
        usleep(500);
    }
    return (1);
}

int Lets_Go_Threads(t_philo *philo)
{
    int i;

    i = -1;
    if(monitor_state_philo(philo) == 0)
        printf("%ld %d is dead\n", (get_current_time_ms() - philo->t_start), philo->info->dead_id);
    pthread_mutex_lock(&philo->info->philo_dead);
    philo->info->_exit = false;
    pthread_mutex_unlock(&philo->info->philo_dead);
    while(++i < philo->info->num_of_philo)
            pthread_join(philo->info->philos[i].threads, NULL);
    ft_free(philo);
    return 1;
}
