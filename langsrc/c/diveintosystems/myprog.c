/*
 * Copyright (c) 2020, Dive into Systems, LLC (https://diveintosystems.org/)
 * An example program that uses the mylib.h library
 */
#include <stdio.h>
#include "mylib.h"   // include library header file

int main(void) {
    float val1, val2, ret;
    printf("Enter two float values: ");
    scanf("%f%f", &val1, &val2);
    printf("%f and %f\n", val1, val2);
    ret = bigger(val1, val2);   // use a library function
    printf("%f is the biggest\n", ret);

    return 0;
}