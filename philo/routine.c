/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 22:29:25 by med-dahr          #+#    #+#             */
/*   Updated: 2024/11/06 22:52:07 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	_thinking(t_philo *philo)
{
	print_moves(philo, "is thinking");
}

void	print_moves(t_philo *philo, char *str)
{
	if (is_dead(philo))
	{
		pthread_mutex_lock(&(philo->info->prt_lock));
		printf(BLUE "%lld %d %s\n" NC, get_current_time_ms() - philo->t_start,
			philo->id, str);
		pthread_mutex_unlock(&(philo->info->prt_lock));
		return ;
	}
}

int	one_thread(t_philo *philo)
{
	if (philo->info->num_of_philo == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_moves(philo, "has taken a fork");
		usleep(philo->info->t_to_die * 1000);
		print_moves(philo, "died");
		pthread_mutex_unlock(philo->left_fork);
		ft_free(philo);
		return (0);
	}
	return (1);
}

/**
 * Function that represents the eating routine of a philosopher.

	* This function is responsible for simulating the eating 
		behavior of a philosopher.
 */

void	_eating(t_philo *philo)
{
	if (is_dead(philo) == 0)
	{
		write_error("Error in the simulation");
		return ;
	}
	pthread_mutex_lock(philo->right_fork);
	print_moves(philo, "has taken a fork");
	pthread_mutex_lock(philo->left_fork);
	print_moves(philo, "has taken a fork");
	print_moves(philo, "is eating");
	pthread_mutex_lock(&philo->mutex_time);
	philo->last_meal = get_current_time_ms();
	pthread_mutex_unlock(&philo->mutex_time);
	pthread_mutex_lock(&philo->lock_meal);
	philo->num_meal++;
	pthread_mutex_unlock(&philo->lock_meal);
	sleep_philo(philo->info->t_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

/**

	* Function that represents the routine for each philosopher
		in a multi-threaded simulation.
 *
 * arg The argument passed to the thread,
	which is a pointer to the philosopher structure.
 * void* Returns NULL when the routine is finished.
 */
void	*routine_multi_thread(void *arg)
{
	t_philo	*philo;
	t_info	*info;

	philo = (t_philo *)arg;
	info = philo->info;
	if (philo->id % 2 != 0 && info->num_of_philo != 1)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&info->stop_lock);
		if (info->stop_simulation)
		{
			pthread_mutex_unlock(&info->stop_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&info->stop_lock);
		if (is_dead(philo) == 0)
		{
			return (NULL);
		}
		_thinking(philo);
		_eating(philo);
		_sleeping(philo);
	}
	return (NULL);
}
