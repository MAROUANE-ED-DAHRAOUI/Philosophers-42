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
# define NC "\033[0m" // No Color

typedef struct s_philo
{
    int             id;
    int             num_of_eat;
    long            last_time_eat;
    int             right_fork;
    int             left_fork;
    t_info          *info;
}      t_philo;

typedef struct s_info{

    pthread_t       *threads;
    long            t_start;
    int             num_of_philo;
    int             t_to_die;
    int             t_to_eat;
    int             t_to_sleep;
    int             num_of_eat;
    int             dead_philo;
    int             arg_last;
    pthread_mutex_t *forks;
    pthread_mutex_t p_lock;
    pthread_mutex_t t_check;
    pthread_mutex_t t_success;
    t_philo         *philos;
}   t_info;

int         ft_strncmp(const char *s1, const char *s2, size_t n);
size_t      ft_strlen(const char *s);
int         ft_atoi(const char *str);
int         write_error(char *str);
int         check_args(t_philo *philo, int ac, char **av);
int         init_philo(t_philo *philo, char **av);
long        get_current_time_ms(void);
int         check_values(t_philo **philo, char **av);
int         check_threads(t_philo *philo);
int         init_several_mtx(t_philo *philo);
int         allocate_memory(t_philo *philo);
int         valide_args(int ac, char **av);
int         Is_success(char *str);
void        ft_free(t_philo **philo);
int         Is_dead(t_philo *philo);
void        *routine_one_thread(void *arg);
int         check_threads(t_philo *philo);
int         Multi_Threads(t_philo *philo);
int         Check_Philos_State(t_philo **Philos);
int         thread_safe_print(char *str, t_philo *philo);
int         check_take_forks(t_philo *philo);
int         philos_alternates(t_philo *philo);
void        *Routine_Multi_Threads(void *arg);

#endif