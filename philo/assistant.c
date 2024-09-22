/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assistant.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:06:29 by med-dahr          #+#    #+#             */
/*   Updated: 2024/09/22 06:11:57 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Routine function for each philosopher thread when there are multiple philosophers.
// Currently, this function doesn't do anything and just returns NULL.
void *Routine_Multi_Threads(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
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
        pthread_join(threads[i], NULL);  // Waits for each thread to finish.
        i++;
    }
    if(i == num_of_philo)
        return 1;  // Returns 1 if all threads have been successfully joined.
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

        if (Is_dead(&(*Philos)[i]) == 0)  // Check if the philosopher is dead.
            break;  // If a philosopher is dead, break the loop.

        i++;
    }
    if(ft_joining_threads((*Philos)->info->threads, (*Philos)->info->num_of_philo) == 0)
        return 0;  // If thread joining fails, return 0.
    return 1;  // Otherwise, return 1.
}

// Function to create multiple threads, one for each philosopher.
// It also initializes the forks and assigns them to each philosopher.
int Multi_Threads(t_philo **philos)
{
    int i;

    i = 0;
    while (i < (*philos)->info->num_of_philo)
    {
        (*philos)[i].info = (*philos)->info;  // Assign shared info to each philosopher.
        (*philos)[i].id = i + 1;  // Assign an ID to each philosopher.
        (*philos)[i].num_of_eat = (*philos)->info->num_of_eat;  // Set number of times to eat.
        (*philos)[i].last_time_eat = get_current_time_ms();  // Set the last time they ate.

        if (i != (*philos)->info->num_of_philo - 1)  // Assign forks for each philosopher.
        {
            (*philos)[i].left_fork = i;  // Left fork is the current philosopher's index.
            (*philos)[i].right_fork = i + 1;  // Right fork is the next philosopher's index.
        }
        else if(i == (*philos)->info->num_of_philo - 1)  // For the last philosopher.
        {
            (*philos)[i].left_fork = i;  // Left fork is their index.
            (*philos)[i].right_fork = 0;  // Right fork is the first philosopher's fork (circular).
        }

        (*philos)[i].last_time_eat = get_current_time_ms();  // Update last eating time again.
        pthread_create(&(*philos)->info->threads[i], NULL, &Routine_Multi_Threads, (void *)&(*philos)[i]);  // Create a thread for each philosopher.
        i++;
    }

    if(Check_Philos_State(philos) == 0)  // Check philosopher states and join threads.
        return 0;  // If checking fails, return 0.

    return 1;  // Return 1 if all threads are created and checked successfully.
}
