/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_dead.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fboughan <fboughan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 12:40:22 by fboughan          #+#    #+#             */
/*   Updated: 2024/03/18 11:41:42 by fboughan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosopher.h"

bool	is_philo_dead(t_args *args, t_philo *philo, int *satisfied_philos)
{
	if (args->must_eat_times > 0 && philo->eaten_meals >= args->must_eat_times)
		*satisfied_philos += 1;
	if (((get_time() - philo->last_meal_time) >= philo->args->time_to_die))
	{
		pthread_mutex_unlock(&args->monitoring_mutex);
		monitoring_2(philo, "is dead \n");
		pthread_mutex_lock(&args->monitoring_mutex);
		args->simulation_should_end = true;
		pthread_mutex_unlock(&args->monitoring_mutex);
		return (true);
	}
	return (false);
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->args->monitoring_mutex);
	philo->last_meal_time = get_time();
	philo->eaten_meals += 1;
	pthread_mutex_unlock(&philo->args->monitoring_mutex);
	monitoring_2(philo, "is eating \n");
	usleep(philo->args->time_to_eat * 1000);
}
