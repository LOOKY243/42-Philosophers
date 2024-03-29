/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarre <gmarre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 00:08:35 by gmarre            #+#    #+#             */
/*   Updated: 2024/03/05 12:41:11 by gmarre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	think(t_philo *philo)
{
	if (is_someone_dead(philo->info))
		return ;
	pthread_mutex_lock(&philo->info->lock_print);
	printf("\033[1;37m%lu \033[1;36m%d is thinking\n\033[0m", get_current_time()
		- philo->info->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->info->lock_print);
	change_state(philo, THINKING);
}

void	sleepy(t_philo *philo)
{
	if (is_someone_dead(philo->info))
		return ;
	if (get_current_time() - philo->last_meal >= philo->info->tte
		+ philo->info->tts)
		change_state(philo, SLEEPING);
	if (philo->info->n_philos % 2)
		usleep(philo->info->tts);
}

void	eat(t_philo *philo)
{
	if (is_someone_dead(philo->info))
		return ;
	if (get_current_time() - philo->last_meal >= philo->info->tte)
	{
		philo->meals++;
		pthread_mutex_lock(&philo->fork.lock_fork);
		philo->fork.available = true;
		pthread_mutex_unlock(&philo->fork.lock_fork);
		pthread_mutex_lock(&philo->s_fork->lock_fork);
		philo->s_fork->available = true;
		pthread_mutex_unlock(&philo->s_fork->lock_fork);
		if (enough_meals(philo))
			return ;
		change_state(philo, EATING);
		pthread_mutex_lock(&philo->info->lock_print);
		printf("\033[1;37m%lu \033[1;34m%d is sleeping\n\033[0m",
			get_current_time() - philo->info->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->info->lock_print);
	}
}

void	take_both_forks(t_philo *philo)
{
	if (is_someone_dead(philo->info))
		return ;
	if (take_own_fork(philo))
	{
		if (take_sfork(philo))
		{
			pthread_mutex_lock(&philo->info->lock_print);
			printf("\033[1;37m%lu \033[1;32m%d is eating\n\033[0m",
				get_current_time() - philo->info->start_time, philo->id + 1);
			pthread_mutex_unlock(&philo->info->lock_print);
			change_state(philo, TOOK_FORKS);
			philo->last_meal = get_current_time();
		}
		else
		{
			pthread_mutex_lock(&philo->fork.lock_fork);
			philo->fork.available = true;
			pthread_mutex_unlock(&philo->fork.lock_fork);
		}
	}
}
