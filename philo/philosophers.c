/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarre <gmarre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 11:42:31 by gmarre            #+#    #+#             */
/*   Updated: 2024/02/14 12:45:06 by gmarre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	take_own_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->fork.lock_fork);
	if (philo->fork.available)
	{
		philo->fork.available = false;
		pthread_mutex_unlock(&philo->fork.lock_fork);
		return (true);
	}
	pthread_mutex_unlock(&philo->fork.lock_fork);
	return (false);
}

bool	take_sfork(t_philo *philo)
{
	int	id;

	id = (philo->id + 1) % philo->info->n_philos;
	philo->s_fork = &philo->info->philo[id].fork;
	pthread_mutex_lock(&philo->s_fork->lock_fork);
	if (philo->s_fork->available)
	{
		philo->s_fork->available = false;
		printf("\033[1;37m%lu \033[1;35m%d has taken a fork\n\033[0m",
			get_current_time() - philo->info->start_time, philo->id + 1);
		pthread_mutex_unlock(&philo->s_fork->lock_fork);
		return (true);
	}
	pthread_mutex_unlock(&philo->s_fork->lock_fork);
	return (false);
}

bool	parsing(int argc, char **argv)
{
	if (!is_digit(argv[1]) || ft_atoi(argv[1]) <= 0)
		return (false);
	if (!is_digit(argv[2]) || ft_atoi(argv[2]) < 0)
		return (false);
	if (!is_digit(argv[3]) || ft_atoi(argv[3]) < 0)
		return (false);
	if (!is_digit(argv[4]) || ft_atoi(argv[4]) < 0)
		return (false);
	if (argc == 6)
		if (!is_digit(argv[5]) || ft_atoi(argv[5]) < 0)
			return (false);
	return (true);
}

int	main(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (EXIT_FAILURE);
	if (!parsing(argc, argv))
	{
		printf("\033[1;31mInvalid arguments!\n\033[0m");
		return (EXIT_FAILURE);
	}
	init_infos(argc, argv);
	return (EXIT_SUCCESS);
}
