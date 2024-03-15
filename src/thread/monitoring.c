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

void monitoring_2(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->args->monitoring_mutex);
	if (philo->args->simulation_should_end)
	{
		pthread_mutex_unlock(&philo->args->monitoring_mutex);
		return ;
	}
	printf("%ld %d %s", (get_time() - philo->start_time), philo->philo_nbr, str);
	pthread_mutex_unlock(&philo->args->monitoring_mutex);
}