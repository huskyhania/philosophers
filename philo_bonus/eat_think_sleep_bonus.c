/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_think_sleep.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 11:12:46 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/02/09 11:15:13 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	philo_think(t_philo *philo)
{
	print_action(philo, "is thinking");
	usleep(1000);
}

void	philo_eat(t_philo *philo, t_all *params)
{
	print_action(philo, "is eating");
	sem_wait(params->eat_sem);
	philo->last_meals_time = get_time_ms();
	sem_post(params->eat_sem);
	precise_usleep(params->time_to_eat);
	sem_wait(params->eat_sem);
	philo->meals_count++;
	sem_post(params->eat_sem);
}

void	philo_sleep(t_philo *philo, t_all *params)
{
	print_action(philo, "is sleeping");
	precise_usleep(params->time_to_sleep);
}

void	pick_up_forks(t_philo *philo, t_all *params)
{
	sem_wait(params->sem_forks);
	print_action(philo, "has taken a fork");
	sem_wait(params->sem_forks);
	print_action(philo, "has taken a fork");
}

void	start_simulation(t_philo *philo, t_all *params)
{
	(void)params;
	while (get_time_ms() < philo->params->start_time)
		usleep(100);
	if (philo->id % 2 != 0)
		philo_sleep(philo, philo->params);
}
