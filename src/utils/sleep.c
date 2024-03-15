/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fboughan <fboughan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 12:40:22 by fboughan          #+#    #+#             */
/*   Updated: 2024/03/14 14:54:02 by fboughan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosopher.h"

void	_sleep(t_philo *philo)
{
	monitoring_2(philo, "is sleeping \n");
	usleep(philo->args->time_to_sleep * 1000);
}
