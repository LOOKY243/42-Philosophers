/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmarre <gmarre@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 14:19:14 by gmarre            #+#    #+#             */
/*   Updated: 2024/02/19 17:20:14 by gmarre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long int	ft_atoi(const char *str)
{
	int			i;
	int			counter;
	long int	res;

	res = 0;
	counter = 1;
	i = 0;
	while (str[i] >= 9 && str[i] <= 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			counter = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + str[i] - '0';
		i++;
	}
	if (res * counter > INT_MAX || res * counter < INT_MIN)
		return (-1);
	return (res * counter);
}

bool	is_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] < '0' || str[i] > '9') && (str[i] != '-' && str[i] != '+'))
			return (false);
		if ((str[i] == '-' || str[i] == '+') && i != 0 && str[i - 1] >= '0'
			&& str[i + 1] <= '9')
			return (false);
		i++;
	}
	return (true);
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
