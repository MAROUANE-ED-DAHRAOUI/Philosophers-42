#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

// Declare the gettime function prototype
long gettime(void);
typedef struct s_data t_data;

typedef struct s_philo
{
    int             id;
    int             num_of_times_to_eat;
    int             last_time_eat;
    pthread_t       threads;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_data          *data;
}      t_philo;

typedef struct s_data{

    pthread_t       *threads;
    long            t_start;
    int             num_of_philo;
    int             t_to_die;
    int             t_to_eat;
    int             t_to_sleep;
    int             num_of_times_to_eat;
    int             t_dead;
    int             arg_last;
    int             dead;
    t_philo         *philo;
    pthread_mutex_t *forks;
    pthread_mutex_t print_lock;
    pthread_mutex_t check_dead;
}   t_data;

int         write_error(char *str); 
int         check_args(t_philo *philo, int ac, char **av);
int         init_philo(t_philo *philo, char **av);
int         check_info(t_philo **philo);

#endif