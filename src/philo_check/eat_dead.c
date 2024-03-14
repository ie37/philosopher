/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_dead.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fboughan <fboughan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 12:40:22 by fboughan          #+#    #+#             */
/*   Updated: 2024/03/14 14:49:44 by fboughan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosopher.h"

bool	starved(t_philo *philo)
{
	return (((get_time() - philo->last_meal_time) >= philo->args->time_to_die));
}

bool	is_philo_dead(t_args *args, t_philo *philo, int *satisfied_philos)
{
	if (args->must_eat_times > 0 && philo->eaten_meals >= args->must_eat_times)
		*satisfied_philos += 1;
	if (starved(philo))
	{
		pthread_mutex_unlock(&args->monitoring_mutex);
		monitoring(philo, DEAD);
		pthread_mutex_lock(&args->monitoring_mutex);
		args->simulation_should_end = true;
		pthread_mutex_unlock(&args->monitoring_mutex);
		return (true);
	}
	return (false);
}

void	all_have_eaten(t_args *args)
{
	args->simulation_should_end = true;
	printf("Every Philosopher had %d meals!\n", args->must_eat_times);
	pthread_mutex_unlock(&args->monitoring_mutex);
}

void	eat(t_philo *philo)
{
	get_fork(philo);
	pthread_mutex_lock(&philo->args->monitoring_mutex);
	philo->last_meal_time = get_time();
	philo->eaten_meals += 1;
	pthread_mutex_unlock(&philo->args->monitoring_mutex);
	monitoring(philo, EAT);
	usleep(philo->args->time_to_eat * MICROSEC);
	drop_fork(philo);
}
