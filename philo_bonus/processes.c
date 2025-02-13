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

void	run_philosophers(t_all *params, t_philo *philo)
{
	while (get_time_ms() < philo->params->start_time)
		usleep(100);
	if (params->no_philos % 2 != 0 && philo->id % 2 != 0)
		philo_sleep(philo, philo->params);
	while (!params->dead)
	{
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
			sem_close(philo->params->sem_forks);
			sem_close(philo->params->print_sem);
			sem_close(philo->params->death_sem);
			exit (0);
		}
		philo_sleep(philo, philo->params);
	}
	sem_close(philo->params->sem_forks);
	sem_close(philo->params->print_sem);
	sem_close(philo->params->death_sem);
	exit (1);
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
		run_philosophers(params, &params->t_philo[i]);
		pthread_join(params->t_philo[i].monitor_th, NULL);
		exit (2);
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
void wait_for_philosophers(t_all *params)
{
	int	i;
	int	status;
	
	i = 0;
	while (i < params->no_philos)
	{
		if (waitpid(params->pid_arr[i], &status, 0) == -1)
			printf("waitpid failed\n");
		else if (waitpid(params->pid_arr[i], &status, 0) == 2)
		{
			printf("process for philo %d exited with %d\n", i + 1, WEXITSTATUS(status));
			clean_up_processes(params, params->no_philos);
		}
		else if (WIFEXITED(status))
			printf("Philosopher %d exited with status %d\n", i + 1, WEXITSTATUS(status));
		i++;
	}
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
	return (0);
}
