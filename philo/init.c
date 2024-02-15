/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarre <gmarre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 00:17:36 by gmarre            #+#    #+#             */
/*   Updated: 2024/02/15 15:26:17 by gmarre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	start_threads(t_info *info)
{
	int	i;
	int	j;
	int	groups;

	if (info->n_philos % 2)
		groups = 3;
	else
		groups = 2;
	i = 0;
	while (i < groups)
	{
		j = i;
		while (j < info->n_philos)
		{
			pthread_create(&info->philo[j].thread, NULL, routine,
				&info->philo[j]);
			j += groups;
		}
		i++;
		usleep(100);
	}
}

void	init_philos(t_info *info)
{
	int	i;

	i = -1;
	while (++i < info->n_philos)
	{
		info->philo[i].id = i;
		info->philo[i].meals = 0;
		info->philo[i].last_meal = get_current_time();
		info->philo[i].fork.available = true;
		pthread_mutex_init(&info->philo->fork.lock_fork, NULL);
		pthread_mutex_init(&info->philo->state.lock_state, NULL);
		info->philo[i].info = info;
		info->philo[i].state.state = THINKING;
	}
	start_threads(info);
	i = -1;
	while (++i < info->n_philos)
		pthread_join(info->philo[i].thread, NULL);
}

void	init_infos(int argc, char **argv)
{
	t_info	info;

	info.philo = ft_calloc(ft_atoi(argv[1]), sizeof(t_philo));
	if (!info.philo)
		return ;
	info.start_time = get_current_time();
	info.n_philos = ft_atoi(argv[1]);
	info.ttd = ft_atoi(argv[2]);
	info.tte = ft_atoi(argv[3]);
	info.tts = ft_atoi(argv[4]);
	info.n_meals = -1;
	if (argc == 6)
		info.n_meals = ft_atoi(argv[5]);
	init_philos(&info);
	free(info.philo);
}
