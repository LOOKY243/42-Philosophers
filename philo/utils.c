/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarre <gmarre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 14:19:14 by gmarre            #+#    #+#             */
/*   Updated: 2024/02/11 00:28:48 by gmarre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *nptr)
{
	int	res;
	int	i;
	int	sign;

	res = 0;
	i = 0;
	sign = 1;
	while ((nptr[i] >= 7 && nptr[i] <= 13) || nptr[i] == 32)
	{
		i++;
	}
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		res = res * 10 + (nptr[i] - '0');
		i++;
	}
	return (res * sign);
}

size_t	get_current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	change_state(t_philo *philo, t_state state)
{
	pthread_mutex_lock(&philo->state.lock_state);
	philo->state.state = state;
	pthread_mutex_unlock(&philo->state.lock_state);
}

bool	check_state(t_philo *philo, t_state state)
{
	bool	value;

	pthread_mutex_lock(&philo->state.lock_state);
	value = philo->state.state == state;
	pthread_mutex_unlock(&philo->state.lock_state);
	return (value);
}
