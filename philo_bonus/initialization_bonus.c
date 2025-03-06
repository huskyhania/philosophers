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

#include "philo_bonus.h"

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
	params->sem_forks = NULL;
	params->print_sem = NULL;
	params->death_sem = NULL;
	params->terminate_sem = NULL;
	params->eat_sem = NULL;
	params->pid_arr = NULL;
}

int	init_semaphors(t_all *params)
{
	if (init_one_sem(&params->sem_forks, "sem_forks", params->no_philos)
		|| init_one_sem(&params->print_sem, "print_sem", 1)
		|| init_one_sem(&params->death_sem, "death_sem", 1)
		|| init_one_sem(&params->terminate_sem, "terminate_sem", 0)
		|| init_one_sem(&params->eat_sem, "eat_sem", 1))
	{
		cleanup_semaphores(params);
		unlink_semaphores(params);
		return (1);
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
		params->t_philo[i].params = params;
		params->t_philo[i].next_food = params->start_time;
	}
	return (0);
}

int	create_processes(t_all *params)
{
	int	i;

	i = -1;
	params->pid_arr = malloc((sizeof(int)) * params->no_philos);
	if (!params->pid_arr)
		return (printf("pid malloc fail\n"));
	while (++i < params->no_philos)
		params->pid_arr[i] = -1;
	i = -1;
	while (++i < params->no_philos)
	{
		if (fork_for_philo(params, i))
		{
			printf("fork for philosopher fail\n");
			clean_up_processes(params, i);
			return (1);
		}
	}
	if (wait_for_philosophers(params))
	{
		clean_up_processes(params, i);
		return (1);
	}
	return (0);
}

int	init_philos(int *input, t_all *params, int argc)
{
	fill_struct(input, params, argc);
	if (init_semaphors(params))
		return (1);
	if (fill_philo_structs(params))
	{
		cleanup_semaphores(params);
		unlink_semaphores(params);
		return (1);
	}
	if (create_processes(params))
	{
		all_cleanup(params);
		unlink_semaphores(params);
		return (1);
	}
	return (0);
}
