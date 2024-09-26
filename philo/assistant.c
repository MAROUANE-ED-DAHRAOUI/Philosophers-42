/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assistant.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:06:29 by med-dahr          #+#    #+#             */
/*   Updated: 2024/09/26 18:13:19 by med-dahr         ###   ########.fr       */
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

int     check_take_forks(t_philo *philo)
{
    pthread_mutex_lock(&philo->info->forks[philo->left_fork]);
    if(thread_safe_print("%ld %d has taken a fork\n", philo) == 0)
    {
        pthread_mutex_unlock(&philo->info->forks[philo->left_fork]);
        pthread_mutex_unlock(&philo->info->forks[philo->right_fork]);
        return 0;
    }
    pthread_mutex_lock(&philo->info->forks[philo->right_fork]);
    if(thread_safe_print("%ld %d has taken a fork\n", philo) == 0)
        return 0;
    return 1;
}

int     philos_alternates(t_philo *philo)
{
    long    _time;
    while(Is_dead(philo) == 1 && philo->num_of_eat != 0)
    {
        if(thread_safe_print("%ld %d is thinking\n", philo) == 0)
            return 0;
        if(check_take_forks(philo) == 0)
            break;
        thread_safe_print("%ld %d is eating\n", philo);
        pthread_mutex_lock(&philo->info->t_success);
        philo->num_of_eat = get_current_time_ms();
        pthread_mutex_unlock(&philo->info->t_success);
        _time = get_current_time_ms();
        while(get_current_time_ms() - _time < philo->info->t_to_sleep)
        {
            if(Is_dead(philo) == 0)
                return 0;
            usleep(500);
        }
        pthread_mutex_lock(&philo->info->t_success);
        philo->num_of_eat--;
        pthread_mutex_unlock(&philo->info->t_success);
        pthread_mutex_unlock(&philo->info->forks[philo->left_fork]);
        pthread_mutex_unlock(&philo->info->forks[philo->right_fork]);
        thread_safe_print("%ld %d is sleeping\n", philo);
        _time = get_current_time_ms();
        while(get_current_time_ms() - _time < philo->info->t_to_sleep)
        {
            if(Is_dead(philo) == 0)
                return 0;
            usleep(500);
        }
    }
    return 1;
}

void *Routine_Multi_Threads(void *arg)
{
    t_philo *philo;
    long    s_time;

    philo = (t_philo *)arg;
    if(philo->id % 2 == 0)
    {
        if(thread_safe_print("%ld %d is sleeping\n", philo) == 0)
            return NULL;
        s_time = get_current_time_ms();
        while(get_current_time_ms() - s_time < philo->info->t_to_sleep)
        {
            if(Is_dead(philo) == 0)
                return NULL;
            usleep(500);
        }
    }
    if(philos_alternates(philo) == 0)
        return NULL;
    return NULL;
}

// Function to join all philosopher threads (waits for all threads to finish).
// Iterates over all philosopher threads and joins them.
int ft_joining_threads(pthread_t *threads, int num_of_philo)
{
    int i;

    i = 0;
    while (i < num_of_philo)
    {
        if (pthread_join(threads[i], NULL) == 0){
            return 0;  // If thread joining fails, return 0.
        }
        i++;
    }
    if(i == num_of_philo)
        return 1;
    return 0;
}

// Function to check the state of all philosophers continuously in a loop.
// It checks if any philosopher has died. If one has, it breaks the loop.
int Check_Philos_State(t_philo **Philos)
{
    int i;

    i = 0;
    while (1)
    {
        if(i == (*Philos)->info->num_of_philo)
            i = 0;  // Reset to first philosopher if we reach the last one.

        if (Is_dead(Philos[i]) == 0)  // Check if the philosopher is dead.
            break;  // If a philosopher is dead, break the loop.

        i++;
    }
    i = 0;
    while(i < (*Philos)->info->num_of_philo)
    {
        if (pthread_join((*Philos)[i].info->threads[0], NULL) == 0)
            return 0;  // If thread joining fails, return 0.
        i++;
    }
    return 1;
}

// Function to create multiple threads, one for each philosopher.
// It also initializes the forks and assigns them to each philosopher. 
int     Multi_Threads(t_philo *philo)
{
    int i;
    t_philo *philos;

    i = 0;
    philos = malloc(philo->info->num_of_philo * sizeof(t_philo));
    if (philos == NULL) {
        write_error("Memory allocation failed for philosophers");
        return 1;
    }
    while (i < philo->info->num_of_philo)
    {
        philos[i].info = philo->info;
        philos[i].id = i + 1;
        philos[i].num_of_eat = philo->info->num_of_eat;
        philos[i].last_time_eat = get_current_time_ms();
        if (i != philo->info->num_of_philo)
        {
            philos[i].left_fork = i;  // Left fork is the current philosopher's index.
            philos[i].right_fork = i + 1;  // Right fork is the next philosopher's index.
        }
        else if(i == philos->info->num_of_philo - 1)
        {
            philos[i].left_fork = i;  // Left fork is their index.
            philos[i].right_fork = 0;  // Right fork is the first philosopher's fork (circular).
        }
        philos[i].last_time_eat = get_current_time_ms();
        pthread_create(philos[i].info->threads, NULL, &Routine_Multi_Threads, (void *)&(philos)[i]);
        i++;
    }
    if(Check_Philos_State(&philos) == 0)
    {
            write_error("Philosopher is dead");
            return (0);
    }
    return 1;
}
