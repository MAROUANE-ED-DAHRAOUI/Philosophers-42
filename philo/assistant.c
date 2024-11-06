/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assistant.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:06:29 by med-dahr          #+#    #+#             */
/*   Updated: 2024/11/06 16:43:05 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* ---->  Function to get the current time in milliseconds.
		This is used for time tracking in the simulation.
*/
long long	get_current_time_ms(void)
{
	struct timeval	tm;

	if (gettimeofday(&tm, NULL) == -1)
		return (printf("Error: gettimeofday failed\n"), -1);
	return (tm.tv_sec * 1000 + tm.tv_usec / 1000);
}

int	initialize_philos(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->info->num_of_philo)
	{
		pthread_mutex_init(&philo->info->philos[i].lock_meal, NULL);
		pthread_mutex_init(&philo->info->philos[i].mutex_time, NULL);
		pthread_mutex_init(&philo->info->stop_lock, NULL);
		philo->info->philos[i].id = i + 1;
		philo->info->philos[i].left_fork = &philo->info->forks[i];
		philo->info->philos[i].right_fork = &philo->info->forks[(i + 1)
			% philo->info->num_of_philo];
		philo->info->philos[i].info = philo->info;
		philo->info->philos[i].num_meal = 0;
		philo->info->philos[i].last_meal = get_current_time_ms();
		philo->info->philos[i].t_start = get_current_time_ms();
		i++;
	}
	return (1);
}

/**
 * Checks the state of a philosopher.
 *

	* This function checks if a philosopher has exceeded the time
		 limit since their last meal.
 * If the time since the last meal is greater than or equal to the time to die,
	the philosopher is considered dead.
 *
 *  philo A pointer to the philosopher structure.
 *  return 0 if the philosopher is dead, 1 otherwise.
 */
int	state_philos(t_philo *philo)
{
	long	last_meal;

	pthread_mutex_lock(&philo->mutex_time);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->mutex_time);
	if ((get_current_time_ms() - last_meal) >= philo->info->t_to_die)
	{
		return (0);
	}
	return (1);
}

/**
 * Monitors the state of the philosophers and checks if they 
		have finished their meals.
 *
 * philo The pointer to the t_philo struct representing the philosophers.
 * Returns 1 if the monitoring is successful, 0 otherwise.
 */
int	monitor_state_philo(t_philo *philo)
{
	t_philo	*current_philo;
	int		i;
	int		finished_philosophers;

	while (1)
	{
		finished_philosophers = 0;
		i = 0;
		while (i < philo->info->num_of_philo)
		{
			current_philo = &philo->info->philos[i];
			if (!check_philosopher_state(philo, current_philo)
				|| !check_philosopher_meals(philo, current_philo,
					&finished_philosophers))
				return (0);
			i++;
		}
		if (!end_simulation_if_all_finished(philo, finished_philosophers))
			return (0);
		usleep(500);
	}
	return (1);
}

/**
 * Function to wait for all philosopher threads to finish.
 *

	* This function waits for all philosopher threads to finish 
		by calling pthread_join() on each thread.
 * If the monitor_state_philo() function returns false,
	the function frees the memory allocated for the philosophers.
 *

	*  philo A pointer to the t_philo structure containing
	 	information about the philosophers.
 *  1 if all threads have finished successfully, 0 otherwise.
 */
int	go_threads(t_philo *philo)
{
	int	i;

	i = 0;
	if (!monitor_state_philo(philo))
	{
		if (philo != NULL)
			ft_free(philo);
	}
	while (i < philo->info->num_of_philo && philo->info->num_of_philo > 1)
	{
		pthread_join(philo->info->philos[i].threads, NULL);
		i++;
	}
	return (1);
}
