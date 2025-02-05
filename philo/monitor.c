/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 19:52:35 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/02/01 17:23:00 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *param)
{
	t_all	*params = (t_all *)param;
	int	i;
	int	finished_meals;

	while (1)
	{
		finished_meals = 0;
		i = -1;
		while (++i < params->no_philos)
		{
			pthread_mutex_lock(&params->t_philo[i].meal_lock);
			if (((get_time_ms() - params->t_philo[i].last_meals_time) > params->time_to_die) && (!params->t_philo[i].eating_f))
			{
				// printf("%ld print from monitor\n", get_time_ms() - params->start_time);
				printf("The last meal time of the deceased: %ld\n", get_time_ms() - params->t_philo[i].last_meals_time);
				pthread_mutex_lock(&params->dead_flag);
				params->dead = 1;
				pthread_mutex_unlock(&params->dead_flag);
				print_action(&params->t_philo[i], "died");
				pthread_mutex_unlock(&params->t_philo[i].meal_lock);
				return (NULL);
			}
			if (params->meals_no > 0 && params->t_philo[i].meals_count >= params->meals_no)
				finished_meals++;

			pthread_mutex_unlock(&params->t_philo[i].meal_lock);
		}
		if (params->meals_no > 0 && finished_meals == params->no_philos)
		{
			pthread_mutex_lock(&params->dead_flag);
			params->dead = 1;
			pthread_mutex_unlock(&params->dead_flag);
			printf("All philosophers have eaten enough times. Ending simulation.\n");
			return (NULL);
		}
		//precise_usleep(500);
	}
	return (NULL);
}
