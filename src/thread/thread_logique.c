/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_logique.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fboughan <fboughan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 12:40:22 by fboughan          #+#    #+#             */
/*   Updated: 2024/03/14 14:51:51 by fboughan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosopher.h"

void	get_fork(t_philo *philo)
{
	if (philo->philo_nbr % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		monitoring(philo, FORK);
		pthread_mutex_lock(philo->right_fork);
		monitoring(philo, FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		monitoring(philo, FORK);
		pthread_mutex_lock(philo->left_fork);
		monitoring(philo, FORK);
	}
}

void	drop_fork(t_philo *philo)
{
	if (philo->philo_nbr % 2 == 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		monitoring(philo, DROP);
		pthread_mutex_unlock(philo->right_fork);
		monitoring(philo, DROP);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		monitoring(philo, DROP);
		pthread_mutex_unlock(philo->left_fork);
		monitoring(philo, DROP);
	}
}

void	*logique(void *_philo)
{
	t_philo	*philo;

	philo = (t_philo *)_philo;
	if (philo->args->nbr_of_philo == 1)
	{
		monitoring(philo, FORK);
		return (NULL);
	}
	while (true)
	{
		pthread_mutex_lock(&philo->args->monitoring_mutex);
		if (philo->args->simulation_should_end)
		{
			pthread_mutex_unlock(&philo->args->monitoring_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->args->monitoring_mutex);
		eat(philo);
		_sleep(philo);
		monitoring(philo, THINK);
	}
	return (NULL);
}
