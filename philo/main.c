/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 05:49:36 by med-dahr          #+#    #+#             */
/*   Updated: 2024/11/06 00:51:34 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	* Function to initialize philosopher information, including start
	time and various simulation parameters.
*/
int	init_philo(t_philo *philo, char **av)
{
	if (allocate_memory(philo, av) == 0)
		return (0);
	philo->info->num_of_philo = ft_atoi(av[1]);
	philo->info->t_to_die = ft_atoi(av[2]);
	philo->info->t_to_eat = ft_atoi(av[3]);
	philo->info->t_to_sleep = ft_atoi(av[4]);
	philo->t_start = get_current_time_ms();
	philo->last_meal = get_current_time_ms();
	philo->id = -1;
	philo->info->_exit = true;
	philo->info->stop_simulation = false;
	if (av[5] != NULL)
		philo->info->limit_meals = ft_atoi(av[5]);
	else
		philo->info->limit_meals = -1;
	if (init_several_mtx(philo) == 0)
	{
		write_error("Mutex initialization failed");
		ft_free(philo);
		return (0);
	}
	if (!check_values(&philo))
		return (0);
	return (1);
}

/*
	* Function to check if an argument string is a valid integer
			(within bounds of int).
*/
int	is_success(char *str)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(str);
	while (str[i] && (str[i] == '0' || str[i] == '+'))
	{
		len--;
		i++;
	}
	if (len > 10)
		return (0);
	else if (len < 10 && len > 0)
		return (1);
	else if (len == 10)
	{
		if (ft_strncmp(str, "2147483647", 10) > 0)
			return (0);
		else
			return (1);
	}
	return (1);
}

// Function to validate all command line arguments passed to the program.
int	valide_args(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (is_success(av[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

/*
	* Function to check command-line arguments and initialize philosopher
			data based on those arguments.
*/
int	check_args(t_philo *philo, int ac, char **av)
{
	if (ac == 5 || ac == 6)
	{
		if (valide_args(ac, av) == 0)
			return (0);
		else
		{
			if (init_philo(philo, av))
				return (1);
			else
				return (0);
		}
	}
	return (0);
}

/*
	* Main function: Entry point of the philosopher simulation. Manages 
	argument checking, memory allocation, and thread creation.
*/
int	main(int ac, char **av)
{
	t_philo	philo;

	if (ac != 5 && ac != 6)
		return (write_error("Wrong number of arguments"));
	philo.info = malloc(sizeof(t_info));
	if (philo.info == NULL)
		return (write_error("Memory allocation failed"));
	if (!check_args(&philo, ac, av))
	{
		free(philo.info);
		return (write_error("Invalid arguments"));
	}
	if (!initialize_philos(&philo))
	{
		free(philo.info);
		return (0);
	}
	create_threads(&philo);
	if (!go_threads(&philo))
	{
		ft_free(&philo);
		return (0);
	}
	free(philo.info);
	return (0);
}
