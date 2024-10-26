/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assistant.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:06:29 by med-dahr          #+#    #+#             */
/*   Updated: 2024/10/26 04:29:53 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* ---->  Function to get the current time in milliseconds. 
        This is used for time tracking in the simulation.
*/
long long get_current_time_ms(void)
{
    struct timeval  tm;

    if (gettimeofday(&tm, NULL) == -1)
        return (printf("Error: gettimeofday failed\n"), -1);
    return (tm.tv_sec * 1000 + tm.tv_usec / 1000);
}

int initialize_philos(t_philo *philo)
{
    int i;

    i = 0;
    while (i < philo->info->num_of_philo)
    {
        pthread_mutex_init(&philo->info->philos[i].lock_meal, NULL);
        pthread_mutex_init(&philo->info->philos[i].mutex_time, NULL);
        philo->info->philos[i].id = i + 1;
        philo->info->philos[i].left_fork = &philo->info->forks[i];
        philo->info->philos[i].right_fork = &philo->info->forks[(i + 1) % philo->info->num_of_philo];
        philo->info->philos[i].info = philo->info;
        philo->info->philos[i].num_meal = 0;
        philo->info->philos[i].last_meal = get_current_time_ms();
        philo->info->philos[i].t_start = get_current_time_ms();
        if (pthread_create(&philo->info->philos[i].threads, NULL, &routine_Multi_thread, &philo->info->philos[i]) != 0)
        {
            write_error("Thread creation failed");
            return 0;
        }
        i++;
    }
    return 1;
}

// void looking_mutex(t_philo **philo)
// {
//     pthread_mutex_lock(&(*philo)->lock_meal);
//     pthread_mutex_unlock(&(*philo)->mutex_time);
//     pthread_mutex_unlock(&(*philo)->info->dead_lock);
// }

// void unlocking_mutex(t_philo **philo)
// {
//     pthread_mutex_unlock(&(*philo)->lock_meal);
//     pthread_mutex_unlock(&(*philo)->mutex_time);
//     pthread_mutex_unlock(&(*philo)->info->dead_lock);
// }

int state_philos(t_philo *philo)
{
    long last_meal;
    pthread_mutex_lock(&philo->mutex_time);  // Lock
    last_meal = philo->last_meal;
    pthread_mutex_unlock(&philo->mutex_time);  // Unlock
    if ((get_current_time_ms() - last_meal) >= philo->info->t_to_die) {
        return 0;
    }
    return 1;  // Philosopher is alive
}


int monitor_state_philo(t_philo *philo)
{
    int i;
    int all_philos_done;

    while (1)
    {
        all_philos_done = 1;  // Assume all philosophers have finished eating

        i = 0;
        while (i < philo->info->num_of_philo)
        {
            t_philo *current_philo = &philo->info->philos[i];

            // Check if a philosopher is dead
            if (state_philos(current_philo) == 0)
            {
                pthread_mutex_lock(&(philo->info->prt_lock));
                printf(RED "%lld %d is dead\n" NC,
                       get_current_time_ms() - philo->t_start,
                       current_philo->id);
                pthread_mutex_unlock(&(philo->info->prt_lock));
                return 0;  // Stop simulation if any philosopher dies
            }

            // Check if philosopher has not reached the meal limit
            pthread_mutex_lock(&current_philo->lock_meal);
            if (philo->info->limit_meals != -1 && current_philo->num_meal < philo->info->limit_meals)
                all_philos_done = 0;  // At least one philosopher hasn't finished
            pthread_mutex_unlock(&current_philo->lock_meal);

            i++;
        }

        // If all philosophers have eaten the required number of times, stop the simulation
        if (philo->info->limit_meals != -1 && all_philos_done)
            return 0;

        usleep(500);  // Small delay to avoid busy-waiting
    }
}

int Lets_Go_Threads(t_philo *philo)
{
    monitor_state_philo(philo);  // Monitor philosophers
    for (int i = 0; i < philo->info->num_of_philo; i++)
    {
        pthread_join(philo->info->philos[i].threads, NULL);  // Wait for threads to finish
    }
    return 1;
}

