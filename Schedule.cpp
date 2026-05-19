#include <iostream>
using namespace std;

struct Process {
    int pid, at, bt, ct, tat, wt, pr, rt;
};

/* ---------- SORT BY ARRIVAL ---------- */
void sortByArrival(Process p[], int n)
{
    for(int i=0;i<n-1;i++)
        for(int j=i+1;j<n;j++)
            if(p[i].at > p[j].at)
                swap(p[i], p[j]);
}

/* ---------- CALCULATE TAT & WT ---------- */
void calculate(Process p[], int n)
{
    for(int i=0;i<n;i++)
    {
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt  = p[i].tat - p[i].bt;
    }
}

/* ---------- DISPLAY TABLE ---------- */
void display(Process p[], int n)
{
    float avgTAT=0, avgWT=0;

    cout<<"\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for(int i=0;i<n;i++)
    {
        cout<<p[i].pid<<"\t"<<p[i].at<<"\t"<<p[i].bt<<"\t"
            <<p[i].ct<<"\t"<<p[i].tat<<"\t"<<p[i].wt<<"\n";

        avgTAT+=p[i].tat;
        avgWT+=p[i].wt;
    }

    cout<<"\nAverage TAT = "<<avgTAT/n;
    cout<<"\nAverage WT  = "<<avgWT/n<<endl;
}

/* ---------- PRINT GANTT ---------- */
void printGantt(int gantt[], int size)
{
    cout<<"\n=========== GANTT CHART ===========\n| ";
    for(int i=0;i<size;i++)
    {
        if(gantt[i]==-1)
            cout<<"IDLE | ";
        else
            cout<<"P"<<gantt[i]<<" | ";
    }

    cout<<"\n0";
    for(int i=1;i<=size;i++)
        cout<<"\t"<<i;

    cout<<endl;
}

/* ================= FCFS ================= */
void FCFS(Process p[], int n)
{
    sortByArrival(p,n);

    int gantt[1000], g=0;
    int time=0;

    for(int i=0;i<n;i++)
    {
        while(time < p[i].at)
        {
            gantt[g++] = -1;
            time++;
        }

        for(int k=0;k<p[i].bt;k++)
        {
            gantt[g++] = p[i].pid;
            time++;
        }

        p[i].ct=time;
    }

    calculate(p,n);
    display(p,n);
    printGantt(gantt,g);
}

/* ================= SJF (NON PREEMPTIVE) ================= */
void SJF(Process p[], int n)
{
    sortByArrival(p,n);

    bool done[20]={false};
    int gantt[1000], g=0;
    int completed=0, time=0;

    while(completed<n)
    {
        int idx=-1, minBT=9999;

        for(int i=0;i<n;i++)
            if(!done[i] && p[i].at<=time && p[i].bt<minBT)
            {
                minBT=p[i].bt;
                idx=i;
            }

        if(idx==-1)
        {
            gantt[g++]=-1;
            time++;
            continue;
        }

        for(int k=0;k<p[idx].bt;k++)
        {
            gantt[g++]=p[idx].pid;
            time++;
        }

        p[idx].ct=time;
        done[idx]=true;
        completed++;
    }

    calculate(p,n);
    display(p,n);
    printGantt(gantt,g);
}

/* ================= SRTN (PREEMPTIVE SJF) ================= */
void SRTN(Process p[], int n)
{
    sortByArrival(p,n);

    for(int i=0;i<n;i++)
        p[i].rt=p[i].bt;

    int gantt[1000], g=0;
    int completed=0, time=0;

    while(completed<n)
    {
        int idx=-1, minRT=9999;

        for(int i=0;i<n;i++)
            if(p[i].at<=time && p[i].rt>0 && p[i].rt<minRT)
            {
                minRT=p[i].rt;
                idx=i;
            }

        if(idx==-1)
        {
            gantt[g++]=-1;
            time++;
            continue;
        }

        gantt[g++]=p[idx].pid;
        p[idx].rt--;
        time++;

        if(p[idx].rt==0)
        {
            p[idx].ct=time;
            completed++;
        }
    }

    calculate(p,n);
    display(p,n);
    printGantt(gantt,g);
}

/* ================= PRIORITY NON PREEMPTIVE ================= */
void PriorityNP(Process p[], int n)
{
    for(int i=0;i<n;i++)
    {
        cout<<"Enter Priority for P"<<p[i].pid<<" : ";
        cin>>p[i].pr;
    }

    sortByArrival(p,n);

    bool done[20]={false};
    int gantt[1000], g=0;
    int completed=0, time=0;

    while(completed<n)
    {
        int idx=-1, high=9999;

        for(int i=0;i<n;i++)
            if(!done[i] && p[i].at<=time && p[i].pr<high)
            {
                high=p[i].pr;
                idx=i;
            }

        if(idx==-1)
        {
            gantt[g++]=-1;
            time++;
            continue;
        }

        for(int k=0;k<p[idx].bt;k++)
        {
            gantt[g++]=p[idx].pid;
            time++;
        }

        p[idx].ct=time;
        done[idx]=true;
        completed++;
    }

    calculate(p,n);
    display(p,n);
    printGantt(gantt,g);
}

/* ================= PRIORITY PREEMPTIVE ================= */
void PriorityP(Process p[], int n)
{
    for(int i=0;i<n;i++)
    {
        cout<<"Enter Priority for P"<<p[i].pid<<" : ";
        cin>>p[i].pr;
        p[i].rt=p[i].bt;
    }

    sortByArrival(p,n);

    int gantt[1000], g=0;
    int completed=0, time=0;

    while(completed<n)
    {
        int idx=-1, high=9999;

        for(int i=0;i<n;i++)
            if(p[i].at<=time && p[i].rt>0 && p[i].pr<high)
            {
                high=p[i].pr;
                idx=i;
            }

        if(idx==-1)
        {
            gantt[g++]=-1;
            time++;
            continue;
        }

        gantt[g++]=p[idx].pid;
        p[idx].rt--;
        time++;

        if(p[idx].rt==0)
        {
            p[idx].ct=time;
            completed++;
        }
    }

    calculate(p,n);
    display(p,n);
    printGantt(gantt,g);
}

/* ================= ROUND ROBIN ================= */
void RoundRobin(Process p[], int n)
{
    int tq;
    cout<<"Enter Time Quantum: ";
    cin>>tq;

    sortByArrival(p,n);

    for(int i=0;i<n;i++)
        p[i].rt=p[i].bt;

    int gantt[1000], g=0;
    int completed=0, time=0;

    while(completed<n)
    {
        bool executed=false;

        for(int i=0;i<n;i++)
        {
            if(p[i].at<=time && p[i].rt>0)
            {
                executed=true;

                int exec = (p[i].rt>tq) ? tq : p[i].rt;

                for(int k=0;k<exec;k++)
                {
                    gantt[g++]=p[i].pid;
                    time++;
                }

                p[i].rt-=exec;

                if(p[i].rt==0)
                {
                    p[i].ct=time;
                    completed++;
                }
            }
        }

        if(!executed)
        {
            gantt[g++]=-1;
            time++;
        }
    }

    calculate(p,n);
    display(p,n);
    printGantt(gantt,g);
}

/* ================= MAIN ================= */
int main()
{
    Process p[20];
    int n, choice;

    cout<<"Enter number of processes: ";
    cin>>n;

    for(int i=0;i<n;i++)
    {
        p[i].pid=i+1;
        cout<<"\nProcess "<<p[i].pid<<"\n";
        cout<<"Arrival Time: ";
        cin>>p[i].at;
        cout<<"Burst Time: ";
        cin>>p[i].bt;
    }

    do{
        cout<<"\n1.FCFS\n2.SJF\n3.SRTN\n4.Priority NP\n5.Priority P\n6.Round Robin\n7.Exit\n";
        cout<<"Enter choice: ";
        cin>>choice;

        switch(choice)
        {
            case 1: FCFS(p,n); break;
            case 2: SJF(p,n); break;
            case 3: SRTN(p,n); break;
            case 4: PriorityNP(p,n); break;
            case 5: PriorityP(p,n); break;
            case 6: RoundRobin(p,n); break;
        }

    }while(choice!=7);

    return 0;
}
