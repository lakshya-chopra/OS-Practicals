#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <bits/time.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct Process
{
    int pid;
    long aTime;
    int bTime;
    int pty;
    int remTime; // remaining time
    int taTime;  // turnaround time
    bool isCompleted;
} p;

int highestPty = 1;

long timeDiff(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000; // ms
}

void sched_procs(p *procs, int num_proc)
{
    // Pre-emptive priority scheduling
    int currentTime = 0, completed = 0;
    float totalWaitTime = 0, totalTurnaroundTime = 0;
    struct timespec startTime, currentRealTime;
    struct timespec delay = {0, 1000000};

    // Get the starting time (real-time clock)
    clock_gettime(CLOCK_REALTIME, &startTime);

    printf("\nProcess\t Burst Time\tWaiting time\tTurnaround Time\n");

    while (completed < num_proc)
    {
        clock_gettime(CLOCK_REALTIME, &currentRealTime);
        long elapsedTime = timeDiff(startTime, currentRealTime); // ms
        currentTime = (int)elapsedTime;
        int selectedPty = __INT_MAX__; // default
        int selectedProc = -1;
        // printf("curr time: %d\n", currentTime);

        for (int i = 0; i < num_proc; i++)
        {

            if (procs[i].pty <= selectedPty && procs[i].aTime <= currentTime && !procs[i].isCompleted)
            {

                selectedPty = procs[i].pty;
                selectedProc = i;
            }
        }

        if (selectedProc == -1)
        {
            // idle
            //  printf("Idle\n");
            //  currentTime++;
            long minArrival = __INT_MAX__;
            for (int i = 0; i < num_proc; i++)
            {
                if (!procs[i].isCompleted && procs[i].aTime > currentTime)
                {
                    minArrival = (procs[i].aTime < minArrival) ? procs[i].aTime : minArrival;
                }
            }
            currentTime = (minArrival != __INT_MAX__) ? minArrival : currentTime + 1;
        }
        else
        {
            // printf("here\n");
            p *proc = &procs[selectedProc];
            proc->remTime--;
            // currentTime++;
            // sleep(1);
            nanosleep(&delay, NULL);

            if (proc->remTime == 0)
            {
                // printf("zero rem time\n");
                proc->isCompleted = true;
                proc->taTime = currentTime - proc->aTime;
                totalTurnaroundTime += proc->taTime;
                float wt = proc->taTime - proc->bTime;
                totalWaitTime += wt;

                printf("%4d %14.2d %14.2lf %14.2d\n", proc->pid, proc->bTime, wt, proc->taTime);
                completed++;
            }
        }
    }
    double avgTat = totalTurnaroundTime / num_proc;
    double avgWT = totalWaitTime / num_proc;

    printf("\nAverage waiting time = %lf\n", avgWT);
    printf("Average turn around time = %lf", avgTat);
}

int main()
{

    int num_processes;
    printf("Enter the number of processes:\n");
    scanf("%d", &num_processes);

    p *procs = (p *)malloc(sizeof(p) * num_processes); // not using VLA

    for (int i = 0; i < num_processes; i++)
    {
        printf("Enter arrival time (in ms), burst time (in ms), and priority for process %d: ", i + 1);
        scanf("%ld %d %d", &procs[i].aTime, &procs[i].bTime, &procs[i].pty);
        procs[i].pid = i + 1;
        procs[i].remTime = procs[i].bTime;
        procs[i].isCompleted = false;
        procs[i].pid = i + 1;
    }

    sched_procs(procs, num_processes);
    free(procs);
    procs = NULL;
}
