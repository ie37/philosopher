/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervise.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fboughan <fboughan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 12:40:22 by fboughan          #+#    #+#             */
/*   Updated: 2024/03/14 14:51:18 by fboughan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosopher.h"

void	supervise(t_args *args, t_philo *philos)
{
	int	satisfied_philos;
	int	i;

	satisfied_philos = 0;
	while (true)
	{
		i = -1;
		pthread_mutex_lock(&args->monitoring_mutex);
		while (++i < args->nbr_of_philo)
		{
			if (is_philo_dead(args, &philos[i], &satisfied_philos))
				return ;
		}
		if (satisfied_philos == args->nbr_of_philo)
		{
			args->simulation_should_end = true;
			printf("Every Philosopher have meals %d times!\n", args->must_eat_times);
			pthread_mutex_unlock(&args->monitoring_mutex);
			return ;
		}
		pthread_mutex_unlock(&args->monitoring_mutex);
	}
}
