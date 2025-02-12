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
	sem_wait(params->death_sem);
	if (!philo->params->dead)
	{
		sem_wait(params->print_sem);
		printf("%ld %d %s\n", get_time_ms() - params->start_time,
			philo->id, message);
		sem_post(params->print_sem);
	}
	sem_post(params->death_sem);
}

int	precise_usleep(int millisecs)
{
	long	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < millisecs)
		usleep(500);
	return (0);
}

void	all_cleanup(t_all *params)
{
	int	i;

	i = -1;
	if (params->t_philo)
	{
		free(params->t_philo);
		params->t_philo = NULL;
	}
}
