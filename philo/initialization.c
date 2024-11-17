/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 20:19:28 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/11/17 18:18:57 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*simulation(void *philo_data)
{
	t_philo *philo = (t_philo *)philo_data;
	printf("Philosopher %d successfully started simulation\n", philo->id);
	pthread_exit(NULL);
}

void	init_threads(t_all *params)
{
	int	i;
	pthread_t	threads[params->no_philos];

	i = -1;
	while (++i < params->no_philos)
	{
		if (pthread_create(&threads[i], NULL, simulation, &params->t_philo[i]) != 0)
		{
			printf("Thread create error for philo %d\n", i + 1);
			exit (1);
		}
		else
			printf("Thread %d created for philo %d\n", i, i + 1);
	}
	i = -1;
	while (++i < params->no_philos)
	{
		pthread_join(threads[i], NULL);
		printf("thread %d for philo %d joined\n", i, i + 1);
	}
}

void	init_philos(int *input, t_all *params, int argc)
{
	int	i;

	i = -1;
	params->no_philos = input[0];
	params->time_to_die = input[1];
	params->time_to_eat = input[2];
	params->time_to_sleep = input[3];
	if (argc == 6)
		params->meals_no = input[4];
	else
		params->meals_no = -1;
	t_philo philo[params->no_philos];
	while (++i < params->no_philos)
	{
		philo[i].id = i + 1;
		philo[i].meals_count = 0;
		philo[i].last_meals_time = 0;
		philo[i].params = params;
	}
	params->t_philo = philo;
	init_threads(params);
}
