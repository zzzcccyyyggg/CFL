#include <stdio.h>

int main() {
    int array[5];
    int i;
    printf("Enter an index to modify (try out-of-bounds to trigger error): ");
    scanf("%d", &i);
    printf("Enter value to store at index %d: ", i);
    scanf("%d", &array[i]); // 数组越界访问
    printf("Stored %d at index %d\n", array[i], i);
    return 0;
}