/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mtx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:58:04 by med-dahr          #+#    #+#             */
/*   Updated: 2024/11/06 00:17:17 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 -----> Function to initialize mutexes for forks, philosophers, and print locks.
*/
int	init_several_mtx(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->info->num_of_philo)
	{
		pthread_mutex_init(&philo->info->forks[i], NULL);
		pthread_mutex_init(&philo->info->philos[i].mutex_time, NULL);
		pthread_mutex_init(&philo->info->philos[i].mutex, NULL);
		pthread_mutex_init(&philo->info->stop_lock, NULL);
		i++;
	}
	pthread_mutex_init(&philo->info->prt_lock, NULL);
	return (1);
}

/*
 -----> Function to check if a philosopher is dead by
		inspecting the shared `dead_philo` variable with proper mutex locking.
*/
bool	Is_dead(t_philo *philo)
{
	bool	dead;

	dead = false;
	pthread_mutex_lock(&philo->info->stop_lock);
	dead = philo->info->_exit;
	pthread_mutex_unlock(&philo->info->stop_lock);
	return (dead);
}

void	sleep_philo(int time)
{
	long	s_time;

	s_time = get_current_time_ms();
	while ((get_current_time_ms() - s_time) < time)
	{
		usleep(500);
	}
}
