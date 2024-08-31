#include <stdio.h>
#include <stdlib.h>

int main() {
    int *ptr = malloc(sizeof(int));
    printf("Allocated memory, now enter a number to store: ");
    scanf("%d", ptr);
    printf("Freeing memory...\n");
    free(ptr); // 第一次释放
    free(ptr); // 第二次释放
    return 0;
}