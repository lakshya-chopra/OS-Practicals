#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

struct P
{
    int pid;
    int aTime, bTime, sTime[20], wTime, fTime, taTime;
};

int quant;
int main()
{
    int n, i, j;
    printf("Enter the no. of processes: ");
    scanf("%d", &n);
    struct P procs[n];

    printf("Enter time quantum: ");
    scanf("%d", &quant);
    // printf("%d\n",n);
    for (int i = 0; i < n; i++)
    {
        printf("Enter arrival time, and burst time for the process %d: ", i + 1);
        scanf("%d %d", &procs[i].aTime, &procs[i].bTime);
        procs[i].pid = i + 1;
    }

    int c = n, s[n][20];
    float time = 0, mini = INT_MAX, b[n], a[n];

    int index = -1;
    for (i = 0; i < n; i++)
    {
        b[i] = procs[i].bTime;
        a[i] = procs[i].aTime;
        for (j = 0; j < 20; j++)
        {
            s[i][j] = -1;
        }
    }

    int tot_wt, tot_tat;
    tot_wt = 0;
    tot_tat = 0;
    bool flag = false;

    while (c != 0)
    {
        mini = INT_MAX;
        flag = false;

        for (i = 0; i < n; i++)
        {
            if (a[i] <= time && (mini > a[i] || (mini == a[i] && b[i] < b[index])) && b[i] > 0)
            {
                index = i;
                mini = a[i];
                flag = true;
            }
        }

        if (!flag)
        {
            if (time == 0)
            {
                time = a[0];
            }
            else
            {
                time++;
            }
            continue;
        }

        j = 0;
        while (s[index][j] != -1)
            j++;
        if (s[index][j] == -1)
        {
            s[index][j] = time;
            procs[index].sTime[j] = time;
        }

        if (b[index] <= quant)
        {
            time += b[index];
            b[index] = 0;
        }
        else
        {
            time += quant;
            b[index] -= quant;
        }

        if (b[index] > 0)
        {
            a[index] = time + 0.1;
        }

        if (b[index] == 0)
        {
            c--;
            procs[index].fTime = time;
            procs[index].wTime = procs[index].fTime - procs[index].aTime - procs[index].bTime;
            tot_wt += procs[index].wTime;
            procs[index].taTime = procs[index].bTime + procs[index].wTime;
            tot_tat += procs[index].taTime;
        }
    }
    printf("\n%-15s%-15s%-15s%-40s%-15s%-15s%-15s\n",
           "Process No.", "Arrival Time", "Burst Time", "Start Times", "Final Time", "Wait Time", "Turnaround Time");

    for (i = 0; i < n; i++)
    {
        printf("%-15d%-15d%-15d", procs[i].pid, procs[i].aTime, procs[i].bTime);

        j = 0;
        while (s[i][j] != -1)
        {
            printf("%d ", procs[i].sTime[j]);
            j++;
        }

        for (int k = j; k < 13; k++)
        {
            printf(" ");
        }

        printf("%30d%15d%15d\n", procs[i].fTime, procs[i].wTime, procs[i].taTime);
    }

    double avg_wt, avg_tat;
    avg_wt = tot_wt / (float)n;
    avg_tat = tot_tat / (float)n;

    printf("Average wait time is : %lf\n", avg_wt);
    printf("Average turnaround time is : %lf\n", avg_tat);

    return 0;
}