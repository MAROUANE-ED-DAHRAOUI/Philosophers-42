/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assistant.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:06:29 by med-dahr          #+#    #+#             */
/*   Updated: 2024/09/27 23:30:45 by med-dahr         ###   ########.fr       */
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
    {
        pthread_mutex_unlock(&philo->info->forks[philo->left_fork]);
        pthread_mutex_unlock(&philo->info->forks[philo->right_fork]);
        return 0;
    }
    return 1;
}

int     philos_alternates(t_philo *philo)
{
    long    _time;
    // while(Is_dead(philo) == 1 && philo->num_of_eat != 0)
    // {
    //     if(thread_safe_print("%ld %d is thinking\n", philo) == 0)
    //         return 0;
    //     if(check_take_forks(philo) == 0)
    //         break;
    //     thread_safe_print("%ld %d is eating\n", philo);
    //     pthread_mutex_lock(&philo->info->t_success);
    //     philo->num_of_eat = get_current_time_ms();
    //     pthread_mutex_unlock(&philo->info->t_success);
    //     _time = get_current_time_ms();
    //     while(get_current_time_ms() - _time < philo->info->t_to_sleep)
    //     {
    //         if(Is_dead(philo) == 0)
    //             return 0;
    //         usleep(500);
    //     }
    //     pthread_mutex_lock(&philo->info->t_success);
    //     philo->num_of_eat--;
    //     pthread_mutex_unlock(&philo->info->t_success);
    //     pthread_mutex_unlock(&philo->info->forks[philo->left_fork]);
    //     pthread_mutex_unlock(&philo->info->forks[philo->right_fork]);
    //     thread_safe_print("%ld %d is sleeping\n", philo);
    //     _time = get_current_time_ms();
    //     while(get_current_time_ms() - _time < philo->info->t_to_sleep)
    //     {
    //         if(Is_dead(philo) == 0)
    //             return 0;
    //         usleep(500);
    //     }
    // }
    // return 1;
    printf ("Is_dead = %d\n", Is_dead(philo));
    while (1)
    {
        // if (Is_dead(philo) == 0)
        //     return 0;
        if(thread_safe_print("%ld %d is thinking\n", philo) == 0)
            return 0;
        if(check_take_forks(philo) == 0)
            break;
        thread_safe_print("%ld %d is eating\n", philo);
        pthread_mutex_lock(&philo->info->t_success);
        philo->info->t_to_eat = get_current_time_ms();
        pthread_mutex_unlock(&philo->info->t_success);
        _time = get_current_time_ms();
        while(get_current_time_ms() - _time < philo->info->t_to_sleep)
        {
            if(Is_dead(philo) == 0)
            {
                printf("Philosopher is dead\n");
                return 0;
            }
            usleep(200);
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
            {
                printf("Philosopher is dead 2\n");
                return 0;
            }
            usleep(200);
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
            usleep(200);
        }
    }
    if(philos_alternates(philo) == 0)
    {
        printf("Philosopher is hehhhhhreee\n");
        return NULL;
    }
    printf("had lphilo kharij\n");
    return 0;
}

// Function to check the state of all philosophers continuously in a loop.
// It checks if any philosopher has died. If one has, it breaks the loop.
int Check_Philos_State(t_philo *Philos, t_info *info)
{
    int i;

    i = 0;
    while (1)
    {
        if(i == info->num_of_philo)
            i = 0;  // Reset to first philosopher if we reach the last one.

        if (Is_dead(&Philos[i]) == 0)
            break;
        i++;
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

void print_info(t_philo *philo)
{
    printf("Number of philosophers = %d\n", philo->info->num_of_philo);
    printf("Time to die = %d\n", philo->info->t_to_die);
    printf("Time to eat = %d\n", philo->info->t_to_eat);
    printf("Time to sleep = %d\n", philo->info->t_to_sleep);
    printf("Number of times each philosopher must eat = %d\n", philo->info->num_of_eat);
    printf("Dead philosophers = %d\n", philo->info->dead_philo);
    printf("Last time a philosopher ate = %ld\n", philo->last_time_eat);   
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
            philo->info->philos[i].left_fork = i;  // Left fork is the current philosopher's index.
            philo->info->philos[i].right_fork = i + 1;  // Right fork is the next philosopher's index.
        }
        else if(i == philo->info->num_of_philo - 1)
        {
            philo->info->philos[i].left_fork = i;  // Left fork is their index.
            philo->info->philos[i].right_fork = 0;  // Right fork is the first philosopher's fork (circular).
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

