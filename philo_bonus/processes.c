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

int	fork_fork_philo(t_all *params, int i);
{
	params->pid_arr[i] = fork();
	if (params->pid_arr[i] == -1)
		return (1);
	if (params->pid_arr[i] == 0)
		run_philosophers(params, params->t_philo[i]);
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
		if (fork_for_philo(params, 0))
		{
			printf("fork for philosopher fail\n");
			clean_up_processes(params, i);
			return (1);
		}
	}
	return (0);
}
