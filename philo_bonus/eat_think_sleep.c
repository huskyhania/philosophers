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

#include "philo.h"

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
	philo->meals_count++;
}

void	philo_sleep(t_philo *philo, t_all *params)
{
	print_action(philo, "is sleeping");
	precise_usleep(params->time_to_sleep);
}
