#include<stdio.h>
#include<stdlib.h>
//incompatible implicit declaration of built-in function 'exit
#define WINXX 0

int getTimeOfDay(void)
{
    // #if defined(WINXX)
    // printf("defined(WINXX) && WINXX\n");
    // printf("WINXX: %d \n", WINXX);
    // #endif
    #if defined (__WIN32__)
        printf("__WIN32__: %d \n", __WIN32__);
    #endif

    #if defined(_WIN64) && _WIN64
    printf("defined(_WIN64) && _WIN64\n");
    printf("_WIN64: %d \n", _WIN64);
    #endif
    printf("%s:%d:%s: doesn't work yet, exiting...\n", __FILE__, __LINE__, __func__);
    printf("__FILE__: %s \n", __FILE__);
    printf("__LINE__: %d \n", __LINE__);
    printf("__func__: %s \n", __func__);
    printf("__func__: %d \n", __func__);
    exit(1);
    return 0;  // to keep compiler happy. 
}

int main(){

    getTimeOfDay();
    return 0;
}