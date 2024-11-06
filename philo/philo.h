/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: med-dahr <med-dahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 22:29:03 by med-dahr          #+#    #+#             */
/*   Updated: 2024/11/06 15:24:32 by med-dahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_info	t_info;

// Define color codes
# define BLUE "\033[0;34m"
# define RED "\033[1;31m"
# define NC "\033[0m"

typedef struct s_philo
{
	int					id;
	int					num_meal;
	long long			last_meal;
	long long			t_start;
	pthread_t			threads;
	pthread_mutex_t		mutex;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		mutex_time;
	pthread_mutex_t		lock_meal;
	t_info				*info;
}						t_philo;

typedef struct s_info
{
	int					num_of_philo;
	int					t_to_die;
	int					t_to_eat;
	int					t_to_sleep;
	bool				_exit;
	bool				stop_simulation;
	int					limit_meals;
	pthread_mutex_t		stop_lock;
	pthread_mutex_t		*forks;
	pthread_mutex_t		prt_lock;
	t_philo				*philos;
}						t_info;

int						ft_strncmp(const char *s1, const char *s2, size_t n);
size_t					ft_strlen(const char *s);
int						ft_atoi(const char *str);
void					create_threads(t_philo *philo);
int						write_error(char *str);
int						check_args(t_philo *philo, int ac, char **av);
int						init_philo(t_philo *philo, char **av);
long long				get_current_time_ms(void);
int						check_values(t_philo **philo);
int						init_several_mtx(t_philo *philo);
int						allocate_memory(t_philo *philo, char **av);
int						valide_args(int ac, char **av);
int						is_success(char *str);
void					ft_free(t_philo *philo);
bool					is_dead(t_philo *philo);
int						monitor_state_philo(t_philo *philo);
int						go_threads(t_philo *philo);
int						initialize_philos(t_philo *philo);
void					*routine_multi_thread(void *arg);
void					_sleeping(t_philo *philo);
void					print_moves(t_philo *philo, char *str);
void					_eating(t_philo *philo);
void					_thinking(t_philo *philo);
int						one_thread(t_philo *philo);
void					sleep_philo(int time);
int						one_thread(t_philo *philo);
int						state_philos(t_philo *philo);
void					stop_all_philosophers(t_info *info);
int						check_philosopher_state(t_philo *philo,
							t_philo *current_philo);
int						check_philosopher_meals(t_philo *philo,
							t_philo *current_philo, int *finished_philosophers);
int						end_simulation_if_all_finished(t_philo *philo,
							int finished_philosophers);

#endif