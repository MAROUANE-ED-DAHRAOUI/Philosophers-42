#ifndef PHILO_H
#define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
#include <sys/time.h>
#include <limits.h>

typedef struct s_info t_info;

// Define color codes
# define BLUE "\033[0;34m"
# define RED "\033[1;31m"
# define NC "\033[0m"

typedef struct s_philo
{
    int             id;
    int             cnt_meals;
    int             num_meal;
    long long       last_meal;
    long long       t_start;
    pthread_t       threads;
    pthread_mutex_t meal_mutex;
    pthread_mutex_t mutex;
    pthread_mutex_t *right_fork;
    pthread_mutex_t *left_fork;
    pthread_mutex_t mutex_time;
    pthread_mutex_t lock_meal;
    t_info          *info;
}      t_philo;

typedef struct s_info{
    int             dead_id;
    int             num_of_philo;
    int             t_to_die;
    int             t_to_eat;
    int             t_to_sleep;
    int             dead_philo;
    int             cnt_meals;
    bool            _exit;
    int             limit_meals ;
    pthread_mutex_t *forks;
    pthread_mutex_t prt_lock;
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
long long   get_current_time_ms(void);
int         check_values(t_philo **philo);
int         init_several_mtx(t_philo *philo);
int         allocate_memory(t_philo *philo, char **av);
int         valide_args(int ac, char **av);
int         Is_success(char *str);
void        ft_free(t_philo *philo);
int         Is_dead(t_philo *philo);
int         monitor_state_philo(t_philo *philo);
int         Lets_Go_Threads(t_philo *philo);
int         initialize_philos(t_philo *philo);
void        *routine_Multi_thread(void *arg);
void         _sleeping(t_philo *philo);
int         unlocking_forks(t_philo *philo);
void        print_moves(t_philo *philo, char *str);
void         _eating(t_philo *philo);
void        _thinking(t_philo *philo);
int         One_thread(t_philo *philo);
void        sleep_philo(int time);
int         One_thread(t_philo *philo);

#endif