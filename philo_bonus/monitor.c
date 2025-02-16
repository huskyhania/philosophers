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

#include "philo.h"

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
		if ((params->meals_no > 0) && o_philo->meals_count >= params->meals_no)
		{
			sem_post(params->eat_sem);
			break ;
		}
		if (((get_time_ms() - o_philo->last_meals_time) > params->time_to_die))
		{
			sem_post(params->eat_sem);
			sem_wait(params->print_sem);
			printf("%ld %d died\n",
				get_time_ms() - params->start_time, o_philo->id);
			sem_wait(params->death_sem);
			params->dead = 1;
			sem_post(params->death_sem);
			sem_post(params->terminate_sem);
			break ;
		}
		sem_post(params->eat_sem);
		//usleep(500);
	}
	return (NULL);
}
