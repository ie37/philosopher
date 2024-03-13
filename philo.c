#include "philosopher.h"

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

suseconds_t	get_time(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec * 1000) + (now.tv_usec / 1000));
}

pthread_mutex_t *init_fork(t_args *args)
{
    pthread_mutex_t *forks;
    int             i;

    forks = malloc(args->philo_nbr * sizeof(pthread_mutex_t));
    i = 0;
    while (i < args->philo_nbr)
    {
        pthread_mutex_init(&forks[i], NULL);
        i++;
    }
    return (forks);
}

t_philo *init_philo(t_args *args, pthread_mutex_t *forks_array)
{
    t_philo *philos;
    int i;
    philos = malloc(args->philo_nbr * sizeof(t_philo));
    i = 0;
    while (i < args->philo_nbr)
    {
        philos[i].philo_nbr = i + 1;
		philos[i].nbr_eaten = 0;
		philos[i].left_fork = &forks_array[i];
		philos[i].right_fork = &forks_array[(i + 1) % args->philo_nbr];
		philos[i].last_time_meal = get_time();
		philos[i].args = args;
        i++;
    }
    return (philos);
}

bool	is_die(t_philo *philo)
{
	return (((get_time() - philo->last_time_meal) >= philo->args->time_to_die));
}

int init_args(t_args *args, char *argv[], int argc)
{
    args->philo_nbr = ft_atoi(argv[1]);
    args->time_to_die = ft_atoi(argv[2]);
    args->time_to_eat = ft_atoi(argv[3]);
    args->time_to_sleep =  ft_atoi(argv[4]);
    if (argc == 6)
        args->max_nbr_eat = ft_atoi(argv[5]);
    pthread_mutex_init(&args->monitor_mutex, NULL);
    args->end = 0;
}

#define FORK "has taken a fork\n"
#define DROP "has dropped a fork\n"
#define EAT "is eating\n"
#define THINK "is Thinkg\n"
#define SLEEP "is sleeping\n"
#define DEAD "is dead\n"

int monitoring(t_philo *philo, char *state)
{
    suseconds_t	timestamp;
    pthread_mutex_lock(&philo->args->monitor_mutex);
	if (philo->args->end)
	{
		pthread_mutex_unlock(&philo->args->monitor_mutex);
		return (0);
	}
	timestamp = get_time() - philo->first_aper;
	printf("%ld %d %s", timestamp, philo->philo_nbr, state);
	pthread_mutex_unlock(&philo->args->monitor_mutex);
}

static inline void	_sleep(t_philo *philo)
{
	monitoring(philo, SLEEP);
	usleep(philo->args->time_to_sleep * 1000);
}

void	*logique(void *_philo)
{
	t_philo	*philo;

	philo = (t_philo *)_philo;
	if (philo->args->philo_nbr == 1)
	{
		monitoring(philo, FORK);
		return (NULL);
	}
	while (true)
	{
		pthread_mutex_lock(&philo->args->monitor_mutex);
		if (philo->args->end)
		{
			pthread_mutex_unlock(&philo->args->monitor_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->args->monitor_mutex);
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
		pthread_mutex_lock(&philo->args->monitor_mutex);
		philo->last_time_meal = get_time();
		philo->nbr_eaten += 1;
		pthread_mutex_unlock(&philo->args->monitor_mutex);
		monitoring(philo, EAT);
		usleep(philo->args->time_to_eat * 1000);
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
		_sleep(philo);
		monitoring(philo, THINK);
	}
	return (NULL);
}

static inline void	all_have_eaten(t_args *args)
{
	args->end = true;
	printf("Every Philosopher had %d meals!\n", args->nbr_eaten);
	pthread_mutex_unlock(&args->monitor_mutex);
}

static inline bool	starved(t_philo *philo)
{
	return (((get_time() - philo->last_time_meal) >= philo->args->time_to_die));
}

static bool	is_philo_dead(t_args *args, t_philo *philo, int *satisfied_philos)
{
	if (args->max_nbr_eat > 0 && philo->nbr_eaten >= args->max_nbr_eat)
		*satisfied_philos += 1;
	if (starved(philo))
	{
		pthread_mutex_unlock(&args->monitor_mutex);
		monitoring(philo, DEAD);
		pthread_mutex_lock(&args->monitor_mutex);
		args->end = true;
		pthread_mutex_unlock(&args->monitor_mutex);
		return (true);
	}
	return (false);
}

static void	supervise(t_philo *philos, t_args *args)
{
	int	satisfied_philos;
	int	i;

	satisfied_philos = 0;
	while (true)
	{
		i = -1;
		pthread_mutex_lock(&args->monitor_mutex);
		while (++i < args->philo_nbr)
		{
			if (is_philo_dead(args, &philos[i], &satisfied_philos))
				return ;
		}
		if (satisfied_philos == args->philo_nbr)
			return (all_have_eaten(args));
		pthread_mutex_unlock(&args->monitor_mutex);
	}
}

bool	launch_threads_and_join(t_args *args, t_philo *philos, pthread_mutex_t *forks)
{
	int	i;

	i = -1;
	while (++i < args->philo_nbr)
	{
		philos[i].first_aper = get_time();
		pthread_create(&philos[i].thread_id, NULL, logique, (void *)&philos[i]);
	}
	supervise(philos, args);
	i = -1;
	while (++i < args->philo_nbr)
		pthread_join(philos[i].thread_id, NULL);
	return (true);
}

int main(int argc, char *argv[])
{
    t_args  args;
    pthread_mutex_t *forks;
    t_philo *philos;
    int i;

    init_args(&args, argv, argc);
    forks = init_fork(&args);
    philos = init_philo(&args, forks);
    if (!launch_threads_and_join(&args, philos, forks))
    {
        fprintf(stderr, "Error launching threads.\n");
        return 1;
    }
    free(forks);
    free(philos);
    return 0;
}