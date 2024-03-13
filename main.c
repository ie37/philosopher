/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fboughan <fboughan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 12:40:22 by nnuno-ca          #+#    #+#             */
/*   Updated: 2024/03/13 14:11:28 by fboughan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static bool	is_philo_dead(t_args *args, t_philo *philo, int *satisfied_philos);
static inline void	all_have_eaten(t_args *args);

#define TAKE_FORK_STR "has taken a fork 🍴\n"
#define DROP_FORK_STR "has dropped a fork 🍴\n"
#define EAT_STR "is eating 😋\n"
#define THINK_STR "is thinking 🤔\n"
#define SLEEP_STR "is sleeping 😴\n"
#define DEAD_STR "is dead 😵\n"

static void	free_forks_until(pthread_mutex_t *forks, int until)
{
	int	i;

	i = 0;
	while (i <= until)
	{
		pthread_mutex_destroy(&forks[i]);
		i += 1;
	}
	free(forks);
}

pthread_mutex_t	*init_forks(t_args *args)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(args->nbr_of_philo * sizeof(pthread_mutex_t));
	if (!forks)

	i = 0;
	while (i < args->nbr_of_philo)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			free_forks_until(forks, i);
			
		}
		i += 1;
	}
	return (forks);
}

suseconds_t	get_time(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec * 1000) + (now.tv_usec / 1000));
}

int ft_atoi(const char *nptr)
{
    int i;
    int m;
    int p;
    int res;

    res = 0;
    p = 0;
    m = 0;
    i = 0;
    while (nptr[i] == 32 || (nptr[i] >= 9 && nptr[i] <= 13))
        i++;
    if (nptr[i] == '-' || nptr[i] == '+')
    {
        if (nptr[i] == '-')
            m++;
        else
            p++;
        i++;
    }
    while (nptr[i] >= '0' && nptr[i] <= '9')
        res = res * 10 + nptr[i++] - '0';
    if (m > p)
        return (-res);
    else
        return (res);
}

bool	init_args(t_args *args, char **argv)
{
	args->nbr_of_philo = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	args->must_eat_times = -1;
	if (argv[5])
		args->must_eat_times = ft_atoi(argv[5]);
	if (args->nbr_of_philo == 0 || args->must_eat_times == 0)
	{
		return (false);
	}
	if (pthread_mutex_init(&args->monitoring_mutex, NULL) != 0)
	{
		return (false);
	}
	args->simulation_should_end = false;
	return (true);
}

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
		philos[i].left_fork = &forks_array[i];
		philos[i].right_fork = &forks_array[(i + 1) % args->nbr_of_philo];
		philos[i].last_meal_time = get_time();
		philos[i].args = args;
		i += 1;
	}
	return (philos);
}

static void	eat(t_philo *philo)
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
	pthread_mutex_lock(&philo->args->monitoring_mutex);
	philo->last_meal_time = get_time();
	philo->eaten_meals += 1;
	pthread_mutex_unlock(&philo->args->monitoring_mutex);
	monitoring(philo, EAT);
	usleep(philo->args->time_to_eat * MICROSEC);
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

static inline void	_sleep(t_philo *philo)
{
	monitoring(philo, SLEEP);
	usleep(philo->args->time_to_sleep * MICROSEC);
}

void	*routine(void *_philo)
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

void	monitoring(t_philo *philo, t_event_id event_id)
{
	suseconds_t	timestamp;
	const char	*events[6] = {DEAD_STR, EAT_STR, THINK_STR,
		SLEEP_STR, TAKE_FORK_STR, DROP_FORK_STR};

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

int	main(int argc, char **argv)
{
	t_args			args;
	pthread_mutex_t	*forks;
	t_philo			*philos;

	if ( !init_args(&args, argv))
		return (EXIT_FAILURE);
	forks = init_forks(&args);
	philos = init_philos(&args, forks);
	if (!launch_threads_and_join(&args, philos, forks))
		return (EXIT_FAILURE);
	//destroy(&args, forks, philos);
	return (EXIT_SUCCESS);
}

void supervise(t_args *args, t_philo *philos)
{
    int	satisfied_philos;
    int i;
    
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
			return (all_have_eaten(args));
		pthread_mutex_unlock(&args->monitoring_mutex);
	}
}

bool	launch_threads_and_join(t_args *args, t_philo *philos, pthread_mutex_t *forks)
{
	int	i;
    int	satisfied_philos;

	i = -1;
	while (++i < args->nbr_of_philo)
	{
		philos[i].start_time = get_time();
		pthread_create(&philos[i].t_id, NULL, routine, (void *)&philos[i]);
		
	}
    supervise(args, philos);
	// satisfied_philos = 0;
	// while (true)
	// {
	// 	i = -1;
	// 	pthread_mutex_lock(&args->monitoring_mutex);
	// 	while (++i < args->nbr_of_philo)
	// 	{
	// 		if (is_philo_dead(args, &philos[i], &satisfied_philos))
	// 			return ;
	// 	}
	// 	if (satisfied_philos == args->nbr_of_philo)
	// 		return (all_have_eaten(args));
	// 	pthread_mutex_unlock(&args->monitoring_mutex);
	// }
	i = -1;
	while (++i < args->nbr_of_philo)
		pthread_join(philos[i].t_id, NULL);
	return (true);
}

static bool	is_philo_dead(t_args *args, t_philo *philo, int *satisfied_philos)
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

static inline void	all_have_eaten(t_args *args)
{
	args->simulation_should_end = true;
	printf("Every Philosopher had %d meals!\n", args->must_eat_times);
	pthread_mutex_unlock(&args->monitoring_mutex);
}