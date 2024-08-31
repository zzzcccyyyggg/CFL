#include <stdio.h>

int main() {
    int *ptr = NULL;
    int index;
    printf("Enter an index to dereference: ");
    scanf("%d", &index);
    printf("Attempting to dereference a NULL pointer at index %d...\n", index);
    // 空指针解引用
    printf("Value: %d\n", ptr[index]);
    return 0;
}