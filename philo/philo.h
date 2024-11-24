/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:15:54 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/11/24 19:39:55 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h> //for exit
//# define MAX_PHILOS 1000

typedef struct philo_table
{
	int	no_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	meals_no;
	pthread_mutex_t *forks;
	struct philosopher	*t_philo; //array of philosopher structs
}	t_all;

typedef struct philosopher
{
	int	id;
	int	meals_count;
	long	last_meals_time;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	t_all	*params;
}	t_philo;

void	init_philos(int *input, t_all *params, int argc);
void	*simulation(void *philo_data);
void	init_threads(t_all *params);

#endif
