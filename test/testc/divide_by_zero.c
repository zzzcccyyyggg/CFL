#include <stdio.h>

int main() {
    int divisor;
    printf("Enter a divisor (attempt division by zero to trigger error): ");
    scanf("%d", &divisor);
    int result = 10 / divisor; // 除零错误
    printf("Result of 10 divided by %d is %d\n", divisor, result);
    return 0;
}