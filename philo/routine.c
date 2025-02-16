/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:38:05 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/02/07 15:02:03 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	one_philo(t_philo *philo, t_all *params)
{
	pthread_mutex_lock(philo->left);
	print_action(philo, "has taken a fork");
	precise_usleep(params->time_to_die);
	pthread_mutex_unlock(philo->left);
}

void	*dining(void *philo_data)
{
	t_philo	*philo;

	philo = (t_philo *)philo_data;
	while (get_time_ms() < philo->params->start_time)
		usleep(100);
	if (philo->params->no_philos == 1)
	{
		one_philo(philo, philo->params);
		return (NULL);
	}
	if (philo->id % 2 != 0)
		philo_sleep(philo, philo->params);
	pthread_mutex_lock(&philo->params->dead_flag);
	while (!philo->params->dead)
	{
		pthread_mutex_unlock(&philo->params->dead_flag);
		philo_think(philo);
		philo_take_forks(philo);
		philo_eat(philo, philo->params);
		philo_release_forks(philo);
		philo_sleep(philo, philo->params);
		pthread_mutex_lock(&philo->params->dead_flag);
	}
	pthread_mutex_unlock(&philo->params->dead_flag);
	return (NULL);
}
