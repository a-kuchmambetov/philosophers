/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-05 11:05:03 by akuchmam          #+#    #+#             */
/*   Updated: 2026-01-05 11:05:03 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <pthread.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum e_error
{
	ERR_INVALID_ARG_COUNT = 1,
	ERR_INVALID_ARG_VALUES,
	ERR_MEMORY_ALLOCATION,
	ERR_THREAD_CREATION,
	ERR_MUTEX_INIT
}	t_error;

typedef struct s_philo
{
	int					id;
	int					meals_eaten;
	int64_t				last_meal_time;
	pthread_t			thread;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	struct s_program	*prog_dt;
}	t_philo;

typedef struct s_program
{
	int				num_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				max_meals;
	int				is_dead;
	int64_t			start_time;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	*forks;
	t_philo			*philos;
	pthread_t		monitor_thread;
}	t_program;

// Thread utilities
int		is_dead(t_program *prog_dt);
int		is_full(t_philo *philo);
void	print_action(t_philo *philo, const char *action);
void	ft_sleep(size_t milliseconds, t_philo *philo);
int64_t	get_cur_time(void);

// Initialization
int ft_atoi(const char *nptr);
void	init_program(t_program *prog_dt, int argc, char **argv, int *errno);
void	destroy_mutexes(t_program *prog_dt, int initialized_count);

// Philosopher routine
void	*philosopher_routine(void *arg);

// Monitor routine
void	*monitor_routine(void *arg);
