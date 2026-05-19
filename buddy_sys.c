#include <stdio.h>
#include <math.h>

#define TOTAL_MEMORY 512
#define MAX_BLOCKS 100

typedef struct {
    int size;
    int free;
    int pid;
} Block;

Block memory[MAX_BLOCKS];
int blockCount = 1;

// -------- Initialize --------
void initMemory() {
    memory[0].size = TOTAL_MEMORY;
    memory[0].free = 1;
    memory[0].pid = -1;
}

// -------- Find next power of 2 --------
int nextPowerOf2(int size) {
    int power = 1;
    while (power < size)
        power *= 2;
    return power;
}

// -------- Display Memory --------
void displayMemory() {
    printf("\n===== MEMORY STATE =====\n");
    printf("Block\tSize\tStatus\n");

    for (int i = 0; i < blockCount; i++) {
        printf("%d\t%d\t", i, memory[i].size);

        if (memory[i].free)
            printf("Free\n");
        else
            printf("Allocated (P%d)\n", memory[i].pid);
    }
}

// -------- Visual Representation --------
void visualize() {
    printf("\n===== VISUAL MEMORY =====\n");

    for (int i = 0; i < blockCount; i++) {
        printf("[");
        if (memory[i].free)
            printf("Free:%d", memory[i].size);
        else
            printf("P%d:%d", memory[i].pid, memory[i].size);
        printf("]");
    }
    printf("\n");
}

// -------- Allocation --------
void allocate(int pid, int size) {
    int required = nextPowerOf2(size);

    for (int i = 0; i < blockCount; i++) {
        if (memory[i].free && memory[i].size >= required) {

            // Split until exact size
            while (memory[i].size > required) {
                // Shift right
                for (int j = blockCount; j > i; j--)
                    memory[j] = memory[j - 1];

                // Split block
                memory[i].size /= 2;
                memory[i + 1].size = memory[i].size;

                memory[i].free = 1;
                memory[i + 1].free = 1;

                memory[i].pid = -1;
                memory[i + 1].pid = -1;

                blockCount++;
            }

            // Allocate
            memory[i].free = 0;
            memory[i].pid = pid;

            printf("\nProcess P%d allocated %d MB\n", pid, required);
            return;
        }
    }

    printf("\nAllocation failed for P%d (Not enough memory)\n", pid);
}

// -------- Merge Buddies --------
void merge() {
    for (int i = 0; i < blockCount - 1; i++) {
        if (memory[i].free && memory[i + 1].free &&
            memory[i].size == memory[i + 1].size) {

            memory[i].size *= 2;

            // Shift left
            for (int j = i + 1; j < blockCount - 1; j++)
                memory[j] = memory[j + 1];

            blockCount--;
            i--; // recheck
        }
    }
}

// -------- Deallocate --------
void deallocate(int pid) {
    for (int i = 0; i < blockCount; i++) {
        if (!memory[i].free && memory[i].pid == pid) {
            memory[i].free = 1;
            memory[i].pid = -1;

            printf("\nProcess P%d deallocated\n", pid);
            merge();
            return;
        }
    }

    printf("\nProcess not found!\n");
}

// -------- MAIN --------
int main() {
    int choice, size, pid = 1, del;

    initMemory();

    do {
        printf("\n===== BUDDY SYSTEM =====\n");
        printf("1. Allocate Process\n");
        printf("2. Deallocate Process\n");
        printf("3. Display Memory\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {

        case 1:
            printf("Enter process size (MB): ");
            scanf("%d", &size);
            allocate(pid++, size);
            visualize();
            break;

        case 2:
            printf("Enter process ID to remove: ");
            scanf("%d", &del);
            deallocate(del);
            visualize();
            break;

        case 3:
            displayMemory();
            visualize();
            break;

        case 4:
            printf("Exiting...\n");
            break;

        default:
            printf("Invalid choice!\n");
        }

    } while (choice != 4);

    return 0;
}