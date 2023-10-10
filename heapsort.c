#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct NameFrequency {
    char *name;
    int frequency;
};

void heapify(struct NameFrequency arr[], int n, int i, int isMaxHeap) {
    int largestOrSmallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (isMaxHeap) {
        if (left < n && arr[left].frequency > arr[largestOrSmallest].frequency)
            largestOrSmallest = left;

        if (right < n && arr[right].frequency > arr[largestOrSmallest].frequency)
            largestOrSmallest = right;
    } else {
        if (left < n && arr[left].frequency < arr[largestOrSmallest].frequency)
            largestOrSmallest = left;

        if (right < n && arr[right].frequency < arr[largestOrSmallest].frequency)
            largestOrSmallest = right;
    }

    if (largestOrSmallest != i) {
        struct NameFrequency temp = arr[i];
        arr[i] = arr[largestOrSmallest];
        arr[largestOrSmallest] = temp;

        heapify(arr, n, largestOrSmallest, isMaxHeap);
    }
}

void heapSort(struct NameFrequency arr[], int n, int isMaxHeap) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, isMaxHeap);

    for (int i = n - 1; i > 0; i--) {
        struct NameFrequency temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(arr, i, 0, isMaxHeap);
    }
}

int main() {
    clock_t start_time, end_time;
    double cpu_time_used;

    start_time = clock();

    char inputFileName[100];
    printf("Enter the input file name: ");
    scanf("%s", inputFileName);

    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    int isMaxHeap;
    printf("Do you want to perform a max heap sort (1) or min heap sort (0)? ");
    scanf("%d", &isMaxHeap);

    int capacity = 100;
    int size = 0;
    struct NameFrequency *names = malloc(capacity * sizeof(struct NameFrequency));

    if (names == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    char line[100];
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        line[strcspn(line, "\n")] = '\0';

        int found = 0;
        for (int i = 0; i < size; i++) {
            if (strcmp(names[i].name, line) == 0) {
                names[i].frequency++;
                found = 1;
                break;
            }
        }

        if (!found) {
            if (size == capacity) {
                capacity *= 2;
                names = realloc(names, capacity * sizeof(struct NameFrequency));
                if (names == NULL) {
                    perror("Memory reallocation failed");
                    return 1;
                }
            }

            names[size].name = strdup(line);
            names[size].frequency = 1;
            size++;
        }
    }

    fclose(inputFile);

    heapSort(names, size, isMaxHeap);

    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
        return 1;
    }

    for (int i = 0; i < size; i++) {
        fprintf(outputFile, "%s,%d\n", names[i].name, names[i].frequency);
    }

    fclose(outputFile);

    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Sorting completed in %.2f seconds.\n", cpu_time_used);

    for (int i = 0; i < size; i++) {
        free(names[i].name);
    }

    free(names);

    return 0;
}
