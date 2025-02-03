/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:39:09 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/02/01 14:48:26 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_action(t_philo *philo, char *message)
{
	t_all *params = philo->params;
	
	pthread_mutex_lock(&params->print_mutex);
	//if (!philo->params->dead)
	printf("%ld %d %s\n", get_time_ms() - params->start_time, philo->id, message);
	pthread_mutex_unlock(&params->print_mutex);
}

int	check_for_death(t_all *params)
{
	pthread_mutex_lock(&params->dead_flag);
	if (params->dead)
	{
		pthread_mutex_unlock(&params->dead_flag);
		return (1);
	}
	pthread_mutex_unlock(&params->dead_flag);
	return (0);
}