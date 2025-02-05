/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 20:19:28 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/02/01 17:22:33 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	one_philo(t_philo *philo, t_all *params)
{
	pthread_mutex_lock(philo->left);
	print_action(philo, "has taken a fork");
	precise_usleep(params->time_to_die);
	pthread_mutex_unlock(philo->left);
}

void	*simulation(void *philo_data)
{
	t_philo *philo = (t_philo *)philo_data;
	t_all *params = philo->params;
	printf("Philosopher %d successfully started simulation\n", philo->id);
	while (get_time_ms() < params->start_time)
		usleep(100);
	if (philo->id % 2 != 0)
	{
		print_action(philo, "is thinking");
		precise_usleep(50);
	}
	while (!params->dead)
	{
		//if (philo->id % 2 != 0)
		//	precise_usleep(500);
		//print_action(philo, "is thinking");
		if (params->no_philos == 1)
		{
			one_philo(philo, params);
			break;
		}
		if (philo->id == params->no_philos)
		{
			pthread_mutex_lock(philo->left);
			print_action(philo, "has taken a fork");
			pthread_mutex_lock(philo->right);
			print_action(philo, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(philo->right);
			print_action(philo, "has taken a fork");
			pthread_mutex_lock(philo->left);
			print_action(philo, "has taken a fork");
		}
		philo->eating_f = 1;
		print_action(philo, "is eating");
		pthread_mutex_lock(&philo->meal_lock);
		philo->last_meals_time = get_time_ms();
		pthread_mutex_unlock(&philo->meal_lock);
		precise_usleep(params->time_to_eat);
		pthread_mutex_lock(&philo->meal_lock);
		philo->meals_count++;
		pthread_mutex_unlock(&philo->meal_lock);
		philo->eating_f = 0;
		pthread_mutex_unlock(philo->right);
		pthread_mutex_unlock(philo->left);
		if (params->meals_no > 0 && philo->meals_count >= params->meals_no)
			break;
		print_action(philo, "is sleeping");
		precise_usleep(params->time_to_sleep);
		print_action(philo, "is thinking");
	}
	pthread_exit(NULL);
}

void	init_threads(t_all *params)
{
	int	i;
	pthread_t	*threads;
	pthread_t	monitor_th;

	i = -1;
	threads = malloc(params->no_philos * sizeof(pthread_t));
	if (!threads)
	{
		printf("Thread array malloc fail\n");//free params
		exit (1);
	}
	while (++i < params->no_philos)
	{
		if (pthread_create(&threads[i], NULL, simulation, &params->t_philo[i]) != 0)
		{
			printf("Thread create error for philo %d\n", i + 1);//add freeing of the previously allocated threads, params
			exit (1);
		}
		else
			printf("Thread %d created for philo %d\n", i, i + 1);
	}
	if (pthread_create(&monitor_th, NULL, (void *(*)(void *))monitor, params) != 0)
	{
		printf("Monitor thread create error\n");
		exit(1);
	}
	i = -1;
	while (++i < params->no_philos)
	{
		pthread_join(threads[i], NULL);
		printf("thread %d for philo %d joined\n", i, i + 1);
	}
	pthread_join(monitor_th, NULL);
	printf("Monitor thread joined\n");
	params->threads = threads;
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
	params->dead = 0;
	params->start_time = get_time_ms() + 1000;
	pthread_mutex_init(&params->dead_flag, NULL);
	pthread_mutex_init(&params->print_mutex, NULL);
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
	params->t_philo = malloc(sizeof(t_philo) * params->no_philos);
	if (!params->t_philo)
	{
		printf("philo malloc error");
		return ;
	}
	i = -1;
	while (++i < params->no_philos)
	{
		params->t_philo[i].id = i + 1;
		params->t_philo[i].meals_count = 0;
		params->t_philo[i].last_meals_time = params->start_time;
		params->t_philo[i].left = &params->forks[i];
		params->t_philo[i].right = &params->forks[(i + 1) % params->no_philos];
		params->t_philo[i].eating_f = 0;
		params->t_philo[i].params = params;
		pthread_mutex_init(&params->t_philo[i].meal_lock, NULL);
		printf("philo %d created, left fork %d and right fork %d\n",
			params->t_philo[i].id, i, (i + 1) % params->no_philos);
	}
	init_threads(params);
}
