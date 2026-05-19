#include <stdio.h>
#include <stdbool.h>

#define N 3   // processes
#define M 3   // resources

void calculateNeed(int need[N][M], int max[N][M], int alloc[N][M])
{
    for(int i=0;i<N;i++)
        for(int j=0;j<M;j++)
            need[i][j] = max[i][j] - alloc[i][j];
}

bool isSafe(int alloc[N][M], int need[N][M], int avail[M])
{
    int work[M];
    bool finish[N] = {false};
    int safeSeq[N];

    for(int i=0;i<M;i++)
        work[i] = avail[i];

    int count = 0;

    while(count < N)
    {
        bool found = false;

        for(int i=0;i<N;i++)
        {
            if(!finish[i])
            {
                bool possible = true;

                for(int j=0;j<M;j++)
                {
                    if(need[i][j] > work[j])
                    {
                        possible = false;
                        break;
                    }
                }

                if(possible)
                {
                    for(int j=0;j<M;j++)
                        work[j] += alloc[i][j];

                    safeSeq[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if(!found)
            break;
    }

    if(count == N)
    {
        printf("\nSAFE STATE\nSequence: ");
        for(int i=0;i<N;i++)
            printf("P%d ", safeSeq[i]+1);
        printf("\n");
        return true;
    }
    else
    {
        printf("\nUNSAFE STATE\n");
        return false;
    }
}

int main()
{
    int max[N][M], alloc[N][M], need[N][M];
    int avail[M], total[M];

    printf("Enter Max Matrix:\n");
    for(int i=0;i<N;i++)
        for(int j=0;j<M;j++)
            scanf("%d",&max[i][j]);

    printf("Enter Allocation Matrix:\n");
    for(int i=0;i<N;i++)
        for(int j=0;j<M;j++)
            scanf("%d",&alloc[i][j]);

    printf("Enter Available Vector:\n");
    for(int j=0;j<M;j++)
        scanf("%d",&avail[j]);

    // Need calculation
    calculateNeed(need, max, alloc);

    printf("\nNeed Matrix:\n");
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<M;j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }

    // Total resources
    for(int j=0;j<M;j++)
    {
        int sum = avail[j];
        for(int i=0;i<N;i++)
            sum += alloc[i][j];
        total[j] = sum;
    }

    printf("\nTotal Resources:\n");
    for(int j=0;j<M;j++)
        printf("%d ", total[j]);
    printf("\n");

    // Initial Safety
    isSafe(alloc, need, avail);

    // ---------------- REQUEST ----------------
    char choice;
    printf("\nDo you want to request additional resources? (y/n): ");
    scanf(" %c",&choice);

    if(choice=='y' || choice=='Y')
    {
        int p;
        int request[M];

        printf("Enter process number (1-3): ");
        scanf("%d",&p);
        p--; // index

        printf("Enter request vector:\n");
        for(int j=0;j<M;j++)
            scanf("%d",&request[j]);

        // Step 1: Request <= Need
        for(int j=0;j<M;j++)
        {
            if(request[j] > need[p][j])
            {
                printf("Error: Request exceeds Need\n");
                return 0;
            }
        }

        // Step 2: Request <= Available
        for(int j=0;j<M;j++)
        {
            if(request[j] > avail[j])
            {
                printf("Process must WAIT (resources not available)\n");
                return 0;
            }
        }

        // Step 3: Pretend allocation
        for(int j=0;j<M;j++)
        {
            avail[j] -= request[j];
            alloc[p][j] += request[j];
            need[p][j] -= request[j];
        }

        printf("\nAfter Request:\n");

        if(isSafe(alloc, need, avail))
        {
            printf("Request GRANTED\n");
        }
        else
        {
            printf("Request DENIED (unsafe)\n");

            // rollback
            for(int j=0;j<M;j++)
            {
                avail[j] += request[j];
                alloc[p][j] -= request[j];
                need[p][j] += request[j];
            }
        }
    }

    return 0;
}
