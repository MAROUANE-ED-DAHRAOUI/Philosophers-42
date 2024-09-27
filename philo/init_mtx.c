/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mtx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:58:04 by med-dahr          #+#    #+#             */
/*   Updated: 2024/09/27 22:50:40 by med-dahr         ###   ########.fr       */
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
    pthread_mutex_init(&philo->info->p_lock, NULL);
    pthread_mutex_init(&philo->info->t_check, NULL);
    pthread_mutex_init(&philo->info->t_success, NULL);
    while(i < philo->info->num_of_philo)
    {
        pthread_mutex_init(&philo->info->forks[i], NULL);
        i++;
    }
    // printf(BLUE"i = %d .  philo->info->num_of_philo = %d\n"NC, i, philo->info->num_of_philo);
    if(i != philo->info->num_of_philo)
        return (0);
    else
        return (1);
}

/*
 -----> Function to check if a philosopher is dead by
        inspecting the shared `dead_philo` variable with proper mutex locking.
*/
int Is_dead(t_philo *philo)
{
    
    if(philo == NULL)
        return (1);
    pthread_mutex_lock(&philo->info->t_check);
    if(philo->info->dead_philo == 0)
    {
        pthread_mutex_unlock(&philo->info->t_check);
        return (0);
    }  
    pthread_mutex_unlock(&philo->info->t_check);
    return (1);
}

/*
------> Routine function for the philosopher when there
        is only one philosopher (thread).
        Handles the philosopher's actions and checks for death.
*/ 
void *routine_one_thread(void *arg)
{
    t_philo     *philo;
    long        time;
    long        t_begin;

    time = get_current_time_ms();
    philo = (t_philo *)arg;
    pthread_mutex_lock(&philo->info->forks[0]);  // Philosopher picks up the only fork available.
    printf(BLUE"%ld 1 taken a fork\n"NC, time - philo->info->t_start);
    pthread_mutex_unlock(&philo->info->forks[0]); // Immediately releases the fork.(KAYHTHA)
    t_begin = get_current_time_ms();
    if(Is_dead(philo) == 1)
    {
        while(get_current_time_ms() - t_begin < philo->info->t_to_die)
        {
            usleep(200);
        }
        printf(RED"%ld 1 is dead\n"NC,get_current_time_ms() - philo->info->t_start);
    }
    return (NULL);
}

/*
---->Function to check and handle philosopher threads.
        If there is only one philosopher, it runs a single thread;
        otherwise, it creates multiple threads.
*/
int check_threads(t_philo *philo)
{
    if(philo->info->num_of_philo == 1)
    {
        pthread_create(&philo->info->threads[0], NULL, &routine_one_thread, (void *)philo);
        pthread_join(philo->info->threads[0], NULL);
        return (1);
    }
    else if(philo->info->num_of_philo > 1)
    {
        if(Multi_Threads(philo) == 1)
        {
            return (1);
        }
    }
    return (0);
}
