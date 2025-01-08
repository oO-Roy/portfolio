#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "multi_threaded_memory_mapping.h"

#define RED "\033[0;31m"
#define BOLD_RED "\033[1;31m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[1;32m"
#define RESET "\033[0m"

#define NUM_OF_THREADS 3

static pthread_key_t key;

void CloseApp(void* data)
{
    (void)(data);
    pthread_getspecific(key);
    printf(BOLD_RED"Destructor!\n"RESET);
}

void* ThreadWork(void* data)
{
    int local_data = 0;
    size_t* local_mall = malloc(sizeof(size_t));

    printf("data: %p", &local_data);
    printf("malloc_data: %p", local_mall);
    
    printf("num: %ld ", (size_t*)data);
    printf(BOLD_GREEN"Thread created!\n"RESET);
    pthread_setspecific(key, data);

    free(local_mall);

    return NULL;
}

int main()
{
	int i = 0;
	int status = 0;

	pthread_t thread_id[NUM_OF_THREADS] = {0};
    pthread_attr_t attr = {0};

    size_t num = 0;

    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 0x8000); 

    pthread_key_create(&key, CloseApp);

	for (i = 0; i < NUM_OF_THREADS; ++i)
    {
        status = pthread_create(&thread_id[i], &attr, &ThreadWork, (void*)num);

        while (0 != status)
        {
            status = pthread_create(&thread_id[i], &attr, &ThreadWork
                                                                , (void*)num);
        }
    }

    for(i = 0; i < NUM_OF_THREADS; ++i)
    {
        while(0 != pthread_join(thread_id[i], NULL));
    }

    pthread_attr_destroy(&attr);
	
    return 0;
}
