#include "../main.h"

int is_dead(t_program *prog_dt)
{
    int dead;

    pthread_mutex_lock(&prog_dt->dead_lock);
    dead = prog_dt->is_dead;
    pthread_mutex_unlock(&prog_dt->dead_lock);
    return dead;
}

int is_full(t_philo *philo)
{
    int full;

    if (philo->prog_dt->max_meals == -1)
        return (0);
    pthread_mutex_lock(&philo->prog_dt->meal_lock);
    full = (philo->meals_eaten >= philo->prog_dt->max_meals);
    pthread_mutex_unlock(&philo->prog_dt->meal_lock);
    return (full);
}

void print_action(t_philo *philo, const char *action)
{
    int timestamp;

    timestamp = get_cur_time() - philo->prog_dt->start_time;
    pthread_mutex_lock(&philo->prog_dt->write_lock);
    if (!is_dead(philo->prog_dt))
        ft_printf("%d %d %s\n", timestamp, philo->id, action);
    pthread_mutex_unlock(&philo->prog_dt->write_lock);
}

void ft_sleep(size_t milliseconds, t_philo *philo)
{
    size_t start;

    start = get_cur_time();
    while ((get_cur_time() - start) < milliseconds)
    {
        if (is_dead(philo->prog_dt))
            return;
        usleep(1);
    }
}

int64_t get_cur_time(void)
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) != 0)
        return -1;
    return (int64_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
