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

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <semaphore.h>
# include <stdlib.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>

typedef struct s_philo_table
{
	int						no_philos;
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						meals_no;
	int						dead;
	int						*pid_arr;
	struct s_philosopher	*t_philo;
	long					start_time;
	sem_t					*sem_forks;
	sem_t					*print_sem;
	sem_t					*death_sem;
	sem_t					*terminate_sem;
	sem_t					*eat_sem;
}	t_all;

typedef struct s_philosopher
{
	int				id;
	int				meals_count;
	long			last_meals_time;
	pthread_t		monitor_th;
	t_all			*params;
}	t_philo;

int		init_philos(int *input, t_all *params, int argc);
long	get_time_ms(void);
void	print_action(t_philo *philo, char *message);
void	*monitor(void *param);
int		precise_usleep(int millisecs);

void	philo_think(t_philo *philo);
void	philo_eat(t_philo *philo, t_all *params);
void	pick_up_forks(t_philo *philo, t_all *params);
void	philo_sleep(t_philo *philo, t_all *params);
void	start_simulation(t_philo *philo, t_all *params);

int		create_processes(t_all *params);
int		wait_for_philosophers(t_all *params);
int		fork_for_philo(t_all *params, int i);
int		init_one_sem(sem_t **sem, const char *name, int value);

void	all_cleanup(t_all *params);
void	clean_philos(t_all *params);
void	clean_pids(t_all *params);
void	clean_up_processes(t_all *params, int count);
void	cleanup_semaphores(t_all *params);
void	unlink_semaphores(t_all *params);

#endif
