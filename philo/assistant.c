/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assistant.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:06:29 by med-dahr          #+#    #+#             */
/*   Updated: 2024/09/20 19:08:34 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int     Multi_Threads(t_philo **philos)
{
    int i;

    i = 0;
    while(i < (*philos)->info->num_of_philo)
    {
        pthread_create(&(*philos)->info->threads[i], NULL, &routine_one_thread, (void *)(*philos));
        i++;
    }
    i = 0;
    while(i < (*philos)->info->num_of_philo)
    {
        pthread_join((*philos)->info->threads[i], NULL);
        i++;
    }
    return (1);
}
