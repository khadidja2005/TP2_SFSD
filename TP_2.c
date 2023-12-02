#include <stdio.h>


<<<<<<< HEAD

int main(){
 int B;  
printf("enter :");
scanf("%d", &B);
=======
int main() {

int isInArray(int value, int array[], size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (array[i] == value) {
            return 1; // Value found
        }
    }
    return 0; // Value not found
}

int main() {
    int numbers[] = {1, 2, 3, 4, 5};
    size_t size = sizeof(numbers) / sizeof(numbers[0]);

    int searchValue = 3;

    if (isInArray(searchValue, numbers, size)) {
        printf("%d is in the array.\n", searchValue);
    } else {
        printf("%d is not in the array.\n", searchValue);
    }

    return 0;
}
>>>>>>> c39ef5e961753a497f79868b91e8c6f55bd69edc

}