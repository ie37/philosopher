/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fboughan <fboughan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 12:40:22 by fboughan          #+#    #+#             */
/*   Updated: 2024/03/18 13:15:56 by fboughan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosopher.h"

t_philo	*init_philos(t_args *args, pthread_mutex_t *forks_array)
{
	t_philo	*philos;
	int		i;

	philos = malloc(args->nbr_of_philo * sizeof(t_philo));
	i = 0;
	while (i < args->nbr_of_philo)
	{
		philos[i].philo_nbr = i + 1;
		philos[i].eaten_meals = 0;
        if ((i + 1) % 2 == 0)
        {
            philos[i].right_fork = &forks_array[i];
		    philos[i].left_fork = &forks_array[(i + 1) % args->nbr_of_philo];
        }
        else
        {
            philos[i].left_fork = &forks_array[i];
            philos[i].right_fork = &forks_array[(i + 1) % args->nbr_of_philo];
        }
		philos[i].last_meal_time = get_time();
		philos[i].args = args;
		i += 1;
	}
	return (philos);
}
