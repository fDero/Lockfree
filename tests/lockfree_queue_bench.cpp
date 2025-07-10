
#include <celero/Celero.h>
#include <lockfree/queue.hpp>
#include <mocks/consumer_producer_dummies.hpp>
#include <mocks/mutex_based_reference_queue.hpp>
#include <thread>

#ifndef CELERO_MAIN_DONE
#define CELERO_MAIN_DONE
CELERO_MAIN;
#endif

#define QSIZE_SMALL 100
#define QSIZE_LARGE 10000

#define CONS_LOW 3
#define CONS_NORM 10
#define CONS_MID 20

#define PRODS_LOW 3
#define PRODS_NORM 10
#define PRODS_MID 20

/**********************************************************************************************************
                                               T3_S100
**********************************************************************************************************/

#undef SUITE_NAME
#define SUITE_NAME T3_S100

BASELINE(SUITE_NAME, ReferenceMutexBasedQueue, 0, 1) {
    using queue_t = mutex_based_reference_queue<payload_t, QSIZE_SMALL>;
    multiple_push_multiple_pops_concurrently<queue_t, CONS_LOW, PRODS_LOW>();
}

BENCHMARK(SUITE_NAME, LockfreeQueue, 0, 1) {
    using queue_t = dero::lockfree::queue<payload_t, QSIZE_SMALL>;
    multiple_push_multiple_pops_concurrently<queue_t, CONS_LOW, PRODS_LOW>();
}

/**********************************************************************************************************
                                             T10_S100
**********************************************************************************************************/

#undef SUITE_NAME
#define SUITE_NAME T10_S100

BASELINE(SUITE_NAME, ReferenceMutexBasedQueue, 0, 1) {
    using queue_t = mutex_based_reference_queue<payload_t, QSIZE_SMALL>;
    multiple_push_multiple_pops_concurrently<queue_t, CONS_NORM, PRODS_NORM>();
}

BENCHMARK(SUITE_NAME, LockfreeQueue, 0, 1) {
    using queue_t = dero::lockfree::queue<payload_t, QSIZE_SMALL>;
    multiple_push_multiple_pops_concurrently<queue_t, CONS_NORM, PRODS_NORM>();
}

/**********************************************************************************************************
                                            T20_S100
**********************************************************************************************************/

#undef SUITE_NAME
#define SUITE_NAME T20_S100

BASELINE(SUITE_NAME, ReferenceMutexBasedQueue, 0, 1) {
    using queue_t = mutex_based_reference_queue<payload_t, QSIZE_SMALL>;
    multiple_push_multiple_pops_concurrently<queue_t, CONS_MID, PRODS_MID>();
}

BENCHMARK(SUITE_NAME, LockfreeQueue, 0, 1) {
    using queue_t = dero::lockfree::queue<payload_t, QSIZE_SMALL>;
    multiple_push_multiple_pops_concurrently<queue_t, CONS_MID, PRODS_MID>();
}

/**********************************************************************************************************
                                          T10_S10000
**********************************************************************************************************/

#undef SUITE_NAME
#define SUITE_NAME T10_S10000

BASELINE(SUITE_NAME, ReferenceMutexBasedQueue, 0, 1) {
    using queue_t = mutex_based_reference_queue<payload_t, QSIZE_LARGE>;
    multiple_push_multiple_pops_concurrently<queue_t, CONS_NORM, PRODS_NORM>();
}

BENCHMARK(SUITE_NAME, LockfreeQueue, 0, 1) {
    using queue_t = dero::lockfree::queue<payload_t, QSIZE_LARGE>;
    multiple_push_multiple_pops_concurrently<queue_t, CONS_NORM, PRODS_NORM>();
}