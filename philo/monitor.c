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

void	check_death_occured(t_all *params, int i)
{
	pthread_mutex_lock(&params->t_philo[i].meal_lock);
	if (((get_time_ms() - params->t_philo[i].last_meals_time)
			> params->time_to_die))
	{
		pthread_mutex_lock(&params->dead_flag);
		params->dead = 1;
		pthread_mutex_unlock(&params->dead_flag);
		pthread_mutex_lock(&params->print_mutex);
		printf("%ld %d died\n",
			get_time_ms() - params->start_time, params->t_philo[i].id);
		pthread_mutex_unlock(&params->print_mutex);
		pthread_mutex_unlock(&params->t_philo[i].meal_lock);
		pthread_exit(NULL);
	}
	pthread_mutex_unlock(&params->t_philo[i].meal_lock);
}

int	check_eating_done(t_all *params)
{
	int	i;
	int	finished_meals;

	finished_meals = 0;
	i = -1;
	while (++i < params->no_philos)
	{
		pthread_mutex_lock(&params->t_philo[i].meal_lock);
		if (params->meals_no > 0
			&& params->t_philo[i].meals_count >= params->meals_no)
			finished_meals++;
		pthread_mutex_unlock(&params->t_philo[i].meal_lock);
	}
	if (params->meals_no > 0 && finished_meals == params->no_philos)
	{
		pthread_mutex_lock(&params->dead_flag);
		params->dead = 1;
		pthread_mutex_unlock(&params->dead_flag);
		pthread_mutex_lock(&params->print_mutex);
		printf("All philosophers have eaten enough times. Ending simulation.\n");
		pthread_mutex_unlock(&params->print_mutex);
		return (1);
	}
	return (0);
}

void	*monitor(void *param)
{
	int		i;
	t_all	*params;

	params = (t_all *)param;
	while (1)
	{
		i = -1;
		while (++i < params->no_philos)
			check_death_occured(params, i);
		if (check_eating_done(params))
			return (NULL);
		//precise_usleep(5000);
	}
	return (NULL);
}
