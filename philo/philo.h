/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 18:15:54 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/01/29 17:36:00 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h> //for exit
# include <sys/time.h>

typedef struct s_philo_table
{
	int						no_philos;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						meals_no;
	int						dead;
	pthread_mutex_t			*forks;
	pthread_mutex_t			dead_flag;
	pthread_mutex_t			print_mutex;
	struct s_philosopher	*t_philo;
	long					start_time;
	pthread_t				*threads;
	pthread_t				monitor;
}	t_all;

typedef struct s_philosopher
{
	int				id;
	int				meals_count;
	long			last_meals_time;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
	pthread_mutex_t	meal_lock;
	t_all			*params;
}	t_philo;

int		init_philos(int *input, t_all *params, int argc);
void	*dining(void *philo_data);
int		init_threads(t_all *params, int i);
long	get_time_ms(void);
void	print_action(t_philo *philo, char *message);
void	*monitor(void *param);
int		precise_usleep(int millisecs);
void	cleanup_mutexes(t_all *params);
void	all_cleanup(t_all *params);

void	philo_think(t_philo *philo);
void	philo_eat(t_philo *philo, t_all *params);
void	philo_take_forks(t_philo *philo);
void	philo_release_forks(t_philo *philo);
void	philo_sleep(t_philo *philo, t_all *params);

#endif
