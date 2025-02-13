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
	t_philo	*one_philo;

	one_philo = (t_philo *)philo;
	params = (t_all *)one_philo->params;
	printf("i'm monitoring\n");
	while (1)
	{
		if (((get_time_ms() - one_philo->last_meals_time)
			> params->time_to_die))
		{
			printf("condition for dying met\n");
			sem_wait(params->death_sem);
			sem_wait(params->print_sem);
			printf("%ld %d died\n",
				get_time_ms() - params->start_time, one_philo->id);
			sem_post(params->print_sem);
			sem_close(params->sem_forks);
			sem_close(params->print_sem);
			sem_close(params->death_sem);
			printf("philosopher died, about to exit...\n");
			exit (2);
		}
		//usleep for testing???
	}
	return (NULL);
}
