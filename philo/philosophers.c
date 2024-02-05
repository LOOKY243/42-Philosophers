/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarre <gmarre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 11:57:24 by gmarre            #+#    #+#             */
/*   Updated: 2024/02/05 12:20:30 by gmarre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

ull compare_time(struct timeval time)
{
	struct timeval current;
	long long result_sec;
	long long result_usec;

	gettimeofday(&current, NULL);
	result_sec = current.tv_sec - time.tv_sec;
	result_usec = current.tv_usec - time.tv_usec;
	return ((result_sec * 1000000) + result_usec);
}

ull	get_current_time(struct timeval time)
{
	struct timeval current;
	long long result_sec;
	long long result_usec;

	gettimeofday(&current, NULL);
	result_sec = current.tv_sec - time.tv_sec;
	result_usec = current.tv_usec - time.tv_usec;
	return ((result_sec * 1000) + (result_usec / 1000));
}

int die(t_philo *philo)
{
	if (compare_time(philo->last_meal) >= philo->info->t_die)
	{
		printf("\033[1;37m%llu \033[1;31m%d died\n", get_current_time(philo->info->time), philo->id + 1);
		pthread_mutex_lock(&philo->state.lock_state);
		philo->state.state = DEAD;
		pthread_mutex_unlock(&philo->state.lock_state);
		return (1);
	}
	return (0);
}

bool	take_fork(t_philo *philo, int id)
{
	bool	available;

	pthread_mutex_lock(&philo->fork.fork);
	available = philo->fork.available;
	if (id == philo->info->n_philo - 1)
	{
		philo->s_fork = &philo->info->philo[0].fork;
		pthread_mutex_lock(&philo->s_fork->fork);
		available = available && philo->s_fork->available;
	}
	else
	{
		philo->s_fork = &philo->info->philo[id + 1].fork;
		pthread_mutex_lock(&philo->s_fork->fork);
		available = available && philo->s_fork->available;
	}
	return (available);
}

void	eat(t_philo *philo, int id)
{
	if (philo->state.state == THINKING)
	{
		if (take_fork(philo, id))
		{
			printf("\033[1;37m%llu \033[1;35m%d has taken a fork\n", get_current_time(philo->info->time), philo->id + 1);
			printf("\033[1;37m%llu \033[1;32m%d is eating\n", get_current_time(philo->info->time), philo->id + 1);
			gettimeofday(&philo->last_meal, NULL);
			philo->fork.available = false;
			philo->s_fork->available = false;
			pthread_mutex_lock(&philo->state.lock_state);
			philo->state.state = TOOK_FORK;
			pthread_mutex_unlock(&philo->state.lock_state);
		}
		pthread_mutex_unlock(&philo->fork.fork);
		pthread_mutex_unlock(&philo->s_fork->fork);
	}
	else
	{
		if (compare_time(philo->last_meal) >= philo->info->t_eat)
		{
			pthread_mutex_lock(&philo->meals.lock_meals);
			philo->meals.meals++;
			pthread_mutex_unlock(&philo->meals.lock_meals);
			pthread_mutex_lock(&philo->state.lock_state);
			philo->state.state = ATE;
			pthread_mutex_unlock(&philo->state.lock_state);
			pthread_mutex_lock(&philo->fork.fork);
			philo->fork.available = true;
			pthread_mutex_unlock(&philo->fork.fork);
			pthread_mutex_lock(&philo->s_fork->fork);
			philo->s_fork->available = true;
			pthread_mutex_unlock(&philo->s_fork->fork);
			philo->s_fork = NULL;
			printf("\033[1;37m%llu \033[1;34m%d is sleeping\n", get_current_time(philo->info->time), philo->id + 1);
		}
	}
}

void	philo_sleep(t_philo *philo)
{
	if (compare_time(philo->last_meal) >= philo->info->t_sleep + philo->info->t_eat)
	{
		pthread_mutex_lock(&philo->state.lock_state);
		philo->state.state = SLEPT;
		pthread_mutex_unlock(&philo->state.lock_state);
	}
}


void	think(t_philo *philo)
{
	pthread_mutex_lock(&philo->state.lock_state);
	philo->state.state = THINKING;
	pthread_mutex_unlock(&philo->state.lock_state);
	printf("\033[1;37m%llu \033[1;36m%d is thinking\n", get_current_time(philo->info->time), philo->id + 1);
}

bool is_someone_dead(t_philo philo)
{
	int	i;

	i = -1;
	while (++i < philo.info->n_philo)
	{
		pthread_mutex_lock(&philo.info->philo[i].state.lock_state);
		if (philo.info->philo[i].state.state == DEAD)
		{
			pthread_mutex_unlock(&philo.info->philo[i].state.lock_state);
			return (true);
		}
		pthread_mutex_unlock(&philo.info->philo[i].state.lock_state);
	}
	return (false);
}

bool is_stomach_full(t_philo philo)
{
	int	i;

 	if (philo.info->n_eat == -1)
 		return (false);
	i = -1;
	while (++i < philo.info->n_philo)
	{
		pthread_mutex_lock(&philo.info->philo[i].meals.lock_meals);
		if (philo.info->philo[i].meals.meals < philo.info->n_eat)
		{
			pthread_mutex_unlock(&philo.info->philo[i].meals.lock_meals);
			return (false);
		}
		pthread_mutex_unlock(&philo.info->philo[i].meals.lock_meals);
	}
	return (true);
}

void    *philosophers(void *param)
{
	t_philo *philo;
	int		i;
	
	philo = param;
	i = -1;
	if (philo->info->n_philo <= 1)
	{
		usleep(philo->info->t_die);
		printf("\033[1;37m%llu \033[1;31m%d died\n", get_current_time(philo->info->time), philo->id + 1);
		return (NULL);
	}
    while (!is_someone_dead(*philo) && !is_stomach_full(*philo))
	{
		if (philo->state.state == THINKING || philo->state.state == TOOK_FORK)
			eat(philo, philo->id);
		if (philo->state.state == ATE)
			philo_sleep(philo);
		if (philo->state.state == SLEPT)
			think(philo);
		die(philo);
	}
	return (NULL);
}

void	init_philo(t_info *info, int n_philo)
{
	int	i;

	i = -1;
	info->philo = malloc(n_philo * sizeof(t_philo));
	if (!info->philo)
		return ;
	while (++i < n_philo)
	{
		info->philo[i].id = i;
		info->philo[i].state.state = THINKING;
		if (i % 2 != 0)
			info->philo[i].state.state = SLEPT;
		pthread_mutex_init(&info->philo[i].state.lock_state, NULL);
		info->philo[i].info = info;
		info->philo[i].meals.meals = 0;
		pthread_mutex_init(&info->philo[i].fork.fork, NULL);
		info->philo[i].fork.available = true;
		gettimeofday(&info->philo[i].last_meal, NULL);
		printf("\033[1;37m%llu \033[1;36m%d is thinking\n", get_current_time(info->time), info->philo[i].id + 1);
	}
	i = -1;
	while (++i < n_philo)
		pthread_create(&info->philo[i].thread, NULL, philosophers, &info->philo[i]);
}

void	init_info(t_info *info, char **argv, int argc)
{
	info->n_philo = ft_atoi(argv[1]);
	info->t_die = ft_atoi(argv[2]) * 1000;
	info->t_eat = ft_atoi(argv[3]) * 1000;
	info->t_sleep = ft_atoi(argv[4]) * 1000;
	info->n_eat = -1;
	if (argc == 6)
		info->n_eat = ft_atoi(argv[5]);
	gettimeofday(&info->time, NULL);
	init_philo(info, ft_atoi(argv[1]));
}

int main(int argc, char **argv)
{
    int     i;
	t_info	info;
	
	if (argc < 5 || argc > 6)
		return (EXIT_FAILURE);
	init_info(&info, argv, argc);
	i = -1;
	while (++i < ft_atoi(argv[1]))
		pthread_join(info.philo[i].thread, NULL);
	return (EXIT_SUCCESS);
}