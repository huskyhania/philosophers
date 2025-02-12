/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 11:17:32 by hskrzypi          #+#    #+#             */
/*   Updated: 2025/02/09 11:18:27 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*void	join_threads(t_all *params)
{
	if (pthread_join(params->monitor, NULL) != 0)
		printf("Failed to join monitor thread\n");
}

int	init_threads(t_all *p, int i)
{
	if (pthread_create(&p->monitor, NULL, monitor, p) != 0)
	{
		return (printf("monitor thread creation fail\n"));
	}
	join_threads(p);
	return (0);
}*/
