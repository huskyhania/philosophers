/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 17:14:06 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/02/22 17:14:19 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	clean_philos(t_all *params)
{
	if (params->t_philo)
	{
		free(params->t_philo);
		params->t_philo = NULL;
	}
}

void	clean_pids(t_all *params)
{
	if (params->pid_arr)
	{
		free(params->pid_arr);
		params->pid_arr = NULL;
	}
}

void	all_cleanup(t_all *params)
{
	clean_pids(params);
	clean_philos(params);
	cleanup_semaphores(params);
}

void	clean_up_processes(t_all *params, int count)
{
	int	i;
	int	status;

	i = -1;
	while (++i < count)
	{
		if (params->pid_arr[i] > 0)
			kill(params->pid_arr[i], SIGKILL);
	}
	i = -1;
	while (++i < count)
	{
		if (params->pid_arr[i] > 0)
		{
			if (waitpid(params->pid_arr[i], &status, 0) == -1
				&& errno != ECHILD)
				printf("waitpid failed for process %d\n", i);
		}
	}
}
