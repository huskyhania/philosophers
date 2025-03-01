/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 12:45:33 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/02/22 13:26:18 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_one_sem(sem_t **sem, const char *name, int value)
{
	sem_unlink(name);
	*sem = sem_open(name, O_CREAT, 0644, value);
	if (*sem == SEM_FAILED)
	{
		printf("Semaphore %s initialization failed\n", name);
		*sem = NULL;
		return (1);
	}
	return (0);
}

void	unlink_semaphores(t_all *params)
{
	(void)params;
	sem_unlink("sem_forks");
	sem_unlink("print_sem");
	sem_unlink("death_sem");
	sem_unlink("terminate_sem");
	sem_unlink("eat_sem");
}

void	cleanup_semaphores(t_all *params)
{
	if (params->sem_forks)
		sem_close(params->sem_forks);
	if (params->print_sem)
		sem_close(params->print_sem);
	if (params->death_sem)
		sem_close(params->death_sem);
	if (params->terminate_sem)
		sem_close(params->terminate_sem);
	if (params->eat_sem)
		sem_close(params->eat_sem);
}
