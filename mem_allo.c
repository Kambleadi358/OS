#include <stdio.h>

#define MAX 50

// -------- GLOBAL VARIABLES --------
int holes[MAX], processes[MAX];
int allocation[MAX];
int hole_count, process_count;

// -------- FUNCTION DECLARATIONS --------
void firstFit();
void bestFit();
void worstFit();
void display(int temp_holes[]);
void resetAllocation();

// -------- RESET --------
void resetAllocation() {
    for (int i = 0; i < process_count; i++)
        allocation[i] = -1;
}

// -------- DISPLAY --------
void display(int temp_holes[]) {
    printf("\nProcess No.\tProcess Size\tHole Allocated\n");

    for (int i = 0; i < process_count; i++) {
        printf("P%d\t\t%d\t\t", i + 1, processes[i]);

        if (allocation[i] != -1)
            printf("%d\n", allocation[i] + 1);
        else
            printf("Not Allocated\n");
    }

    printf("\nRemaining Hole Sizes:\n");
    for (int i = 0; i < hole_count; i++) {
        printf("Hole %d: %d\n", i + 1, temp_holes[i]);
    }
}

// -------- FIRST FIT --------
void firstFit() {
    int temp_holes[MAX];

    for (int i = 0; i < hole_count; i++)
        temp_holes[i] = holes[i];

    resetAllocation();

    for (int i = 0; i < process_count; i++) {
        for (int j = 0; j < hole_count; j++) {
            if (temp_holes[j] >= processes[i]) {
                allocation[i] = j;
                temp_holes[j] -= processes[i];
                break;
            }
        }
    }

    printf("\n--- First Fit ---\n");
    display(temp_holes);
}

// -------- BEST FIT --------
void bestFit() {
    int temp_holes[MAX];

    for (int i = 0; i < hole_count; i++)
        temp_holes[i] = holes[i];

    resetAllocation();

    for (int i = 0; i < process_count; i++) {
        int bestIndex = -1;

        for (int j = 0; j < hole_count; j++) {
            if (temp_holes[j] >= processes[i]) {
                if (bestIndex == -1 || temp_holes[j] < temp_holes[bestIndex]) {
                    bestIndex = j;
                }
            }
        }

        if (bestIndex != -1) {
            allocation[i] = bestIndex;
            temp_holes[bestIndex] -= processes[i];
        }
    }

    printf("\n--- Best Fit ---\n");
    display(temp_holes);
}

// -------- WORST FIT --------
void worstFit() {
    int temp_holes[MAX];

    for (int i = 0; i < hole_count; i++)
        temp_holes[i] = holes[i];

    resetAllocation();

    for (int i = 0; i < process_count; i++) {
        int worstIndex = -1;

        for (int j = 0; j < hole_count; j++) {
            if (temp_holes[j] >= processes[i]) {
                if (worstIndex == -1 || temp_holes[j] > temp_holes[worstIndex]) {
                    worstIndex = j;
                }
            }
        }

        if (worstIndex != -1) {
            allocation[i] = worstIndex;
            temp_holes[worstIndex] -= processes[i];
        }
    }

    printf("\n--- Worst Fit ---\n");
    display(temp_holes);
}

// -------- MAIN --------
int main() {
    int choice;

    printf("Enter number of memory holes: ");
    scanf("%d", &hole_count);

    printf("Enter sizes of holes:\n");
    for (int i = 0; i < hole_count; i++) {
        printf("Hole %d: ", i + 1);
        scanf("%d", &holes[i]);
    }

    printf("\nEnter number of processes: ");
    scanf("%d", &process_count);

    printf("Enter sizes of processes:\n");
    for (int i = 0; i < process_count; i++) {
        printf("Process %d: ", i + 1);
        scanf("%d", &processes[i]);
    }

    do {
        printf("\n===== MENU =====\n");
        printf("1. First Fit\n");
        printf("2. Best Fit\n");
        printf("3. Worst Fit\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: firstFit(); break;
            case 2: bestFit(); break;
            case 3: worstFit(); break;
            case 4: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }

    } while (choice != 4);

    return 0;
}