#include "modeling.h"

typedef struct element
{
    priority_t priority;
    size_t order;
} element_t;

double get_first_task_cooldown(void)
{
    return TYPE1_TASK_MIN_ARRIVAL_TIME + ((double)rand()) / (double)RAND_MAX * TYPE1_TASK_MAX_ARRIVAL_TIME;
}

double get_first_task_required(void)
{
    return TYPE1_TASK_MIN_SERVICE_TIME + ((double)rand()) / (double)RAND_MAX * TYPE1_TASK_MAX_SERVICE_TIME;
}

double get_second_task_required(void)
{
    return TYPE2_TASK_MIN_SERVICE_TIME + ((double)rand()) / (double)RAND_MAX * TYPE2_TASK_MAX_SERVICE_TIME;
}

void print_mid_results(results_t *results)
{
    printf("Кол-во вышедших заявок: %zu", results->first_task_out_amount);
    print_newline();
    puts("1 ТИП");
    printf("ВОШЛО: %lu\n", results->first_task_in_amount);
    printf("ВЫШЛО: %lu\n", results->first_task_out_amount);
    printf("СРЕДНЕЕ ВРЕМЯ ОЖИДАНИЯ (е.в.): %.3lf\n", results->first_task_sum_service / results->first_task_out_amount);
    puts("2 ТИП");
    printf("ВОШЛО: %lu\n", results->second_task_in_amount);
    printf("ВЫШЛО: %lu\n", results->second_task_out_amount);
    printf("СРЕДНЕЕ ВРЕМЯ ОЖИДАНИЯ (е.в.): %.3lf\n", results->second_task_sum_service / results->second_task_out_amount);
    printf("ДЛИНА очереди: %lu\n", results->queue_length);
    printf("СРЕДНЯЯ ДЛИНА очереди: %.3lf\n", ((double)results->queue_sum_length) /
                                             (results->first_task_out_amount + results->second_task_out_amount));
    printf("Среднее время ожидания заявок (е.в.): %.3lf\n", (results->first_task_sum_service + results->second_task_sum_service) /
                                                                      (results->first_task_out_amount + results->second_task_out_amount));
}

void print_total_results(results_t *results)
{
    double service_time = results->first_task_sum_service + results->second_task_sum_service;
    double expected_service_time = (TYPE1_TASK_MIN_SERVICE_TIME + TYPE1_TASK_MAX_SERVICE_TIME) / 2.0f * results->first_task_out_amount +
                                   (TYPE2_TASK_MIN_SERVICE_TIME + TYPE2_TASK_MAX_SERVICE_TIME) / 2.0f * results->first_task_out_amount / 3.0;
    double expected_arrival_time = expected_service_time;
    double hold_time = service_time - results->first_task_sum_arrival;
    double delta_arrival = fabs(results->first_task_sum_arrival - expected_arrival_time) / expected_arrival_time * 100;
    double delta_modeling = fabs(expected_service_time - service_time) / expected_service_time * 100;

    puts("ИТОГ");
    printf("Время прихода (е.в.): %.3lf (Ожидаемое: %.3lf, погрешность: %.3lf%%)\n", results->first_task_sum_arrival, expected_arrival_time, delta_arrival);
    printf("Время работы (е.в.): %.3lf (Ожидаемое: %.3lf, погрешность: %.3lf%%)\n", service_time, expected_service_time, delta_modeling);
    printf("Время простоя (е.в.): %.3lf\n", hold_time);
    printf("ВОШЛО 1 типа: %lu\n", results->first_task_in_amount);
    printf("ВЫШЛО 1 типа: %lu\n", results->first_task_out_amount);
    printf("Количество обращений 2 типа: %lu\n", results->second_task_out_amount);

    print_newline();
}

int simulate_list_queue(void)
{
    int rc;
    list_queue_t *list_queue;
    results_t results = {0};

    list_queue = new_list_queue();
    if (list_queue == NULL)
        return ERR_NO_MEMORY;

    double first_task_cooldown = get_first_task_cooldown();
    double current_remaining_time = 0;
    int curr_type = 0;

    srand(time(NULL));

    list_queue_push(list_queue, LOW_PRIORITY_TASK);
    results.second_task_in_amount++;

    while (results.first_task_out_amount < TYPE1_TASK_AMOUNT)
    {
        if (curr_type == HIGH_PRIORITY_TASK)
        {
            results.first_task_out_amount++;
            results.queue_length = get_list_queue_size(list_queue);
            results.queue_sum_length += results.queue_length;
            if (results.first_task_out_amount % TYPE1_TASK_PRINT_STEP == 0)
            {
                print_mid_results(&results);
                print_newline();
            }
        }
        else if (curr_type == LOW_PRIORITY_TASK)
        {
            results.second_task_out_amount++;
            results.queue_sum_length += get_list_queue_size(list_queue);
            list_queue_insert(list_queue, LOW_PRIORITY_TASK, 4 - 1);
            results.second_task_in_amount++;
        }

        rc = list_queue_pop(list_queue, &curr_type);
        if (rc != EXIT_SUCCESS)
            return rc;

        if (curr_type == HIGH_PRIORITY_TASK)
        {
            current_remaining_time = get_first_task_required();
            results.first_task_sum_service += current_remaining_time;
        }
        else if (curr_type == LOW_PRIORITY_TASK)
        {
            current_remaining_time = get_second_task_required();
            results.second_task_sum_service += current_remaining_time;
        }
        else
            abort();

        while (current_remaining_time > EPSILON)
        {
            if (first_task_cooldown < current_remaining_time)
            {
                list_queue_push(list_queue, HIGH_PRIORITY_TASK);
                results.first_task_in_amount++;
                current_remaining_time -= first_task_cooldown;
                first_task_cooldown = get_first_task_cooldown();
                results.first_task_sum_arrival += first_task_cooldown;
            }
            else
            {
                first_task_cooldown -= current_remaining_time;
                current_remaining_time = 0;
            }
        }
    }

    print_total_results(&results);

    free_list_queue(list_queue);

    return EXIT_SUCCESS;
}
