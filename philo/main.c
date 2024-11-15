/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:45:01 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/11/09 19:43:57 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void input_error(void)
{
	write(1, "Please give 4 or 5 arguments\n", 30);
	write(1, "no of philos(1), time to die(2), eat(3), sleep(4)\n", 51);
	write(1, "and optional number of meals(5)", 32);
}

int main(int argc, char **argv)
{
	(void)argv;
	if (argc < 5 || argc > 6)
	{
		input_error();
		return (1);
	}
	else
		printf("Run philosophers\n");
	return (0);
}
