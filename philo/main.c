/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:39:28 by med-dahr          #+#    #+#             */
/*   Updated: 2024/09/17 15:58:50 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void ft_free(t_philo *philo)
{   
    free(philo);
}

int write_error(char *str)
{
    printf("%s\n", str);
    return (1); 
}

int    check_info(t_philo **philo)
{
    if ((*philo)->data->num_of_philo > 200 || (*philo)->data->num_of_philo <= 0 || (*philo)->data->t_to_die < 60 || 
		(*philo)->data->t_to_eat < 60 || (*philo)->data->t_to_sleep < 60 || (*philo)->data->num_of_times_to_eat == 0)
        return (0);
    else
        return (1);
}

int init_philo(t_philo *philo, char **av)
{
    int res;

    philo->data->t_start = gettime();
    philo->data->num_of_philo = atoi(av[1]);
    philo->data->t_to_die =  atoi(av[2]);
    philo->data->t_to_eat = atoi(av[3]);
    philo->data->t_to_sleep = atoi(av[4]);
    res = check_info(&philo);    
    if(res == 0)
        return (0);
    else
        return (1);
}

int Is_success(char *str)
{
    int i;
    int len;
    
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
        if(ft_strncmp(str, "2147483647", 10) >= 0)
            return (0);
        else
            return (1);
    }
    return (1);
}

int valide_args(t_philo *philo, int ac, char **av)
{
    int i;
    
    i = 0;
    while(++i < ac)
    {
        if (Is_success(av[i]) == 0)
            return (0);
        i++;
    }
}

int        check_args(t_philo *philo, int ac, char **av)
{
    int res;

    if(ac == 5 || ac == 6)
    {
        if(ac == 6)
            philo->data->arg_last = 1;
        else
            philo->data->arg_last = 0;
        if(valide_args(philo, ac, av) == 0)
            return (0);
        res = (int)init_philo(philo, av);
        if(res == 0)
            return (0);
        else  
            return (1);
    }
    return (0);
}

int main(int ac, char **av)
{
    t_philo philo;
    int res;

    if (ac != 5 && ac != 6)
            return (write_error("Wrong amount of arguments"));
    else
    {
        res = check_args(&philo, ac, av);
        if (res == 0)
        {
            write_error("Wrong arguments");
            return (0);
        }
    }
    ft_free(&philo);
    return (0);
}
