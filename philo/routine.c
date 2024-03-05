/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarre <gmarre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 00:14:52 by gmarre            #+#    #+#             */
/*   Updated: 2024/03/05 12:42:49 by gmarre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*routine(void *param)
{
	t_philo	*philo;

	philo = (t_philo *)param;
	pthread_mutex_lock(&philo->info->lock_print);
	printf("\033[1;37m%lu \033[1;36m%d is thinking\n\033[0m", get_current_time()
		- philo->info->start_time, philo->id + 1);
	pthread_mutex_unlock(&philo->info->lock_print);
	while (!is_someone_dead(philo->info) && !enough_meals(philo))
	{
		if (check_state(philo, THINKING))
			take_both_forks(philo);
		if (check_state(philo, TOOK_FORKS))
			eat(philo);
		if (check_state(philo, EATING))
			sleepy(philo);
		if (check_state(philo, SLEEPING))
			think(philo);
		die(philo);
		usleep(5);
	}
	return (NULL);
}

void	die(t_philo *philo)
{
	if (philo->info->ttd < get_current_time() - philo->last_meal)
	{
		if (!is_someone_dead(philo->info))
		{
			change_state(philo, DEAD);
			pthread_mutex_lock(&philo->info->lock_print);
			printf("\033[1;37m%lu \033[1;31m%d died\n\033[0m",
				get_current_time() - philo->info->start_time,
				philo->id + 1);
			pthread_mutex_unlock(&philo->info->lock_print);
		}
	}
}

bool	is_someone_dead(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->n_philos)
	{
		if (check_state(&info->philo[i], DEAD))
			return (true);
		i++;
	}
	return (false);
}

bool	enough_meals(t_philo *philo)
{
	if (philo->info->n_meals == -1)
		return (false);
	if (philo->meals >= philo->info->n_meals)
		return (true);
	return (false);
}
