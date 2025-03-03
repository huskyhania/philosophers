/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 19:52:35 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/02/06 15:04:06 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	handle_death(t_all *params, t_philo *philo)
{
	int	i;

	i = -1;
	sem_post(params->eat_sem);
	sem_wait(params->print_sem);
	sem_wait(params->death_sem);
	params->dead = 1;
	sem_post(params->death_sem);
	printf("%ld %d died\n", get_time_ms() - params->start_time, philo->id);
	while (++i < params->no_philos)
		sem_post(params->terminate_sem);
	sem_post(params->sem_forks);
}

void	*monitor(void *philo)
{
	t_all	*params;
	t_philo	*o_philo;

	o_philo = (t_philo *)philo;
	params = (t_all *)o_philo->params;
	while (get_time_ms() < params->start_time)
		usleep(100);
	while (1)
	{
		sem_wait(params->eat_sem);
		if (((get_time_ms() - o_philo->last_meals_time) > params->time_to_die))
		{
			handle_death(params, o_philo);
			break ;
		}
		sem_post(params->eat_sem);
		usleep(500);
	}
	return (NULL);
}
