/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:38:05 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/02/07 15:02:03 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	one_philo(t_philo *philo, t_all *params)
{
	pthread_mutex_lock(philo->left);
	print_action(philo, "has taken a fork");
	precise_usleep(params->time_to_die);
	pthread_mutex_unlock(philo->left);
}

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
	precise_usleep(params->time_to_eat);
	pthread_mutex_lock(&philo->meal_lock);
	philo->meals_count++;
	pthread_mutex_unlock(&philo->meal_lock);
}

void	philo_take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0 || philo->id == 1)
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
	precise_usleep(params->time_to_sleep);
}

void	*simulation(void *philo_data)
{
	t_philo	*philo;

	philo = (t_philo *)philo_data;
	while (get_time_ms() < philo->params->start_time)
		usleep(100);
	if (philo->id % 2 != 0)
		philo_sleep(philo, philo->params);
	pthread_mutex_lock(&philo->params->dead_flag);
	while (!philo->params->dead)
	{
		pthread_mutex_unlock(&philo->params->dead_flag);
		philo_think(philo);
		if (philo->params->no_philos == 1)
		{
			one_philo(philo, philo->params);
			break ;
		}
		philo_take_forks(philo);
		philo_eat(philo, philo->params);
		philo_release_forks(philo);
		philo_sleep(philo, philo->params);
		pthread_mutex_lock(&philo->params->dead_flag);
	}
	pthread_mutex_unlock(&philo->params->dead_flag);
	pthread_exit(NULL);
}
