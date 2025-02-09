/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 20:19:28 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/02/08 15:02:25 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	fill_struct(int *input, t_all *params, int argc)
{
	params->no_philos = input[0];
	params->time_to_die = input[1];
	params->time_to_eat = input[2];
	params->time_to_sleep = input[3];
	if (argc == 6)
		params->meals_no = input[4];
	else
		params->meals_no = -1;
	params->dead = 0;
	params->start_time = get_time_ms() + 1000;
}

static int	initialize_mutexes(t_all *params)
{
	int	i;

	i = -1;
	if (pthread_mutex_init(&params->dead_flag, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&params->print_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&params->dead_flag);
		return (1);
	}
	while (++i < params->no_philos)
	{
		if (pthread_mutex_init(&params->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&params->forks[i]);
			pthread_mutex_destroy(&params->dead_flag);
			pthread_mutex_destroy(&params->print_mutex);
			return (1);
		}
	}
	return (0);
}

static int	fill_philo_structs(t_all *params)
{
	int	i;

	i = -1;
	params->t_philo = malloc(sizeof(t_philo) * params->no_philos);
	if (!params->t_philo)
		return (printf("philo array malloc error\n"));
	while (++i < params->no_philos)
	{
		params->t_philo[i].id = i + 1;
		params->t_philo[i].meals_count = 0;
		params->t_philo[i].last_meals_time = params->start_time;
		params->t_philo[i].left = &params->forks[i];
		params->t_philo[i].right = &params->forks[(i + 1) % params->no_philos];
		params->t_philo[i].params = params;
		if (pthread_mutex_init(&params->t_philo[i].meal_lock, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&params->t_philo[i].meal_lock);
			free(params->t_philo);
			params->t_philo = NULL;
			return (1);
		}
	}
	return (0);
}

int	init_philos(int *input, t_all *params, int argc)
{
	int	i;

	i = -1;
	fill_struct(input, params, argc);
	params->forks = malloc(sizeof(pthread_mutex_t) * params->no_philos);
	if (!params->forks)
		return (printf("fork malloc error"));
	if (initialize_mutexes(params))
	{
		free(params->forks);
		params->forks = NULL;
		return (1);
	}
	if (!fill_philo_structs(params))
	{
		if (!init_threads(params, -1))
			return (0);
	}
	cleanup_mutexes(params);
	free(params->forks);
	params->forks = NULL;
	return (1);
}
