/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 20:19:28 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/02/07 15:02:21 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_threads(t_all *params)
{
	int	i;
	pthread_t	*threads;
	pthread_t	monitor_th;

	i = -1;
	threads = malloc(params->no_philos * sizeof(pthread_t));
	if (!threads)
	{
		printf("Thread array malloc fail\n");
		return ;
	}
	while (++i < params->no_philos)
	{
		if (pthread_create(&threads[i], NULL, simulation, &params->t_philo[i]) != 0)
		{
			printf("Thread create error for philo %d\n", i + 1);//add freeing of the previously allocated threads, params
			exit (1);
		}
	}
	if (pthread_create(&monitor_th, NULL, (void *(*)(void *))monitor, params) != 0)
		exit(1);// exit is not allowed in mandatory
	i = -1;
	while (++i < params->no_philos)
		pthread_join(threads[i], NULL);
	pthread_join(monitor_th, NULL);
	params->threads = threads;
}

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

void	cleanup_mutexes(t_all *params)
{
	int	i;

	i = -1;
	pthread_mutex_destroy(&params->dead_flag);
	pthread_mutex_destroy(&params->print_mutex);
	while (++i < params->no_philos)
		pthread_mutex_destroy(&params->forks[i]);
}

int	initialize_mutexes(t_all *params)
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

void	init_philos(int *input, t_all *params, int argc)
{
	int	i;

	i = -1;
	fill_struct(input, params, argc);
	params->forks = malloc(sizeof(pthread_mutex_t) * params->no_philos);
	if (!params->forks)
	{
		printf("fork malloc error");
		return ;
	}
	if (initialize_mutexes(params))
	{
		free(params->forks);
		return ;
	}
	params->t_philo = malloc(sizeof(t_philo) * params->no_philos);
	if (!params->t_philo)
	{
		printf("philo array malloc error");//cleanup mutexes, free(params->forks)
		cleanup_mutexes(params);
		free(params->forks);
		return ;
	}
	while (++i < params->no_philos)
	{
		params->t_philo[i].id = i + 1;
		params->t_philo[i].meals_count = 0;
		params->t_philo[i].last_meals_time = params->start_time;
		params->t_philo[i].left = &params->forks[i];
		params->t_philo[i].right = &params->forks[(i + 1) % params->no_philos];
		params->t_philo[i].params = params;
		pthread_mutex_init(&params->t_philo[i].meal_lock, NULL);
	}
	init_threads(params);
}
