#include "philo.h"

int	thread_safe_print(char *str, t_philo *philo)
{
	// printf("thread_safe_print-----\n");
	 	// if(Is_dead(philo) == 0)
        //     return 0;
        pthread_mutex_lock(&philo->info->p_lock);
        if(philo->info->dead_philo == 1)
		{
            printf(str, get_current_time_ms() - philo->info->t_start, philo->id);
			pthread_mutex_unlock(&philo->info->p_lock);
			return (1);
		}
        pthread_mutex_unlock(&philo->info->p_lock);
		return (0);
}

int ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while ((n > i) && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

size_t	ft_strlen(const char *s)
{
    size_t	i;

    i = 0;
    while (s[i])
        i++;
    return (i);
}

int		ft_atoi(const char *str)
{
	int	i;
	int	nega;
	int	rlt;

	i = 0;
	nega = 1;
	rlt = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			nega = -nega;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		rlt = rlt * 10 + (str[i] - 48);
		i++;
	}
	return (rlt * nega);
}
