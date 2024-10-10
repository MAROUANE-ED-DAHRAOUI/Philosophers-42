/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 05:49:36 by med-dahr          #+#    #+#             */
/*   Updated: 2024/09/28 19:50:59 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Function to free all allocated memory for the philosopher simulation
    (threads, forks, and other info structures).
*/
void ft_free(t_philo *philo)
{
    if(philo->info->forks != NULL)
        free(philo->info->forks);
    if(philo->info->philos != NULL)
        free(philo->info->philos);
    free(philo->info);
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
    {
        (*philo)->info->round_meals = ft_atoi(av[5]);
        (*philo)->info->cnt_meals = 0;
        if((*philo)->info->round_meals <= 0)
            return (0);
    }
    else
    {
        (*philo)->info->round_meals  = -1;
        (*philo)->info->cnt_meals = 0;
    }
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
    int res;
    
    philo->info->num_of_philo = ft_atoi(av[1]);
    philo->info->t_to_die =  ft_atoi(av[2]);
    philo->info->t_to_eat = ft_atoi(av[3]);
    philo->info->t_to_sleep = ft_atoi(av[4]);
    philo->info->t_start = get_current_time_ms();
    philo->info->T_last_meal = get_current_time_ms();
    philo->info->dead_philo = 0;
    philo->info->round_meals  = 0;
    philo->info->cnt_meals = 0;
    // printf(BLUE"Number of philosophers = %d\n"NC, philo->info->num_of_philo);
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
    philo->info->forks = malloc(sizeof(pthread_mutex_t) * philo->info->num_of_philo);
    if(philo->info->forks == NULL)
        return 0;
    philo->info->philos = malloc(sizeof(t_philo) * philo->info->num_of_philo);
    if(philo->info->philos == NULL)
        return 0;
    return (1);
}

void    destroy_mutex(t_philo philo)
{
    int i;

    i = 0;
    pthread_mutex_destroy(&philo.info->prt_lock);
    pthread_mutex_destroy(&philo.info->lock_meal);
    pthread_mutex_destroy(&philo.info->dead_lock);
    pthread_mutex_destroy(&philo.info->philo_dead);
    pthread_mutex_destroy(&philo.info->lst_meal_lock);
    while(i < philo.info->num_of_philo)
    {
        pthread_mutex_destroy(&philo.info->forks[i]);
        i++;
    }
    ft_free(&philo);
}

// Main function: Entry point of the philosopher simulation. Manages argument checking, memory allocation, and thread creation.
int main(int ac, char **av)
{
    t_philo philo;
    int res;

    res = 0;
    philo.info = NULL;
    if (ac != 5 && ac != 6)
        return (write_error("Wrong amount of arguments"));
    else
    {
        philo.info = malloc(sizeof(t_info));
        if (philo.info == NULL)
        {
            write_error("Memory allocation failed");
            free(philo.info);
            return (0);
        }
        res = check_args(&philo, ac, av);
        if (res == 0)
        {
            write_error("Wrong arguments");
            free(philo.info);
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
    }

    if(Lets_Go_Threads(&philo) == 0)
    {
        write_error("Thread creation failed");
        free(philo.info);
        return (0);
    }
    destroy_mutex(philo);
    return (0);
}
