/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _helper1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:20:09 by med-dahr          #+#    #+#             */
/*   Updated: 2024/11/06 22:11:43 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Checks the state of a philosopher and performs necessary actions 
 *      if the philosopher is dead.
 * 
 * philo : The philosopher whose state is being checked.
 * current_philo : The current philosopher being checked.
 * Returns 0 if the philosopher is dead, 1 otherwise.
 */
int	check_philosopher_state(t_philo *philo, t_philo *current_philo)
{
	if (state_philos(current_philo) == 0)
	{
		pthread_mutex_lock(&(philo->info->stop_lock));
		philo->info->_exit = false;
		pthread_mutex_unlock(&(philo->info->stop_lock));
		pthread_mutex_lock(&(philo->info->prt_lock));
		printf(RED "%lld %d died\n" NC, get_current_time_ms()
			- philo->t_start, current_philo->id);
		pthread_mutex_unlock(&(philo->info->prt_lock));
		ft_free(philo);
		return (0);
	}
	return (1);
}

/**
 * Checks the number of meals consumed by a philosopher.
 * 
 * This function checks if the number of meals consumed by a philosopher
 * exceeds the specified limit. If the limit is reached, the count of finished
 * philosophers is incremented.
 * 
 * philo The philosopher whose meals are being checked.
 * current_philo The current philosopher being checked.
 * finished_philosophers Pointer to the count of finished philosophers.
 * return 1 if the check is successful.
 */
int	check_philosopher_meals(t_philo *philo, t_philo *current_philo,
		int *finished_philosophers)
{
	pthread_mutex_lock(&current_philo->lock_meal);
	if (philo->info->limit_meals != -1
		&& current_philo->num_meal >= philo->info->limit_meals)
	{
		(*finished_philosophers)++;
	}
	pthread_mutex_unlock(&current_philo->lock_meal);
	return (1);
}

/**
* Checks if all philosophers have finished their meals and 
    ends the simulation if true.
 *  philo The pointer to the t_philo struct.
 * finished_philosophers The number of philosophers that 
 *  have finished their meals.
 * Returns 0 if the simulation is ended, 1 otherwise.
*/
int	end_simulation_if_all_finished(t_philo *philo, int finished_philosophers)
{
	int	i;

	if (finished_philosophers == philo->info->num_of_philo)
	{
		if (is_dead(philo) == 0)
		{
			ft_free(philo);
			return (0);
		}
		stop_all_philosophers(philo->info);
		i = 0;
		while (i < philo->info->num_of_philo)
		{
			pthread_join(philo->info->philos[i].threads, NULL);
			i++;
		}
		return (0);
	}
	return (1);
}
