/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarre <gmarre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 11:38:47 by gmarre            #+#    #+#             */
/*   Updated: 2024/03/05 12:35:30 by gmarre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define INT_MAX 2147483647
# define INT_MIN -2147483648

typedef struct s_info	t_info;

typedef enum s_state
{
	SLEEPING,
	EATING,
	TOOK_FORKS,
	THINKING,
	DEAD
}						t_state;

typedef struct s_state_mutex
{
	pthread_mutex_t		lock_state;
	t_state				state;
}						t_state_mutex;

typedef struct s_fork_mutex
{
	pthread_mutex_t		lock_fork;
	bool				available;
}						t_fork_mutex;

typedef struct s_philo
{
	int					id;
	int					meals;
	t_info				*info;
	pthread_t			thread;
	t_state_mutex		state;
	t_fork_mutex		fork;
	t_fork_mutex		*s_fork;
	size_t				last_meal;

}						t_philo;

typedef struct s_info
{
	t_philo				*philo;
	int					n_philos;
	int					n_meals;
	size_t				ttd;
	size_t				tte;
	size_t				tts;
	size_t				start_time;
	bool				someone_died;
	pthread_mutex_t		lock_print;
}						t_info;

long int				ft_atoi(const char *nptr);
void					start_threads(t_info *info);
void					init_philos(t_info *info);
void					init_infos(int argc, char **argv);
bool					enough_meals(t_philo *philo);
bool					is_someone_dead(t_info *info);
void					die(t_philo *philo);
void					*routine(void *param);
bool					check_state(t_philo *philo, t_state state);
void					change_state(t_philo *philo, t_state state);
size_t					get_current_time(void);
void					think(t_philo *philo);
void					sleepy(t_philo *philo);
void					eat(t_philo *philo);
void					take_both_forks(t_philo *philo);
bool					take_own_fork(t_philo *philo);
bool					take_sfork(t_philo *philo);
bool					is_digit(char *str);
void					*ft_calloc(size_t nmemb, size_t size);

#endif
