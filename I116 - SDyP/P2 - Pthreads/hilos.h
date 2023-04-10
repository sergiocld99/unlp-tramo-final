#include <pthread.h>
#include <stddef.h>     // NULL

void create_and_join(void *(*start_routine)(void *), int T);