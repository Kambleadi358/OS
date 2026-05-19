#include <stdio.h>

#define FRAMES 3
#define MAX 20

// Display frames
void display(int frame[])
{
    for(int i = 0; i < FRAMES; i++)
    {
        if(frame[i] == -1)
            printf("- ");
        else
            printf("%d ", frame[i]);
    }
}

// ---------------- FIFO ----------------
void fifo(int pages[], int n)
{
    int frame[FRAMES];
    int index = 0, faults = 0;

    for(int i = 0; i < FRAMES; i++)
        frame[i] = -1;

    printf("\nFIFO Execution:\n");

    for(int i = 0; i < n; i++)
    {
        int found = 0;

        for(int j = 0; j < FRAMES; j++)
        {
            if(frame[j] == pages[i])
            {
                found = 1;
                break;
            }
        }

        if(!found)
        {
            frame[index] = pages[i];
            index = (index + 1) % FRAMES;
            faults++;
        }

        printf("Page %d -> ", pages[i]);
        display(frame);
        printf(found ? " (Hit)\n" : " (Fault)\n");
    }

    printf("Total Page Faults = %d\n", faults);
}

// ---------------- LRU ----------------
void lru(int pages[], int n)
{
    int frame[FRAMES], time[FRAMES];
    int faults = 0, t = 0;

    for(int i = 0; i < FRAMES; i++)
    {
        frame[i] = -1;
        time[i] = 0;
    }

    printf("\nLRU Execution:\n");

    for(int i = 0; i < n; i++)
    {
        int found = 0;

        for(int j = 0; j < FRAMES; j++)
        {
            if(frame[j] == pages[i])
            {
                found = 1;
                time[j] = ++t;
                break;
            }
        }

        if(!found)
        {
            int lru_index = 0;
            for(int j = 1; j < FRAMES; j++)
            {
                if(time[j] < time[lru_index])
                    lru_index = j;
            }

            frame[lru_index] = pages[i];
            time[lru_index] = ++t;
            faults++;
        }

        printf("Page %d -> ", pages[i]);
        display(frame);
        printf(found ? " (Hit)\n" : " (Fault)\n");
    }

    printf("Total Page Faults = %d\n", faults);
}

// ---------------- OPTIMAL ----------------
void optimal(int pages[], int n)
{
    int frame[FRAMES];
    int faults = 0;

    for(int i = 0; i < FRAMES; i++)
        frame[i] = -1;

    printf("\nOptimal Execution:\n");

    for(int i = 0; i < n; i++)
    {
        int found = 0;

        for(int j = 0; j < FRAMES; j++)
        {
            if(frame[j] == pages[i])
            {
                found = 1;
                break;
            }
        }

        if(!found)
        {
            int pos = -1, farthest = i;

            for(int j = 0; j < FRAMES; j++)
            {
                int k;
                for(k = i + 1; k < n; k++)
                {
                    if(frame[j] == pages[k])
                        break;
                }

                if(k == n)
                {
                    pos = j;
                    break;
                }

                if(k > farthest)
                {
                    farthest = k;
                    pos = j;
                }
            }

            if(pos == -1) pos = 0;

            frame[pos] = pages[i];
            faults++;
        }

        printf("Page %d -> ", pages[i]);
        display(frame);
        printf(found ? " (Hit)\n" : " (Fault)\n");
    }

    printf("Total Page Faults = %d\n", faults);
}

// ---------------- MAIN MENU ----------------
int main()
{
    int pages[MAX], n, choice;

    printf("Enter number of pages: ");
    scanf("%d", &n);

    printf("Enter page reference string:\n");
    for(int i = 0; i < n; i++)
        scanf("%d", &pages[i]);

    do
    {
        printf("\n--- MENU ---\n");
        printf("1. FIFO\n");
        printf("2. LRU\n");
        printf("3. Optimal\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1: fifo(pages, n); break;
            case 2: lru(pages, n); break;
            case 3: optimal(pages, n); break;
            case 4: printf("Exit\n"); break;
            default: printf("Invalid choice\n");
        }

    } while(choice != 4);

    return 0;
}
