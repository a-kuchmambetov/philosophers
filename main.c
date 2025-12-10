#include "main.h"

static int start_threads(t_program *prog_dt)
{
    const int64_t start_time = get_cur_time();
    int i;

    prog_dt->start_time = start_time;
    i = 0;
    while (i < prog_dt->num_of_philos)
    {
        prog_dt->philos[i].last_meal_time = start_time;
        if (pthread_create(&prog_dt->philos[i].thread, NULL, philosopher_routine, &prog_dt->philos[i]) != 0)
        {
            pthread_mutex_lock(&prog_dt->dead_lock);
            prog_dt->is_dead = 1;
            pthread_mutex_unlock(&prog_dt->dead_lock);
            return ERR_THREAD_CREATION;
        }
        i++;
    }
    if (pthread_create(&prog_dt->monitor_thread, NULL, monitor_routine, prog_dt) != 0)
    {
        pthread_mutex_lock(&prog_dt->dead_lock);
        prog_dt->is_dead = 1;
        pthread_mutex_unlock(&prog_dt->dead_lock);
        return ERR_THREAD_CREATION;
    }
    return 0;
}

static int join_threads(t_program *prog_dt)
{
    int i;

    i = 0;
    while (i < prog_dt->num_of_philos)
    {
        pthread_join(prog_dt->philos[i].thread, NULL);
        i++;
    }
    pthread_join(prog_dt->monitor_thread, NULL);
    return 0;
}

int main(int argc, char **argv)
{
    t_program prog_dt;
    int errno;

    errno = 0;
    init_program(&prog_dt, argc, argv, &errno);
    if (errno)
        return errno;
    errno = start_threads(&prog_dt);
    if (errno)
    {
        destroy_mutexes(&prog_dt, prog_dt.num_of_philos);
        free(prog_dt.philos);
        return errno;
    }
    join_threads(&prog_dt);
    destroy_mutexes(&prog_dt, prog_dt.num_of_philos);
    free(prog_dt.philos);
    return 0;
}
