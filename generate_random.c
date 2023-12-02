#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int getRandomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}

int main() {
    // Seed the random number generator with the current time
    srand(time(NULL));

    int minValue ;
    int maxValue;

    scanf("%d",&minValue);
    scanf("%d",&maxValue);

    // Generate a random number between minValue and maxValue (inclusive)
    int randomNum = getRandomNumber(minValue, maxValue);

    printf("Random number between %d and %d: %d\n", minValue, maxValue, randomNum);

    return 0;
}
