/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarre <gmarre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:58:20 by gmarre            #+#    #+#             */
/*   Updated: 2024/02/02 12:53:32 by gmarre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>

typedef unsigned long long ull;

typedef enum e_state {
	THINKING,
	SLEPT,
	ATE,
	TOOK_FORK,
	DEAD
}	t_state;

typedef struct s_mutex_state {
	t_state	state;
	pthread_mutex_t lock_state;
}	t_mutex_state;

typedef struct s_mutex_meals {
	int meals;
	pthread_mutex_t lock_meals;
} t_mutex_meals;

typedef struct s_info t_info;

typedef struct s_fork {
	pthread_mutex_t			fork;
	bool					available;
} t_fork;

typedef struct s_philo {
	int			id;
	t_mutex_meals		meals;
	t_mutex_state		state;
	pthread_t	thread;
	struct timeval	last_meal;
	struct timeval	start_sleep;
	t_fork			*s_fork;
	t_fork			fork;
	t_info	*info;
} t_philo;

typedef struct s_info {
	t_philo *philo;
    ull t_eat;
	ull t_sleep;
    ull t_die;
	int n_eat;
	int	n_philo;
	struct timeval time;
}   t_info;


int	ft_atoi(const char *nptr);

#endif