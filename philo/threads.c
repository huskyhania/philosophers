/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 11:17:32 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/02/09 11:18:27 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	join_threads(t_all *params, int no_created, int monitor_flag)
{
	int	i;

	i = -1;
	while (++i < no_created)
	{
		if (pthread_join(params->threads[i], NULL) != 0)
			printf("Failed to join thread %d\n", i);
	}
	if (monitor_flag)
	{
		if (pthread_join(params->monitor, NULL) != 0)
			printf("Failed to join monitor thread\n");
	}
	free(params->threads);
	params->threads = NULL;
}

int	init_threads(t_all *p, int i)
{
	p->threads = malloc(p->no_philos * sizeof(pthread_t));
	if (!p->threads)
		return (printf("Thread array malloc fail\n"));
	while (++i < p->no_philos)
	{
		if (pthread_create(&p->threads[i], NULL, dining, &p->t_philo[i]) != 0)
		{
			pthread_mutex_lock(&p->dead_flag);
			p->dead = 1;
			pthread_mutex_unlock(&p->dead_flag);
			join_threads(p, i, 0);
			return (printf("Thread create error for philo %d\n", i + 1));
		}
	}
	if (pthread_create(&p->monitor, NULL, monitor, p) != 0)
	{
		pthread_mutex_lock(&p->dead_flag);
		p->dead = 1;
		pthread_mutex_unlock(&p->dead_flag);
		join_threads(p, p->no_philos, 0);
		return (printf("monitor thread creation fail\n"));
	}
	join_threads(p, p->no_philos, 1);
	return (0);
}
