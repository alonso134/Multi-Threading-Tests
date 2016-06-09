//
//  main.cpp
//  Hw03
//
//  Created by Kaichen Xiao on 6/8/16.
//  Copyright © 2016 Kaichen Xiao. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <array>
#include <random>
#include <string>
#include <unistd.h>
#include <pthread.h>

using namespace std;

struct storageArray {
    long start = 0;
    long end = 0;
    long* arr;
    long size;
    long sum = 0;
    int nThreads;
};

void* Array_Summation(void* data) {
    long Sum=0;
    storageArray *dataArray = (struct storageArray*) data;
    for (long i = dataArray->start; i <= dataArray->end; i++) {
        Sum += dataArray->arr[i];
    }
    
    return (void*) Sum;
}


int main(int argc, const char * argv[]) {
    if (argc <= 1) {
        printf("Shit, your argument should contain two numbers.\n");
    }
    else {
        long size = atol(argv[1]);
        int nThreads = atoi(argv[2]);
        long totalSum =0;
        pthread_t act_Threads[nThreads];
        long *randomArray;
        randomArray = (long*)malloc(size*sizeof(long));
        //dataArray.arr = new long[dataArray.size];
        
        for (long i = 0; i < size; i++) {
            long randN = random() % 1000;
            randomArray[i] = randN;
            //cout << randN << ' ';
        }
        clock_t startClock = clock();
        long chunk = size/nThreads; //256
        
        for (int i = 0; i < nThreads; i++) {
            storageArray *dataArray;
            dataArray = (storageArray*)malloc(sizeof(storageArray));
            dataArray->end = chunk*(i+1)-1; //0:256 1:512 768 1024
            dataArray->start = chunk*i; //0:0 1:256 2:
            dataArray->arr = randomArray;
            if (pthread_create(&act_Threads[i], NULL, Array_Summation, (void*)dataArray)) {
                printf( "*** Error creating thread ***\n" );
                exit(-1);
            }
            //cout << dataArray->start << ':' << dataArray->end << endl;
            //cout << "Sum: " << dataArray->sum << endl;
        }
        
        for (int i = 0; i < nThreads; i++) {
            long partialSum;
            if (pthread_join(act_Threads[i], (void**) &partialSum)) {
                printf("*** Error creating thread ***\n");
                exit(-1);
            }
            totalSum += partialSum;
        }
        
        cout << "Sum: " << totalSum << endl;
        delete[] randomArray;
        randomArray = nullptr;
        clock_t endClock = clock();
        double elapsedTime = (double)(endClock - startClock)/CLOCKS_PER_SEC;
        cout << "Time Elapsed: " << elapsedTime << endl;
    }
    return 0;
}
