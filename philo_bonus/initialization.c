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
int	init_semaphors(t_all *params)
{
	sem_unlink("sem_forks");
	params->sem_forks = sem_open("sem_forks", O_CREAT , 0644, params->no_philos);
	if (params->sem_forks == SEM_FAILED)
		return (printf("semaphor for forks fail\n"));
	sem_unlink("print_sem");
	params->print_sem = sem_open("print_sem", O_CREAT , 0644, 1);
	if (params->print_sem == SEM_FAILED)
	{
		sem_close(params->sem_forks);
		sem_unlink("sem_forks");
		return (printf("semaphor for print fail\n"));
	}
	sem_unlink("death_sem");
	params->death_sem = sem_open("death_sem", O_CREAT , 0644, 1);
	if (params->death_sem == SEM_FAILED)
	{
		sem_close(params->sem_forks);
		sem_close(params->print_sem);
		sem_unlink("sem_forks");
		sem_unlink("print_sem");
		return (printf("semaphor for death flag fail\n"));
	}
	printf("fork, print and death semaphors created\n");
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
		params->t_philo[i].params = params;
	}
	return (0);
}

int	init_philos(int *input, t_all *params, int argc)
{
	fill_struct(input, params, argc);
	printf("main struct created\n");
	if (init_semaphors(params))
		return (1);
	if (fill_philo_structs(params))
		return (1);
	create_processes(params);
	sem_close(params->sem_forks);
	sem_close(params->print_sem);
	sem_close(params->death_sem);
	sem_unlink("sem_forks");
	sem_unlink("print_sem");
	sem_unlink("death_sem");
	return (0);
}
