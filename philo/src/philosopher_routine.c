/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_routine.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akuchmam <akuchmam@student.hive.fi>        #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-01-05 10:49:36 by akuchmam          #+#    #+#             */
/*   Updated: 2026-01-05 10:49:36 by akuchmam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../main.h"

static void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken the right fork");
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken the left fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken the left fork");
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken the right fork");
	}
}

static void	start_eating(t_philo *philo)
{
	take_forks(philo);
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->prog_dt->meal_lock);
	philo->last_meal_time = get_cur_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->prog_dt->meal_lock);
	ft_sleep(philo->prog_dt->time_to_eat, philo);
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
}

static void	handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, "has taken the left fork");
	while (!is_dead(philo->prog_dt))
		ft_sleep(1000, philo);
	pthread_mutex_unlock(philo->left_fork);
}

static int	calc_think_time(t_philo *philo)
{
	long long	time_to_think;

	time_to_think = (philo->prog_dt->time_to_die - philo->prog_dt->time_to_eat
			- philo->prog_dt->time_to_sleep);
	if (time_to_think <= 0)
		return (0);
	return (time_to_think / 2);
}

void	*philosopher_routine(void *arg)
{
	t_philo		*philo;
	const int	time_to_think = calc_think_time((t_philo *)arg);

	philo = (t_philo *)arg;
	if (philo->prog_dt->num_of_philos == 1)
		return (handle_single_philo(&philo->prog_dt->philos[0]), NULL);
	if (philo->id % 2 == 0)
		ft_sleep(philo->prog_dt->time_to_eat / 5, philo);
	while (!is_dead(philo->prog_dt) && !is_full(philo))
	{
		start_eating(philo);
		if (is_dead(philo->prog_dt) || is_full(philo))
			break ;
		print_action(philo, "is sleeping");
		ft_sleep(philo->prog_dt->time_to_sleep, philo);
		if (is_dead(philo->prog_dt) || is_full(philo))
			break ;
		print_action(philo, "is thinking");
		ft_sleep(time_to_think, philo);
	}
	return (NULL);
}
