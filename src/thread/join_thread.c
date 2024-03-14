/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fboughan <fboughan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 12:40:22 by fboughan          #+#    #+#             */
/*   Updated: 2024/03/14 14:50:18 by fboughan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosopher.h"

bool	launch_threads_and_join(t_args *args, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < args->nbr_of_philo)
	{
		philos[i].start_time = get_time();
		pthread_create(&philos[i].t_id, NULL, logique, (void *)&philos[i]);
	}
	supervise(args, philos);
	i = -1;
	while (++i < args->nbr_of_philo)
		pthread_join(philos[i].t_id, NULL);
	return (true);
}
