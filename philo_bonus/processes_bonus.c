/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:26:12 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/02/11 15:47:37 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	one_philo(t_all *params, t_philo *philo)
{
	while (get_time_ms() < philo->params->start_time)
		usleep(100);
	while (!params->dead)
	{
		philo_think(philo);
		sem_wait(params->sem_forks);
		print_action(philo, "has taken a fork");
		precise_usleep(params->time_to_die);
		sem_post(params->sem_forks);
	}
	sem_post(params->death_sem);
	cleanup_semaphores(params);
	clean_pids(params);
	pthread_join(philo->monitor_th, NULL);
	clean_philos(params);
	exit (2);
}

void	run_philosophers(t_all *params, t_philo *philo)
{
	start_simulation(philo, philo->params);
	sem_wait(params->death_sem);
	while (!params->dead)
	{
		sem_post(params->death_sem);
		philo_think(philo);
		pick_up_forks(philo, philo->params);
		philo_eat(philo, philo->params);
		sem_post(params->sem_forks);
		sem_post(params->sem_forks);
		if (params->meals_no > 0 && philo->meals_count == params->meals_no)
		{
			pthread_join(philo->monitor_th, NULL);
			all_cleanup(params);
			exit (0);
		}
		philo_sleep(philo, philo->params);
		sem_wait(params->death_sem);
	}
	sem_post(params->death_sem);
	pthread_join(philo->monitor_th, NULL);
	all_cleanup(params);
	exit (2);
}

int	fork_for_philo(t_all *params, int i)
{
	params->pid_arr[i] = fork();
	if (params->pid_arr[i] == -1)
		return (1);
	if (params->pid_arr[i] == 0)
	{
		if (pthread_create(&params->t_philo[i].monitor_th, NULL,
				monitor, &params->t_philo[i]) != 0)
		{
			printf("Failed to create monitor thread for philosopher %d\n", i);
			exit (3);
		}
		if (params->no_philos == 1)
			one_philo(params, &params->t_philo[i]);
		else
			run_philosophers(params, &params->t_philo[i]);
	}
	return (0);
}

void	*watch_terminate_sem(void *arg)
{
	t_all	*params;
	int		i;
	int		status;

	i = -1;
	params = (t_all *)arg;
	sem_wait(params->terminate_sem);
	if (params->full_flag == params->no_philos)
		return (NULL);
	params->dead = 1;
	while (++i < params->no_philos)
	{
		if (params->pid_arr[i] > 0)
		{
			if (kill(params->pid_arr[i], 0) == 0)
			{
				kill(params->pid_arr[i], SIGKILL);
				if (waitpid(params->pid_arr[i], &status, 0) == -1
					&& errno != ECHILD)
					printf("waitpid failed for process %d\n", i);
			}
		}
	}
	return (NULL);
}

int	wait_for_philosophers(t_all *params)
{
	pthread_t	watcher_thread;
	int			i;
	int			status;

	i = -1;
	status = -1;
	if (pthread_create(&watcher_thread, NULL, watch_terminate_sem, params) != 0)
		return (printf("termination watcher thread creation fail\n"));
	i = -1;
	while (++i < params->no_philos)
	{
		if (waitpid(params->pid_arr[i], &status, 0) == -1 && errno != ECHILD)
			printf("waitpid failed\n");
		if (status == 0)
			params->full_flag++;
	}
	sem_post(params->terminate_sem);
	pthread_join(watcher_thread, NULL);
	if (params->full_flag == params->no_philos)
		printf("All philosophers have eaten enough times\n");
	return (0);
}
