#include <stdio.h>
#include <stdbool.h>

#define N 3   // Processes
#define M 3   // Resources

int main()
{
    int alloc[N][M], request[N][M], avail[M];
    int work[M];
    bool finish[N];

    printf("Enter Allocation Matrix:\n");
    for(int i=0;i<N;i++)
        for(int j=0;j<M;j++)
            scanf("%d",&alloc[i][j]);

    printf("Enter Request Matrix:\n");
    for(int i=0;i<N;i++)
        for(int j=0;j<M;j++)
            scanf("%d",&request[i][j]);

    printf("Enter Available Resources:\n");
    for(int j=0;j<M;j++)
        scanf("%d",&avail[j]);

    // Step 1: Initialize
    for(int j=0;j<M;j++)
        work[j] = avail[j];

    for(int i=0;i<N;i++)
    {
        bool zero = true;
        for(int j=0;j<M;j++)
        {
            if(alloc[i][j] != 0)
            {
                zero = false;
                break;
            }
        }

        if(zero)
            finish[i] = true;
        else
            finish[i] = false;
    }

    // Step 2 & 3
    while(1)
    {
        bool found = false;

        for(int i=0;i<N;i++)
        {
            if(!finish[i])
            {
                bool possible = true;

                for(int j=0;j<M;j++)
                {
                    if(request[i][j] > work[j])
                    {
                        possible = false;
                        break;
                    }
                }

                if(possible)
                {
                    for(int j=0;j<M;j++)
                        work[j] += alloc[i][j];

                    finish[i] = true;
                    found = true;

                    printf("Process P%d executed\n", i+1);
                }
            }
        }

        if(!found)
            break;
    }

    // Step 4: Check deadlock
    bool deadlock = false;

    for(int i=0;i<N;i++)
    {
        if(!finish[i])
        {
            deadlock = true;
            printf("Process P%d is deadlocked\n", i+1);
        }
    }

    if(deadlock)
        printf("\nSYSTEM IS IN DEADLOCK\n");
    else
        printf("\nNO DEADLOCK (SAFE)\n");

    return 0;
}
