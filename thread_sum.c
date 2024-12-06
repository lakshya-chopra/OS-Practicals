#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sum = 0;

void *sum_func(void *param);

int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_attr_t attr;

    if (argc != 2)                                 
    {
        fprintf(stderr, "usage: %s <integer value>\n", argv[0]);
        exit(1);
    }
    else if (atoi(argv[1]) < 0)                  
    {
        fprintf(stderr, "integer value must be > 0\n");
        exit(1);
    }
    pthread_attr_init(&attr);                       // initializing attributes of the thread
    pthread_create(&tid, &attr, sum_func, argv[1]);   // creating thread
    pthread_join(tid,NULL);                        
    printf("Sum of first %d numbers is: %d\n",atoi(argv[1]),sum);                        
    return 0;
}

void *sum_func(void *param)
{
    int n = atoi((char*)param);
    for(int i=0;i<=n;i++)
        sum+=i;
    pthread_exit(0);
}