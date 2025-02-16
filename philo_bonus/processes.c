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

#include "philo.h"

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
	sem_close(params->sem_forks);
	sem_close(params->print_sem);
	sem_close(params->death_sem);
	sem_close(params->terminate_sem);
	sem_close(params->eat_sem);
	free(params->pid_arr);
	params->pid_arr = NULL;
	pthread_join(philo->monitor_th, NULL);
	free(params->t_philo);
	params->t_philo = NULL;
	exit (2);
}

void	run_philosophers(t_all *params, t_philo *philo)
{
	while (get_time_ms() < philo->params->start_time)
		usleep(100);
	if (philo->id % 2 != 0)
		philo_sleep(philo, philo->params);
	sem_wait(params->death_sem);
	while (!params->dead)
	{
		sem_post(params->death_sem);
		philo_think(philo);
		sem_wait(params->sem_forks);
		print_action(philo, "has taken a fork");
		sem_wait(params->sem_forks);
		print_action(philo, "has taken a fork");
		philo_eat(philo, philo->params);
		sem_post(params->sem_forks);
		sem_post(params->sem_forks);
		if (params->meals_no > 0 && philo->meals_count == params->meals_no)
		{
			if (params->pid_arr)
			{
				free(params->pid_arr);
				params->pid_arr = NULL;
			}
			pthread_join(philo->monitor_th, NULL);
			if (params->t_philo)
			{
				free(params->t_philo);
				params->t_philo = NULL;
			}
			sem_close(params->sem_forks);
			sem_close(params->print_sem);
			sem_close(params->death_sem);
			sem_close(params->terminate_sem);
			sem_close(params->eat_sem);
			exit (0);
		}
		philo_sleep(philo, philo->params);
		sem_wait(params->death_sem);
	}
	sem_post(params->death_sem);
	sem_close(params->sem_forks);
	sem_close(params->print_sem);
	sem_close(params->death_sem);
	sem_close(params->terminate_sem);
	sem_close(params->eat_sem);
	free(params->pid_arr);
	params->pid_arr = NULL;
	pthread_join(philo->monitor_th, NULL);
	free(params->t_philo);
	params->t_philo = NULL;
	exit (2);
}

int	fork_for_philo(t_all *params, int i)
{
	params->pid_arr[i] = fork();
	if (params->pid_arr[i] == -1)
		return (1);
	if (params->pid_arr[i] == 0)
	{
		if (pthread_create(&params->t_philo[i].monitor_th, NULL, monitor, &params->t_philo[i]) != 0)
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

void	clean_up_processes(t_all *params, int count)
{
	int	i;

	i = -1;
	while (++i < count)
	{
		if (params->pid_arr[i] > 0)
			kill(params->pid_arr[i], SIGKILL);
	}
	free(params->pid_arr);
	params->pid_arr = NULL;
}
void	*watch_terminate_sem(void *arg)
{
	t_all *params = (t_all *)arg;

	sem_wait(params->terminate_sem);
	params->dead = 1;

	for (int j = 0; j < params->no_philos; j++)
	{
		if (params->pid_arr[j] > 0)
			kill(params->pid_arr[j], SIGKILL);
	}
	return (NULL);
}

void wait_for_philosophers(t_all *params)
{
	pthread_t watcher_thread;
	int i, status;
	if (pthread_create(&watcher_thread, NULL, watch_terminate_sem, params) != 0)
	{
		return;
	}
	for (i = 0; i < params->no_philos; i++)
	{
		if (waitpid(params->pid_arr[i], &status, 0) == -1)
			printf("waitpid failed\n");
	}
	sem_post(params->terminate_sem);
	pthread_join(watcher_thread, NULL);
	//sem_post(params->terminate_sem);
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
	wait_for_philosophers(params);
	free(params->pid_arr);
	params->pid_arr = NULL;
	free(params->t_philo);
	params->t_philo = NULL;
	return (0);
}
