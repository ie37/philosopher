/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fboughan <fboughan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 12:40:22 by fboughan          #+#    #+#             */
/*   Updated: 2024/03/18 11:41:42 by fboughan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosopher.h"

static void create_thread(t_args *args, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < args->nbr_of_philo)
	{
		philos[i].start_time = get_time();
		pthread_create(&philos[i].t_id, NULL, logique, (void *)&philos[i]);
	}
}

bool	launch_threads_and_join(t_args *args, t_philo *philos)
{
	int	i;

	create_thread(args, philos);
	supervise(args, philos);
	i = -1;
	while (++i < args->nbr_of_philo)
		pthread_join(philos[i].t_id, NULL);
	return (true);
}
