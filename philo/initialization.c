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

void	join_threads(t_all *params)
{
	int	i;

	i = -1;
	while (++i < params->no_philos)
	{
		if (pthread_join(params->threads[i], NULL) != 0)
			printf("Failed to join thread %d\n", i);
	}
	if (pthread_join(params->monitor, NULL) != 0)
		printf("Failed to join monitor thread\n");
	free(params->threads);
	params->threads = NULL;
}

int	init_threads(t_all *p)
{
	int	i;

	i = -1;
	p->threads = malloc(p->no_philos * sizeof(pthread_t));
	if (!p->threads)
		return (printf("Thread array malloc fail\n"));
	while (++i < p->no_philos)
	{
		if (pthread_create(&p->threads[i], NULL, dining, &p->t_philo[i]) != 0)
		{
			while (--i >= 0)
				pthread_detach(p->threads[i]);
			free(p->threads);
			return (printf("Thread create error for philo %d\n", i + 1));
		}
	}
	if (pthread_create(&p->monitor, NULL, monitor, p) != 0)
	{
		while (--i >= 0)
			pthread_detach(p->threads[i]);
		free(p->threads);
		return (printf("monitor thread creation fail\n"));
	}
	join_threads(p);
	return (0);
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

int	fill_philo_structs(t_all *params)
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
			return (1);
		}
	}
	init_threads(params);
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
	if (!fill_philo_structs(params))
	{
		if (!init_threads(params))
			return ;
	}
	cleanup_mutexes(params);
	free(params->forks);
}
