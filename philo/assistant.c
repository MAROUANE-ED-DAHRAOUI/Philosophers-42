/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assistant.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:06:29 by med-dahr          #+#    #+#             */
/*   Updated: 2024/11/04 12:51:27 by med-dahr         ###   ########.fr       */
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
        pthread_mutex_init(&philo->info->stop_lock, NULL);
        philo->info->philos[i].id = i + 1;
        philo->info->philos[i].left_fork = &philo->info->forks[i];
        philo->info->philos[i].right_fork = &philo->info->forks[(i + 1) % philo->info->num_of_philo];
        philo->info->philos[i].info = philo->info;
        philo->info->philos[i].num_meal = 0;
        philo->info->philos[i].last_meal = get_current_time_ms();
        philo->info->philos[i].t_start = get_current_time_ms();
        // philo->info->stop_simulation = false;

        // pthread_create(&philo->info->philos[i].threads, NULL, &routine_Multi_thread, &philo->info->philos[i]);
        i++;
    }
    return 1;
}

int state_philos(t_philo *philo)
{
    long last_meal;
    pthread_mutex_lock(&philo->mutex_time);
    last_meal = philo->last_meal;
    pthread_mutex_unlock(&philo->mutex_time);
    if ((get_current_time_ms() - last_meal) >= philo->info->t_to_die) {
        return 0;
    }
    return 1;
}

// Function to set the stop_simulation flag
void stop_all_philosophers(t_info *info)
{
    pthread_mutex_lock(&info->stop_lock);
    info->stop_simulation = true;
    pthread_mutex_unlock(&info->stop_lock);
}

int monitor_state_philo(t_philo *philo)
{
    int i;
    int finished_philosophers;
    t_philo *current_philo;

    while (1)
    {
        finished_philosophers = 0;
        i = 0;
        while(i < philo->info->num_of_philo)
        {
            current_philo = &philo->info->philos[i];
            // Check if the philosopher is dead
            if (state_philos(current_philo) == 0)
            {
                // if(Is_dead(philo) == 0)
                // {
                //     ft_free(philo);
                //     return 0;
                // }
                pthread_mutex_lock(&(philo->info->stop_lock));
                philo->info->_exit = false;
                pthread_mutex_unlock(&(philo->info->stop_lock));

                pthread_mutex_lock(&(philo->info->prt_lock));
                printf(RED "%lld %d is dead\n" NC, 
                       get_current_time_ms() - philo->t_start, 
                       current_philo->id);
                pthread_mutex_unlock(&(philo->info->prt_lock));
                ft_free(philo);
                return 0;
            }

            // Check if the philosopher has reached the required meal count
            pthread_mutex_lock(&current_philo->lock_meal);
            if (philo->info->limit_meals != -1 && 
                current_philo->num_meal >= philo->info->limit_meals)
            {
                finished_philosophers++;
            }
            pthread_mutex_unlock(&current_philo->lock_meal);
            i += 1;
        }
        if (finished_philosophers == philo->info->num_of_philo)
        {
            if(Is_dead(philo) == 0)
            {
                ft_free(philo);
                return (0);
            }
            // printf("All philosophers have finished eating\n");
            stop_all_philosophers(philo->info);
            for (int i = 0; i < philo->info->num_of_philo; i++) {
            pthread_join(philo->info->philos[i].threads, NULL);
            }
            return (0);
        }
        usleep(500);
    }
        return (1);
}

int Lets_Go_Threads(t_philo *philo)
{
    int i;

    i = 0;
    // printf("joining...\n");
    //  while (i < philo->info->num_of_philo && philo->info->num_of_philo > 1)
    // {
    //     // printf("joining...\n");
    //     pthread_join(philo->info->philos[i].threads, NULL);
    //     i++;
    // }
    if(!monitor_state_philo(philo))
    {
        if(philo != NULL)
            ft_free(philo);

        // return (0);
    }
    printf("joining...\n");
     while (i < philo->info->num_of_philo && philo->info->num_of_philo > 1)
    {
        // printf("joining...\n");
        pthread_join(philo->info->philos[i].threads, NULL);
        i++;
    }
    return (1);
}
