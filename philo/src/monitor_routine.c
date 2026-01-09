/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-05 10:50:52 by akuchmam          #+#    #+#             */
/*   Updated: 2026-01-05 10:50:52 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../main.h"

static int	check_all_philos_full(t_program *prog_dt)
{
	int	i;
	int	all_ate;

	i = 0;
	all_ate = 1;
	while (i < prog_dt->num_of_philos)
	{
		pthread_mutex_lock(&prog_dt->meal_lock);
		if (prog_dt->philos[i].meals_eaten < prog_dt->max_meals
			|| prog_dt->max_meals == -1)
			all_ate = 0;
		pthread_mutex_unlock(&prog_dt->meal_lock);
		i++;
	}
	return (all_ate);
}

static int	check_some_philo_dead(t_program *prog_dt)
{
	int	i;

	i = 0;
	while (i < prog_dt->num_of_philos)
	{
		pthread_mutex_lock(&prog_dt->meal_lock);
		if ((get_cur_time() - prog_dt->philos[i].last_meal_time)
			> prog_dt->time_to_die)
		{
			pthread_mutex_lock(&prog_dt->write_lock);
			printf("%zu %d died\n", get_cur_time() - prog_dt->start_time,
				prog_dt->philos[i].id);
			pthread_mutex_lock(&prog_dt->dead_lock);
			prog_dt->is_dead = 1;
			pthread_mutex_unlock(&prog_dt->dead_lock);
			pthread_mutex_unlock(&prog_dt->write_lock);
			pthread_mutex_unlock(&prog_dt->meal_lock);
			return (1);
		}
		pthread_mutex_unlock(&prog_dt->meal_lock);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_program	*prog_dt;

	prog_dt = (t_program *)arg;
	while (!is_dead(prog_dt))
	{
		if (check_some_philo_dead(prog_dt))
			break ;
		if (check_all_philos_full(prog_dt))
		{
			pthread_mutex_lock(&prog_dt->write_lock);
			pthread_mutex_lock(&prog_dt->dead_lock);
			prog_dt->is_dead = 1;
			printf("All philosophers full\n");
			pthread_mutex_unlock(&prog_dt->dead_lock);
			pthread_mutex_unlock(&prog_dt->write_lock);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
