# ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>

typedef struct philosopher {
 int id;
 enum state { PENSANT, FAIM, MANGE, DORS } state;
 int times_eaten;
 int times_slept;
 long last_meal_time;
 long first_aper;
 pthread_t thread;
 char **argvp;
 pthread_mutex_t *fourchettes;
} Philosopher;

typedef struct t_args 
{
    int last_meal_time;
    int philo_nbr;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int max_nbr_eat;
    int end;
    pthread_mutex_t	monitor_mutex;
}               t_args;

typedef struct t_philo
{
    int philo_nbr;
    int last_time_meal;
    int nbr_eaten;
    pthread_mutex_t *right_fork;
    pthread_mutex_t *left_fork;
    int first_aper;
    pthread_t	thread_id;
    t_args *args;
}               t_philo;
#endif
