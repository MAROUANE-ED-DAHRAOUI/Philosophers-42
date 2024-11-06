/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _helper.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 22:28:55 by med-dahr          #+#    #+#             */
/*   Updated: 2024/11/06 13:26:10 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Function to free all allocated memory for the philosopher simulation
	(threads, forks, and other info structures).
*/
void	ft_free(t_philo *philo)
{
	int	i;

	(void)philo;
	if (!philo || !philo->info)
		return ;
	i = 0;
	while (i < philo->info->num_of_philo)
	{
		pthread_mutex_destroy(&philo->info->forks[i]);
		pthread_mutex_destroy(&philo->info->philos[i].lock_meal);
		pthread_mutex_destroy(&philo->info->philos[i].mutex_time);
		pthread_mutex_destroy(&philo->info->philos[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&philo->info->prt_lock);
}

/*
	* Function to print an error message to the console and
		return an error code (1).
*/
int	write_error(char *str)
{
	printf(RED "%s\n" NC, str);
	return (1);
}

void	create_threads(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->info->num_of_philo)
	{
		if (pthread_create(&philo->info->philos[i].threads, NULL,
				&routine_multi_thread, &philo->info->philos[i]) != 0)
		{
			write_error("Thread creation failed");
			ft_free(philo);
			return ;
		}
		i++;
	}
}

int	allocate_memory(t_philo *philo, char **av)
{
	int	num_philos;

	num_philos = ft_atoi(av[1]);
	philo->info->forks = malloc(sizeof(pthread_mutex_t) * num_philos);
	if (philo->info->forks == NULL)
	{
		fprintf(stderr, "Error: Memory allocation for forks failed\n");
		return (0);
	}
	philo->info->philos = malloc(sizeof(t_philo) * num_philos);
	if (philo->info->philos == NULL)
	{
		fprintf(stderr, "Error: Memory allocation for philos failed\n");
		free(philo->info->forks);
		return (0);
	}
	return (1);
}

/*
	* Function to validate the philosopher's input values and
		return 0 or 1 depending on their validity.
*/
int	check_values(t_philo **philo)
{
	if ((*philo)->info->num_of_philo > 200 || (*philo)->info->num_of_philo <= 0
		|| (*philo)->info->t_to_die < 60 || (*philo)->info->t_to_eat < 60
		|| (*philo)->info->t_to_sleep < 60)
	{
		return (0);
	}
	else
		return (1);
}
