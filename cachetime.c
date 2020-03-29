// Program to illustrate the getopt() 
// function in C 
  
#include <stdio.h>  
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>  
#include <time.h>
#include <math.h>

long int reps = 10000000;
char* data;
long int arraySize = 0;


#define MAX_THREAD 16


void *read_latency_measure(void* vargp)
{
    float *time = (float *)vargp;
    clock_t start, end;
    start = clock(); 
    long int sum = 0;

    for (long int i = 0; i < reps; i++)
    {
        for(long int j = arraySize - 1; j >= 0; j--)
        {
            char* ptr = &data[j];
            sum += *ptr%2;
        }
    }

    end = clock(); 
    double timeTaken = (double)(end - start) / (double)CLOCKS_PER_SEC;
    *time = timeTaken/(reps * arraySize) * pow(10, 9);
}

void *read_write_latency_measure(void* vargp)
{
    float *time = (float *)vargp;
    clock_t start, end;
    start = clock(); 

    for (long int i = 0; i < reps; i++)
    {
        for(long int j = arraySize - 1; j >= 0; j--)
        {
            char* ptr = &data[j];
            char entry = *ptr;
            entry = entry & 0xFC;
            *ptr = entry;
        }
    }

    end = clock(); 
    double timeTaken = (double)(end - start) / (double)CLOCKS_PER_SEC;
    *time = timeTaken/(reps * arraySize) * pow(10, 9);
}
  
int main(int argc, char *argv[])  
{ 
    int opt  ; 
    int random = 1;
    int modify = 0;
    int numThread = 1;

    while((opt = getopt(argc, argv, "srmt:")) != -1)  
    {  
        switch(opt)  
        {  
            case 's':
                random = 0;
                //printf("Sequential Access.\n");
                break;  
            case 'r': 
                random = 1;
                //printf("Random Access.\n");
                break; 
            case 'm':
                modify = 1; 
                //printf("Read and write.\n");
                break;  
            case 't':  
                numThread = atoi(optarg);  
                //printf("Number of threads: %d\n", numThread);
                break;  
            case ':':  
                //printf("option needs a value\n");  
                break;  
            case '?':  
                //printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  
    
    reps = atol(argv[optind++]);
    arraySize = (long int)pow(2, atoi(argv[optind]));
    data = (char*) malloc(arraySize*sizeof(char));
    //printf("Repetition: %ld\n", reps); 
    //printf("Array Size: %d\n", arraySize);  

    if(numThread > MAX_THREAD)
        numThread = MAX_THREAD;

    for(long int i = 0; i < arraySize; i++)
        data[i] = (char) rand() % 256;

    pthread_t threads[MAX_THREAD];
    float returnTime[MAX_THREAD];
    
    for (int id = 0; id < numThread; id ++){
        //printf("Creating thread %d.\n", id);
        
        if(modify)
            pthread_create(&threads[id], NULL, read_write_latency_measure, (void *)&returnTime[id]); 
        else
            pthread_create(&threads[id], NULL, read_latency_measure, (void *)&returnTime[id]);
        
    }
    float averageTime = 0;

    for (int id = 0; id < numThread; id ++){
        pthread_join(threads[id], NULL);
        averageTime += returnTime[id];
        //printf("Thread %d returned.\n", id);
    }

    averageTime /= numThread;

    if(arraySize < 1000)
        printf("size: %ld    latency: %.4f ns\n", arraySize, averageTime);
    else
        printf("size: %ld K  latency: %.4f ns\n", (long int)arraySize/1024, averageTime);


    return 0; 
} 


