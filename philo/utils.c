/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:39:09 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/02/06 17:29:35 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_action(t_philo *philo, char *message)
{
	t_all	*params;

	params = philo->params;
	pthread_mutex_lock(&params->dead_flag);
	if (!philo->params->dead)
	{
		pthread_mutex_lock(&params->print_mutex);
		printf("%ld %d %s\n", get_time_ms() - params->start_time,
			philo->id, message);
		pthread_mutex_unlock(&params->print_mutex);
	}
	pthread_mutex_unlock(&params->dead_flag);
}

int	precise_usleep(int millisecs)
{
	long	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < millisecs)
		usleep(500);
	return (0);
}

void	cleanup_mutexes(t_all *params)
{
	int	i;

	i = -1;
	pthread_mutex_destroy(&params->dead_flag);
	pthread_mutex_destroy(&params->print_mutex);
	while (++i < params->no_philos)
		pthread_mutex_destroy(&params->forks[i]);
}

void	all_cleanup(t_all *params)
{
	int	i;

	i = -1;
	cleanup_mutexes(params);
	if (params->t_philo)
	{
		while (++i < params->no_philos)
			pthread_mutex_destroy(&params->t_philo[i].meal_lock);
		free(params->t_philo);
		params->t_philo = NULL;
	}
	if (params->forks)
	{
		free(params->forks);
		params->forks = NULL;
	}
}
