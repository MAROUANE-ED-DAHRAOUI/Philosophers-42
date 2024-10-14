#ifndef PHILO_H
#define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
#include <sys/time.h>

typedef struct s_info t_info;

// Define color codes
# define BLUE "\033[0;34m"
# define RED "\033[1;31m"
# define NC "\033[0m"

typedef struct s_philo
{
    int             id;
    int             round_meals ;
    int             cnt_meals;
    pthread_t       threads;
    pthread_mutex_t *right_fork;
    pthread_mutex_t *left_fork;
    t_info          *info;
}      t_philo;

typedef struct s_info{
    long            t_start;
    long            T_last_meal;
    int             num_of_philo;
    int             t_to_die;
    int             t_to_eat;
    int             t_to_sleep;
    int             dead_philo;
    int             cnt_meals;
    int             arg_last;
    int             round_meals ;
    int             meal_eaten;
    pthread_mutex_t *forks;
    pthread_mutex_t prt_lock;
    pthread_mutex_t lock_meal;
    pthread_mutex_t lst_meal_lock;
    pthread_mutex_t dead_lock;
    pthread_mutex_t philo_dead;
    t_philo         *philos;
}       t_info;

int         ft_strncmp(const char *s1, const char *s2, size_t n);
size_t      ft_strlen(const char *s);
int         ft_atoi(const char *str);
int         write_error(char *str);
int         check_args(t_philo *philo, int ac, char **av);
int         init_philo(t_philo *philo, char **av);
long        get_current_time_ms(void);
int         check_values(t_philo **philo, char **av);
int         init_several_mtx(t_philo *philo);
int         allocate_memory(t_philo *philo);
int         valide_args(int ac, char **av);
int         Is_success(char *str);
void        ft_free(t_philo *philo);
int         Is_dead(t_philo *philo);
int         Lets_Go_Threads(t_philo *philo);
int         initialize_philos(t_philo **philo);
void        *routine_Multi_thread(void *arg);
int         _routine(t_philo *philo);
int         _sleeping(t_philo *philo);
int         unlocking_forks(t_philo *philo);
int         _eating(t_philo *philo);
int         _forks(t_philo *philo);
int         _thinking(t_philo *philo);
void        sleep_philo(int time);
void destroy_mutex(t_philo philo);

#endif