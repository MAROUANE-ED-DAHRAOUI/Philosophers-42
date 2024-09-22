/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 05:49:36 by med-dahr          #+#    #+#             */
/*   Updated: 2024/09/22 06:08:05 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int check_threads(t_philo *philo);

// Function to free all allocated memory for the philosopher simulation (threads, forks, and other info structures).
void ft_free(t_philo *philo)
{
    if(philo->info->threads)
    {
        free(philo->info->threads);
        philo->info->threads = NULL;
    }
    if(philo->info->forks)
    {
        free(philo->info->forks);
        philo->info->forks = NULL;
    }
    if(philo->info)
    {
        free(philo->info);
        philo->info = NULL;
    }
    if(philo)
    {
        free(philo);
        philo = NULL;
    }
}

// Function to print an error message to the console and return an error code (1).
int write_error(char *str)
{
    printf(RED"%s\n"NC, str);
    return (1); 
}

// Function to validate the philosopher's input values and return 0 or 1 depending on their validity.
int check_values(t_philo **philo, char **av)
{
    if((*philo)->info->arg_last == 1)
        (*philo)->info->num_of_eat = atoi(av[5]);
    else
        (*philo)->info->num_of_eat = -1;

    if ((*philo)->info->num_of_philo > 200 || (*philo)->info->num_of_philo <= 0 || 
        (*philo)->info->t_to_die < 60 || (*philo)->info->t_to_eat < 60 || 
        (*philo)->info->t_to_sleep < 60 || (*philo)->info->num_of_eat == 0)
    {
        return (0);
    }
    else
        return (1);
}

// Function to initialize philosopher information, including start time and various simulation parameters.
int init_philo(t_philo *philo, char **av)
{
    int res;

    philo->info->t_start = get_time_current_ms();
    philo->info->num_of_philo = ft_atoi(av[1]);
    philo->info->t_to_die =  ft_atoi(av[2]);
    philo->info->t_to_eat = ft_atoi(av[3]);
    philo->info->t_to_sleep = ft_atoi(av[4]);
    res = check_values(&philo, av);    
    if(res == 0)
        return (0);
    else
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
    else if(len < 10)
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
    int res;

    if(ac == 5 || ac == 6)
    {
        if(ac == 6)
            philo->info->arg_last = 1;
        else
            philo->info->arg_last = 0;

        if(valide_args(ac, av) == 0)
            return (0);
        else
        {
            res = (int)init_philo(philo, av);
            if(res == 0)
                return (0);
            else  
                return (1);
        }
    }
    return (0);
}

// Function to allocate memory for philosopher threads and forks. Also initializes mutexes.
int allocate_memory(t_philo *philo)
{
    int success;

    philo->info->threads = malloc(philo->info->num_of_philo * sizeof(pthread_t));
    philo->info->forks = malloc(philo->info->num_of_philo * sizeof(pthread_mutex_t));
    if(philo->info->threads == NULL || philo->info->forks == NULL)
        return 0;

    success = init_several_mtx(philo);
    if(success == 0)
    {
        write_error("Mutex initialization failed");
        ft_free(philo);
        return 0;
    }
    return (1);
}

// Main function: Entry point of the philosopher simulation. Manages argument checking, memory allocation, and thread creation.
int main(int ac, char **av)
{
    t_philo philo;
    int res;

    res = 0;
    philo.info = NULL;
    philo.info->dead_philo = -1;
    if (ac != 5 && ac != 6)
        return (write_error("Wrong amount of arguments"));
    else
    {
        philo.info = malloc(sizeof(t_info));
        if (philo.info == NULL)
        {
            write_error("Memory allocation failed");
            ft_free(&philo);
            return (0);
        }
        res = check_args(&philo, ac, av);
        if (res == 0)
        {
            write_error("Wrong arguments");
            ft_free(&philo);
            return (0);
        }
        else
        {
           if (allocate_memory(&philo) == 0)
            {
                write_error("Memory allocation failed");
                return (0);
            }
        }
       if(check_threads(&philo) == 0)
        {
            write_error("Thread creation failed");
            ft_free(&philo);
            return (0);
        }
    }
    // ft_free(&philo);
    return (0);
}
