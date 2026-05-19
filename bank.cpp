#include <iostream>
using namespace std;

int main()
{
    int n = 3, m = 3;

    int max[3][3], alloc[3][3], need[3][3];
    int avail[3], total[3];

    cout << "Enter Max Matrix (3x3):\n";
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            cin >> max[i][j];

    cout << "Enter Allocation Matrix (3x3):\n";
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            cin >> alloc[i][j];

    cout << "Enter Available Vector (3 values):\n";
    for(int j=0;j<m;j++)
        cin >> avail[j];

    // -------- NEED MATRIX --------
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            need[i][j] = max[i][j] - alloc[i][j];

    cout << "\nNeed Matrix:\n";
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
            cout << need[i][j] << " ";
        cout << endl;
    }

    // -------- TOTAL RESOURCES --------
    for(int j=0;j<m;j++)
    {
        int sum = avail[j];
        for(int i=0;i<n;i++)
            sum += alloc[i][j];
        total[j] = sum;
    }

    cout << "\nTotal Resources:\n";
    for(int j=0;j<m;j++)
        cout << total[j] << " ";
    cout << endl;

    // -------- BANKER'S ALGORITHM --------
    bool finish[3] = {false};
    int safeSeq[3];
    int work[3];

    // copy available → work
    for(int j=0;j<m;j++)
        work[j] = avail[j];

    int count = 0;

    while(count < n)
    {
        bool found = false;

        for(int i=0;i<n;i++)
        {
            if(!finish[i])
            {
                bool possible = true;

                for(int j=0;j<m;j++)
                {
                    if(need[i][j] > work[j])
                    {
                        possible = false;
                        break;
                    }
                }

                if(possible)
                {
                    for(int j=0;j<m;j++)
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

    // -------- RESULT --------
    if(count == n)
    {
        cout << "\nSystem is in SAFE state\n";
        cout << "Safe Sequence: ";

        for(int i=0;i<n;i++)
            cout << "P" << safeSeq[i]+1 << " --> ";

        cout << endl;
    }
    else
    {
        cout << "\nSystem is in UNSAFE state\n";
    }

    return 0;
}