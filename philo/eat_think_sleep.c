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
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meals_time = get_time_ms();
	pthread_mutex_unlock(&philo->meal_lock);
	precise_usleep(params, params->time_to_eat);
	pthread_mutex_lock(&philo->meal_lock);
	philo->meals_count++;
	pthread_mutex_unlock(&philo->meal_lock);
}

void	philo_take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0
		|| (philo->id == 1 && philo->params->no_philos % 2 != 0))
	{
		pthread_mutex_lock(philo->left);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->right);
		print_action(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->left);
		print_action(philo, "has taken a fork");
	}
}

void	philo_release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->right);
	pthread_mutex_unlock(philo->left);
}

void	philo_sleep(t_philo *philo, t_all *params)
{
	print_action(philo, "is sleeping");
	precise_usleep(params, params->time_to_sleep);
}
