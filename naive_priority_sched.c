#include <stdio.h>
#include <stdlib.h>

typedef struct proc
{

    int pid;
    int pty;  // priority
    float bt; // burst time
    float wt;
    float tat;
} p;

void swap(p *a, p *b)
{
    p temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{

    int num_processes;
    printf("Enter the number of processes:\n");
    scanf("%d", &num_processes);

    p *procs = (p *)malloc(sizeof(p) * num_processes); // not using VLA

    for (int i = 0; i < num_processes; i++)
    {
        printf("P%d -> Priority: ", i + 1);
        scanf("%d", &procs[i].pty);

        printf("P%d: Burst time: ", i + 1);
        scanf("%f", &procs[i].bt);

        procs[i].pid = i + 1;
    }

    // sorting the array of proc structs based on priority.
    for (int i = 0; i < num_processes - 1; i++)
    {
        for (int j = 0; j < num_processes - i - 1; j++)
        {
            if (procs[j].pty > procs[j + 1].pty)
            {
                swap(&procs[j], &procs[j + 1]);
            }
        }
    }

    // calc waiting time
    double total_wt;
    procs[0].wt = 0;
    for (int i = 1; i < num_processes; i++)
    {
        procs[i].wt = 0;
        double wt;
        for (int j = 0; j < i; j++)
        {
            wt += procs[j].bt;
        }
        procs[i].wt = wt;
        total_wt += wt;
    }
    double avg_wt = total_wt / num_processes;

    // now calc turnaround time & print:
    double total_tat;
    printf("\nProcess\tPriority\tBurst Time\tWaiting time\t Turnaround Time\n");
    for (int i = 0; i < num_processes; i++)
    {
        procs[i].tat = procs[i].bt + procs[i].wt;
        printf("%4d %10d %14.2f %14.2f %14.2f\n", procs[i].pid, procs[i].pty, procs[i].bt, procs[i].wt, procs[i].tat);
        total_tat += procs[i].tat;
    }

    double avg_tat = total_tat / num_processes;

    printf("\nAverage waiting time = %lf\n", avg_wt);
    printf("Average turn around time = %lf", avg_tat);

    free(procs);   
    procs = NULL;

    return 0;
}
