#include <stdio.h>

int adder() {
    int a;
    return a + 2;
}

int assign() {
    int y = 90;
    return y;
}

int main(void) {
    int x;
    assign();
    x = adder();
    printf("x is: %d\n", x);
    return 0;
}