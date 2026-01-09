/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-05 10:56:38 by akuchmam          #+#    #+#             */
/*   Updated: 2026-01-05 10:56:38 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../main.h"

void	destroy_mutexes(t_program *prog_dt, int num_of_mutexes)
{
	int	i;

	pthread_mutex_destroy(&prog_dt->write_lock);
	pthread_mutex_destroy(&prog_dt->meal_lock);
	pthread_mutex_destroy(&prog_dt->dead_lock);
	i = 0;
	while (i < num_of_mutexes)
	{
		pthread_mutex_destroy(&prog_dt->forks[i]);
		i++;
	}
	if (prog_dt->forks)
		free(prog_dt->forks);
	return ;
}

static void	parse_args(t_program *program_dt, int argc, char **argv, int *errno)
{
	if (argc < 5 || argc > 6)
	{
		*errno = ERR_INVALID_ARG_COUNT;
		return ;
	}
	program_dt->num_of_philos = ft_atoi(argv[1]);
	if (program_dt->num_of_philos > 200)
	{
		*errno = ERR_INVALID_ARG_VALUES;
		return ;
	}
	program_dt->time_to_die = ft_atoi(argv[2]);
	program_dt->time_to_eat = ft_atoi(argv[3]);
	program_dt->time_to_sleep = ft_atoi(argv[4]);
	program_dt->max_meals = -1;
	if (argc == 6)
		program_dt->max_meals = ft_atoi(argv[5]);
	if (program_dt->num_of_philos <= 0 || program_dt->time_to_die <= 0
		|| program_dt->time_to_eat <= 0 || program_dt->time_to_sleep <= 0
		|| (argc == 6 && program_dt->max_meals <= 0))
		*errno = ERR_INVALID_ARG_VALUES;
}

static int	init_mutexes(t_program *prog_dt)
{
	int	i;

	if (pthread_mutex_init(&prog_dt->write_lock, NULL) != 0)
		return (ERR_MUTEX_INIT);
	if (pthread_mutex_init(&prog_dt->meal_lock, NULL) != 0)
		return (pthread_mutex_destroy(&prog_dt->write_lock), ERR_MUTEX_INIT);
	if (pthread_mutex_init(&prog_dt->dead_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&prog_dt->write_lock);
		pthread_mutex_destroy(&prog_dt->meal_lock);
		return (ERR_MUTEX_INIT);
	}
	prog_dt->forks = malloc(sizeof(pthread_mutex_t) * prog_dt->num_of_philos);
	if (!prog_dt->forks)
		return (destroy_mutexes(prog_dt, 0), ERR_MEMORY_ALLOCATION);
	i = 0;
	while (i < prog_dt->num_of_philos)
	{
		if (pthread_mutex_init(&prog_dt->forks[i], NULL) != 0)
			return (destroy_mutexes(prog_dt, i), ERR_MUTEX_INIT);
		i++;
	}
	return (0);
}

static int	init_philos(t_program *prog_dt)
{
	int	i;

	prog_dt->philos = malloc(sizeof(t_philo) * prog_dt->num_of_philos);
	if (!prog_dt->philos)
		return (ERR_MEMORY_ALLOCATION);
	i = 0;
	while (i < prog_dt->num_of_philos)
	{
		prog_dt->philos[i].id = i + 1;
		prog_dt->philos[i].meals_eaten = 0;
		prog_dt->philos[i].last_meal_time = 0;
		prog_dt->philos[i].left_fork = &prog_dt->forks[i];
		prog_dt->philos[i].right_fork = &prog_dt->forks[
			(i + 1) % prog_dt->num_of_philos];
		prog_dt->philos[i].prog_dt = prog_dt;
		i++;
	}
	return (0);
}

void	init_program(t_program *prog_dt, int argc, char **argv, int *errno)
{
	*prog_dt = (t_program){0};
	parse_args(prog_dt, argc, argv, errno);
	if (*errno)
		return ;
	*errno = init_mutexes(prog_dt);
	if (*errno)
		return ;
	*errno = init_philos(prog_dt);
	if (*errno)
	{
		destroy_mutexes(prog_dt, prog_dt->num_of_philos);
		return ;
	}
}
