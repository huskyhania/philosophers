/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:15:54 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/11/16 20:48:51 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>

typedef struct philo_table
{
	int no_philos;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int meals_no;
}	t_all;

typedef struct philosopher
{
	int	id;
	int	meals_count;
}	one_philo;

void	init_philos(int *input, t_all *params, int argc);

#endif
