/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 20:19:28 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/11/20 21:35:35 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*simulation(void *philo_data)
{
	t_philo *philo = (t_philo *)philo_data;
	t_all *params = philo->params;
	int left_fork = philo->id;
	int right_fork = (philo->id + 1) % philo->id;
	printf("Philosopher %d successfully started simulation\n", philo->id);
	while (1)
	{
		printf("philosopher %d is thinking\n", philo->id);
		printf("Philosopher %d picked up left fork %d\n", philo->id, left_fork);
        	pthread_mutex_lock(&params->forks[right_fork]);
        	printf("Philosopher %d picked up right fork %d\n", philo->id, right_fork);

        	// Eating
        	printf("Philosopher %d is eating\n", philo->id);
        	usleep(params->time_to_eat * 1000); // Simulate eating time

        	// Release forks
        	pthread_mutex_unlock(&params->forks[right_fork]);
        	printf("Philosopher %d put down right fork %d\n", philo->id, right_fork);
        	pthread_mutex_unlock(&params->forks[left_fork]);
        	printf("Philosopher %d put down left fork %d\n", philo->id, left_fork);

        	// Sleeping
        	printf("Philosopher %d is sleeping\n", philo->id);
        	usleep(params->time_to_sleep * 1000); // Simulate sleeping time
	}
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
	params->forks = malloc(sizeof(pthread_mutex_t) * params->no_philos);
	if (!params->forks)
	{
		printf("fork malloc error");
		return ;
	}
	while (++i < params->no_philos)
	{
		pthread_mutex_init(&params->forks[i], NULL);
		printf("mutex %d created\n", i);
	}
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
