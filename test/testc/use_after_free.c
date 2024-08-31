#include <stdio.h>
#include <stdlib.h>

int main() {
    int *ptr = malloc(sizeof(int));
    printf("Enter a number: ");
    scanf("%d", ptr);
    free(ptr); // 释放内存
    printf("Trying to use memory after it has been freed...\n");
    printf("Value: %d\n", *ptr); // 释放后使用
    return 0;
}