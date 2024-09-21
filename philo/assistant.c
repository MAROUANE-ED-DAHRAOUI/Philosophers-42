/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assistant.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:06:29 by med-dahr          #+#    #+#             */
/*   Updated: 2024/09/21 19:28:06 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void        *Routine_Multi_Threads(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    return NULL;
}

int     ft_joining_threads(pthread_t *threads, int num_of_philo)
{
    int i;

    i = 0;
    while (i < num_of_philo)
    {
        pthread_join(threads[i], NULL);
        i++;
    }
    if(i == num_of_philo)
        return 1;
    return 0;
}

int     Check_Philos_State(t_philo **Philos)
{
    int i;

    i = 0;
    while (1)
    {
        if(i == (*Philos)->info->num_of_philo)
            i = 0;
        if (Is_dead(&(*Philos)[i]) == 0)
            break ;
        i++;
    }
    if(ft_joining_threads((*Philos)->info->threads, (*Philos)->info->num_of_philo) == 0)
        return 0;
    return 1;
}

int     Multi_Threads(t_philo **philos)
{
    int i;

    i = 0;
    while (i < (*philos)->info->num_of_philo)
    {
        (*philos)[i].info = (*philos)->info;
        (*philos)[i].id = i + 1;
        (*philos)[i].num_of_eat = (*philos)->info->num_of_eat;
        (*philos)[i].last_time_eat = get_current_time_ms();
        if (i != (*philos)->info->num_of_philo - 1)
        {
            (*philos)[i].left_fork = i;
            (*philos)[i].right_fork = i + 1;
        }
        else if(i == (*philos)->info->num_of_philo - 1)
        {
            (*philos)[i].left_fork = i;
            (*philos)[i].right_fork = 0;
        }
        (*philos)[i].last_time_eat = get_current_time_ms();
        pthread_create(&(*philos)->info->threads[i], NULL, &Routine_Multi_Threads, (void *)&(*philos)[i]);
        i++;
    }
    if(Check_Philos_State(philos) == 0)
        return 0;
    return 1;
}
