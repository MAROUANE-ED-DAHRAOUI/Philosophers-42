/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assistant.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:06:29 by med-dahr          #+#    #+#             */
/*   Updated: 2024/09/28 19:23:53 by med-dahr         ###   ########.fr       */
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
    pthread_mutex_lock(philo->info->forks);
    if(thread_safe_print("%ld %d has taken a fork\n", philo) == 0)
    {
        pthread_mutex_unlock(philo->info->forks);
        return 0;
    }
    pthread_mutex_lock(philo->info->second_forks);
    if(thread_safe_print("%ld %d has taken a fork\n", philo) == 0)
    {
        pthread_mutex_unlock(philo->info->forks);
        pthread_mutex_unlock(philo->info->second_forks);
        return 0;
    }
    return 1;
}

void    user_sleep(int time, t_philo *philo)
{
    long    current_time;
    long    time_event;

    pthread_mutex_lock(&philo->info->p_lock);
    if (philo->info->dead_philo == 0)
    {
        pthread_mutex_unlock(&philo->info->p_lock);
        return ;
    }
    pthread_mutex_unlock(&philo->info->p_lock);
    time_event = (long)time;
    current_time = get_current_time_ms();
    time_event = time_event + current_time;
    while(current_time < time_event)
    {
        current_time = get_current_time_ms();
        usleep(200);
    }
}

int     philos_alternates(t_philo *philo)
{
    
    // printf ("Is_dead = %d\n", Is_dead(philo));
    while (1)
    {
        // if (Is_dead(philo) == 0)
        //     return 0;
        if(thread_safe_print("%ld %d is thinking\n", philo) == 0)
            return 0;
        if(check_take_forks(philo) == 0)
            break;
        thread_safe_print("%ld %d is eating\n", philo);
        user_sleep(philo->info->t_to_eat, philo);
        pthread_mutex_unlock(philo->info->forks);
        pthread_mutex_unlock(philo->info->second_forks);
        // pthread_mutex_lock(&philo->info->t_success);
        philo->last_time_eat = get_current_time_ms();

        // pthread_mutex_unlock(&philo->info->t_success);
        // _time = get_current_time_ms();
        // while(get_current_time_ms() - _time < philo->info->t_to_sleep)
        // {
        //     if(Is_dead(philo) == 0)
        //     {
        //         printf("Philosopher is dead\n");
        //         return 0;
        //     }
        //     usleep(200);
        // }
        // pthread_mutex_lock(&philo->info->t_success);
        // philo->num_of_eat--;
        // pthread_mutex_unlock(&philo->info->t_success);
        thread_safe_print("%ld %d is sleeping\n", philo);
        user_sleep(philo->info->t_to_sleep, philo);
        // _time = get_current_time_ms();
        // while(get_current_time_ms() - _time < philo->info->t_to_sleep)
        // {
        //     if(Is_dead(philo) == 0)
        //     {
        //         printf("Philosopher is dead 2\n");
        //         return 0;
        //     }
        //     usleep(200);
        // }
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
            usleep(200);
        }
    }
    if(philos_alternates(philo) == 0)
    {
        printf("Philosopher is hehhhhhreee\n");
        return NULL;
    }
    return 0;
}

// Function to check the state of all philosophers continuously in a loop.
// It checks if any philosopher has died. If one has, it breaks the loop.
int Check_Philos_State(t_philo *Philos, t_info *info)
{
    int i;
    long   s_time;

    i = 0;
    while (1)
    {
        i = 0;
        while (i < info->num_of_philo)
        {
            s_time = get_current_time_ms() - Philos[i].last_time_eat;
            if (s_time > info->t_to_die)
            {
                pthread_mutex_lock(&info->p_lock);
                info->dead_philo = 0;
                pthread_mutex_unlock(&info->p_lock);
                return 0;
            }
            // if (Is_dead(&Philos[i]) == 0)
            //     return 0;
            i++;
        }
    }
    i = 0;
    while(i < info->num_of_philo)
    {
        if (pthread_join(*(Philos[i].info->threads), NULL) == 0)
            {
                printf(RED"Ana Hnaaanananaaanahanahanah\n"NC);
                return 1;
            }
        i++;
    }
    return 0;
}

// Function to create multiple threads, one for each philosopher.
// It also initializes the forks and assigns them to each philosopher. 
int      Multi_Threads(t_philo *philo)
{
    int i;

    i = 0;
    philo->info->philos = malloc(philo->info->num_of_philo * sizeof(t_philo));
    if (philo->info->philos == NULL) {
        write_error("Memory allocation failed for philosophers");
        return 1;
    }
    philo->info->dead_philo = 1;
    while (i < philo->info->num_of_philo)
    {
        philo->info->philos[i].info = philo->info;
        philo->info->philos[i].id = i + 1;
        philo->info->philos[i].last_time_eat = get_current_time_ms();
        if (i != philo->info->num_of_philo)
        {
            philo->info->philos[i].info->forks = &philo->info->forks[i];
            philo->info->philos[i].info->second_forks = &philo->info->forks[i + 1];
            // philo->info->philos[i].left_fork = i;  // Left fork is the current philosopher's index.
            // philo->info->philos[i].right_fork = i + 1;  // Right fork is the next philosopher's index.
        }
        else if(i == philo->info->num_of_philo - 1)
        {
            philo->info->philos[i].info->forks = &philo->info->forks[i];
            philo->info->philos[i].info->second_forks = &philo->info->forks[0];
            // philo->info->philos[i].left_fork = i;  // Left fork is their index.
            // philo->info->philos[i].right_fork = 0;  // Right fork is the first philosopher's fork (circular).
        }
        // printf("Philosopher %d\n", philo->info->philos[i].id);
        // print_info(&philo->info->philos[i]);
        // printf("\n");
        pthread_create(philo->info->philos[i].info->threads, NULL, &Routine_Multi_Threads, (void *)&(philo->info->philos)[i]);
        i++;
    }
    if(Check_Philos_State(philo->info->philos, philo->info) == 0)
    {
        write_error("Philosopher is deadddddd\n");
        return (0);
    }
    return 1;
}

