/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hskrzypi <hskrzypi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:45:01 by hskrzypi          #+#    #+#             */
/*   Updated: 2024/11/17 16:11:12 by hskrzypi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_number(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static long	ft_atol(const char *str)
{
	int	i;
	long	number;
	int	negation;

	i = 0;
	number = 0;
	negation = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			negation = -1;
		i++;
	}
	while (str[i] != '\0')
	{
		number = number * 10 + (str[i] - 48);
		i++;
	}
	return (number * negation);
}

static int	input_check(int argc, char **argv, int *input)
{
	int	i;
	long	n;
	i = 1;
	while (i < argc)
	{
		if (is_number(argv[i]))
			n = ft_atol(argv[i]);
		else 
			return (1);
		if (n > 0 && n <= INT_MAX)
			input[i - 1] = (int)n;
		else
			return (1);
		i++;
	}
	return (0);
}

static int	input_error(void)
{
	int	ret;

	ret = printf("Please give 4 or 5 positive numbers\n"
		"no of philos(1), time to die(2), eat(3), sleep(4)\n"
		"and optional number of meals(5)\n");
	return (ret);
}

int	main(int argc, char **argv)
{
	int	input[5];
	t_all	ph_params;

	if (argc < 5 || argc > 6)
		return	(input_error());
	else
	{
		if (!input_check(argc, argv, input))
		{
			printf("Run philosophers\n");
			init_philos(input, &ph_params, argc);
		}
		else
			return (input_error());
	}
	return (0);
}
