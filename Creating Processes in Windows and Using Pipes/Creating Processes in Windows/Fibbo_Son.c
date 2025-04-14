#define _CRT_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void fibonacci(int n) {
    int a = 0, b = 1, temp;
    for (int i = 0; i < n; i++) {
        printf("%d ", a);
        temp = a + b;
        a = b;
        b = temp;
    }
}

int main(int argc, char *argv[]) {
    printf("I am the son!");
    printf("\n");

    if (argc < 2) {  // Ensure argument is passed
        printf("No number provided.\n");
        return 1;
    }
    
    int n = atoi(argv[1]);

    if (n < 1) {
        printf("The function only accepts values greater than 0\n");
        return 1;
    }

    fibonacci(n);
    getchar();
    return 0;
}
