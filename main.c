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

void *thread_philosophe(void *arg)
{
    Philosopher *philosophe = (Philosopher *)arg;
    while (1)
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        philosophe->state = PENSANT;
        printf("%ld  [%d]   is thinking\n", (tv.tv_sec * 1000 + tv.tv_usec / 1000), philosophe->id);
        if (philosophe->argvp[5] != NULL)
            usleep(ft_atoi(philosophe->argvp[5]) * 1000);
        philosophe->state = FAIM;
        int left_fork = (philosophe->id - 1) % ft_atoi(philosophe->argvp[1]);
        int right_fork = philosophe->id;
        printf("%ld  [%d]  has taken a fork\n", (tv.tv_sec * 1000 + tv.tv_usec / 1000), philosophe->id);
        if (philosophe->id == 1)
        {
            pthread_mutex_lock(&(philosophe->fourchettes[right_fork]));
            pthread_mutex_lock(&(philosophe->fourchettes[left_fork]));
        }
        else
        {
            pthread_mutex_lock(&(philosophe->fourchettes[left_fork]));
            pthread_mutex_lock(&(philosophe->fourchettes[right_fork]));
        }
        printf("%ld  [%d]  is eating\n", (tv.tv_sec * 1000 + tv.tv_usec / 1000), philosophe->id);
        philosophe->state = MANGE;
        usleep(ft_atoi(philosophe->argvp[3]) * 1000);
        if (philosophe->id == 1)
        {
            pthread_mutex_unlock(&(philosophe->fourchettes[left_fork]));
            pthread_mutex_unlock(&(philosophe->fourchettes[right_fork]));
        }
        else
        {
            pthread_mutex_unlock(&(philosophe->fourchettes[left_fork]));
            pthread_mutex_unlock(&(philosophe->fourchettes[right_fork]));
        }
        philosophe->state = DORS;
        printf("%ld  [%d]   is sleeping\n", (tv.tv_sec * 1000 + tv.tv_usec / 1000), philosophe->id);
        if (philosophe->argvp[5] != NULL)
            usleep(ft_atoi(philosophe->argvp[5]) * 1000);
    }
}

bool philosopher_eat_or_die(Philosopher *philosopher, char *argv[])
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long current_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    long time_since_last_meal = current_time - philosopher->last_meal_time;
    return time_since_last_meal >= ft_atoi(argv[2]);
}

void initialize_mutexes(pthread_mutex_t *mutexes, int count)
{
    for (int i = 0; i < count; i++)
    {
        pthread_mutex_init(&mutexes[i], NULL);
    }
}

void destroy_mutexes(pthread_mutex_t *mutexes, int count)
{
    for (int i = 0; i < count; i++)
    {
        pthread_mutex_destroy(&mutexes[i]);
    }
}

suseconds_t	get_time(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec * 1000) + (now.tv_usec / 1000));
}

int main(int argc, char *argv[])
{
    int i;
    i = 0;
    int number_of_philosopher = ft_atoi(argv[1]);
    printf("nb philo = %d\n", number_of_philosopher);

    Philosopher *philosophers = malloc(number_of_philosopher * sizeof(Philosopher));
    pthread_mutex_t *fourchettes = malloc(number_of_philosopher * sizeof(pthread_mutex_t));

    initialize_mutexes(fourchettes, number_of_philosopher);

    while (i < number_of_philosopher)
    {
        philosophers[i].id = i + 1;
        philosophers[i].state = PENSANT;
        philosophers[i].times_eaten = 0;
        philosophers[i].times_slept = 0;
        philosophers[i].argvp = argv;
        philosophers[i].fourchettes = fourchettes;
        philosophers[i].first_aper = get_time();
        i++;
    }
    i = 0;
    while (i < number_of_philosopher)
    {
        pthread_create(&philosophers[i].thread, NULL, thread_philosophe, &philosophers[i]);
        i++;
    }
    while (1)
    {
        i = 0;
        while (i < number_of_philosopher)
        {
            if (philosophers[i].state == FAIM && philosopher_eat_or_die(&philosophers[i], argv))
            {
                printf("%ld %d est mort\n", get_time(), philosophers[i].id);
                destroy_mutexes(fourchettes, number_of_philosopher);
                free(fourchettes);
                exit(EXIT_SUCCESS);
            }
            i++;
        }
        usleep(10000);
    }
}