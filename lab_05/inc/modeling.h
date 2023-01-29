#ifndef __MODELING_H__
#define __MODELING_H__

#include <math.h>
#include <time.h>
#include "print.h"
#include "array.h"
#include "list.h"
#include "presets.h"

typedef enum priority
{
    HIGH_PRIORITY_TASK = 1,
    LOW_PRIORITY_TASK = 2
} priority_t;

int
simulate_array_queue(void);

int
simulate_list_queue(void);

typedef struct results
{
    size_t first_task_in_amount;
    size_t first_task_out_amount;
    size_t second_task_in_amount;
    size_t second_task_out_amount;
    size_t queue_length;
    size_t queue_sum_length;
    double first_task_sum_arrival;
    double first_task_sum_service;
    double second_task_sum_service;
    size_t calls;
} results_t;

#define ITERATIONS 100

#define TYPE1_TASK_AMOUNT 1000LU
#define TYPE1_TASK_PRINT_STEP 100

#define TYPE1_TASK_MIN_ARRIVAL_TIME 0.0f
#define TYPE1_TASK_MAX_ARRIVAL_TIME 5.0f
#define TYPE1_TASK_MIN_SERVICE_TIME 0.0f
#define TYPE1_TASK_MAX_SERVICE_TIME 10.0f
#define TYPE2_TASK_MIN_SERVICE_TIME 0.0f
#define TYPE2_TASK_MAX_SERVICE_TIME 4.0f

#define EPSILON 1e-7


#endif  // __MODELING_H__
