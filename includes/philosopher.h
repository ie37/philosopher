/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fboughan <fboughan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 12:40:22 by fboughan          #+#    #+#             */
/*   Updated: 2024/03/18 11:51:36 by fboughan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <memory.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>



typedef struct s_args
{
	int				nbr_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_times;
	pthread_mutex_t	monitoring_mutex;
	bool			simulation_should_end;
}					t_args;

typedef struct s_philo
{
	int				philo_nbr;
	int				eaten_meals;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	suseconds_t		last_meal_time;
	pthread_t		t_id;
	suseconds_t		start_time;
	t_args			*args;
}					t_philo;

typedef enum e_event_id
{
	DEAD,
	EAT,
	THINK,
	SLEEP,
	FORK,
	DROP,
}					t_event_id;

pthread_mutex_t		*init_forks(t_args *args);
t_philo				*init_philos(t_args *args, pthread_mutex_t *forks_array);
bool				launch_threads_and_join(t_args *args, t_philo *philos);
void				*logique(void *_philo);
// void				monitoring(t_philo *philo, t_event_id event_id);
// bool				valid_args(int argc, char **argv);
bool				init_args(t_args *args, char **argv);
suseconds_t			get_time(void);
int					panic(t_args *args, pthread_mutex_t *forks, t_philo *philos,
						char *error_msg);
void				destroy(t_args *args, pthread_mutex_t *forks,
						t_philo *philos);

void				_sleep(t_philo *philo);
int					ft_atoi(const char *nptr);
void				eat(t_philo *philo);
bool				is_philo_dead(t_args *args, t_philo *philo,
						int *satisfied_philos);
void				get_fork(t_philo *philo);
void				drop_fork(t_philo *philo);
void				supervise(t_args *args, t_philo *philos);
void monitoring_2(t_philo *philo, char *str);
bool	is_all_digits(char **argv);

#endif
