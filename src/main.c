/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fboughan <fboughan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 12:40:22 by fboughan          #+#    #+#             */
/*   Updated: 2024/03/14 14:49:22 by fboughan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosopher.h"

int	main(int argc, char **argv)
{
	t_args			args;
	pthread_mutex_t	*forks;
	t_philo			*philos;

	if (!init_args(&args, argv) || !valid_args(argc, argv))
		return (EXIT_FAILURE);
	forks = init_forks(&args);
	philos = init_philos(&args, forks);
	if (!launch_threads_and_join(&args, philos))
		return (EXIT_FAILURE);
	destroy(&args, forks, philos);
	return (EXIT_SUCCESS);
}