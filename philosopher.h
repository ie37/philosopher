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

#endif
