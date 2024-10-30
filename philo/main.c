/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 05:49:36 by med-dahr          #+#    #+#             */
/*   Updated: 2024/10/28 13:45:40 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Function to free all allocated memory for the philosopher simulation
    (threads, forks, and other info structures).
*/
void ft_free(t_philo *philo)
{
    int i;

    i = 0;
    while(i < philo->info->num_of_philo)
    {
       pthread_mutex_destroy(&philo->info->forks[i]);
       pthread_mutex_destroy(&philo->info->philos[i].lock_meal);
       pthread_mutex_destroy(&philo->info->philos[i].mutex_time);
       pthread_mutex_destroy(&philo[i].meal_mutex);
       pthread_mutex_destroy(&philo[i].mutex);
       i++;
    }
    pthread_mutex_destroy(&philo->info->prt_lock);
    pthread_mutex_destroy(&philo->info->philo_dead);
    free(philo->info->forks);
    free(philo->info->philos);
    // free(philo->info);
}

// Function to print an error message to the console and return an error code (1).
int write_error(char *str)
{
    printf(RED"%s\n"NC, str);
    return (1); 
}

// Function to validate the philosopher's input values and return 0 or 1 depending on their validity.
int check_values(t_philo **philo)
{
    if ((*philo)->info->num_of_philo > 200 || (*philo)->info->num_of_philo <= 0 || 
        (*philo)->info->t_to_die < 60 || (*philo)->info->t_to_eat < 60 || 
        (*philo)->info->t_to_sleep < 60)
    {
        return (0);
    }
    else
        return (1);
}

// Function to initialize philosopher information, including start time and various simulation parameters.
int init_philo(t_philo *philo, char **av)
{
    if (allocate_memory(philo, av) == 0)
    {
            write_error("Memory allocation failed");
            return (0);
    }
    philo->info->num_of_philo = ft_atoi(av[1]);
    philo->info->t_to_die =  ft_atoi(av[2]);
    philo->info->t_to_eat = ft_atoi(av[3]);
    philo->info->t_to_sleep = ft_atoi(av[4]);
    philo->t_start = get_current_time_ms();
    philo->last_meal = get_current_time_ms();
    philo->id = -1;
    philo->info->_exit = true;
    philo->info->dead_id = -1;
    if(av[5] != NULL)
        philo->info->limit_meals = ft_atoi(av[5]);
    else
        philo->info->limit_meals = -1;
    if (init_several_mtx(philo) == 0)
    {
        write_error("Mutex initialization failed");
        ft_free(philo);
        return 0;
    }
    if(!check_values(&philo))
        return (0);
    return (1);
}

// Function to check if an argument string is a valid integer (within bounds of int).
int Is_success(char *str)
{
    int i;
    size_t len;
    
    i = 0;
    len = ft_strlen(str);
    while(str[i] && (str[i] == '0' || str[i] == '+'))
    {
        len--;
        i++;
    }
    if(len > 10)
        return (0);
    else if(len < 10 && len > 0)
        return (1);
    else if(len == 10)
    {
        if(ft_strncmp(str, "2147483647", 10) > 0)
            return (0);
        else
            return (1);
    }
    return (1);
}

// Function to validate all command line arguments passed to the program.
int valide_args(int ac, char **av)
{
    int i;
    
    i = 1;
    while(i < ac)
    {
        if (Is_success(av[i]) == 0)
            return (0);
        i++;
    }
    return (1);
}

// Function to check command-line arguments and initialize philosopher data based on those arguments.
int check_args(t_philo *philo, int ac, char **av)
{
    if(ac == 5 || ac == 6)
    {
        if(valide_args(ac, av) == 0)
            return (0);
        else
        {
            if(init_philo(philo, av))
                return (1);
            else
                return (0);
        }
    }
    return (0);
}

// Function to allocate memory for philosopher threads and forks. Also initializes mutexes.
int allocate_memory(t_philo *philo, char **av)
{
    int num_philos = ft_atoi(av[1]);
    philo->info->forks = malloc(sizeof(pthread_mutex_t) * num_philos);
    if (philo->info->forks == NULL) {
        fprintf(stderr, "Error: Memory allocation for forks failed\n");
        return 0;
    }
    philo->info->philos = malloc(sizeof(t_philo) * num_philos);
    if (philo->info->philos == NULL) {
        fprintf(stderr, "Error: Memory allocation for philos failed\n");
        free(philo->info->forks); // Free previously allocated memory
        return 0;
    }
    return 1;
}

// Main function: Entry point of the philosopher simulation. Manages argument checking, memory allocation, and thread creation.
int main(int ac, char **av)
{
    t_philo philo;

    if (ac != 5 && ac != 6)
        return write_error("Wrong number of arguments");

    philo.info = malloc(sizeof(t_info));
    if (philo.info == NULL)
        return write_error("Memory allocation failed");

    if (!check_args(&philo, ac, av))
    {
        free(philo.info);
        return write_error("Invalid arguments");
    }

    if (!initialize_philos(&philo)) {
        free(philo.info);
        return 0;
    }
    if(Lets_Go_Threads(&philo) == 0)
    {
        ft_free(&philo);
        return 0;
    }
    free(philo.info);
    return 0;
}

