/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fboughan <fboughan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 12:40:22 by fboughan          #+#    #+#             */
/*   Updated: 2024/03/14 14:50:51 by fboughan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosopher.h"

void	monitoring(t_philo *philo, t_event_id event_id)
{
	suseconds_t	timestamp;
	const char	*events[6] = {DEAD_STR, EAT_STR, THINK_STR, SLEEP_STR,
		TAKE_FORK_STR, DROP_FORK_STR};

	pthread_mutex_lock(&philo->args->monitoring_mutex);
	if (philo->args->simulation_should_end)
	{
		pthread_mutex_unlock(&philo->args->monitoring_mutex);
		return ;
	}
	timestamp = get_time() - philo->start_time;
	printf("%ld %d %s", timestamp, philo->philo_nbr, events[event_id]);
	pthread_mutex_unlock(&philo->args->monitoring_mutex);
}
