#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct Process
{
    int pid;          // Process ID
    long aTime;       // Arrival time (ms)
    int bTime;        // Burst time (ms)
    int remTime;      // Remaining time (ms)
    int taTime;       // Turnaround time (ms)
    int wTime;        // Waiting time (ms)
    bool isCompleted; // Process completion status
} p;

long timeDiff(struct timespec start, struct timespec end)
{
    return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000; // ms
}

void sched_procs(p *procs, int num_proc)
{
    int completed = 0, currentTime = 0;
    float totalWaitTime = 0, totalTurnaroundTime = 0;
    struct timespec startTime, currentRealTime;

    clock_gettime(CLOCK_REALTIME, &startTime);

    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");

    while (completed < num_proc)
    {
        clock_gettime(CLOCK_REALTIME, &currentRealTime);
        long elapsedTime = timeDiff(startTime, currentRealTime);
        currentTime = (int)elapsedTime;

        // Select process with the shortest remaining time
        int shortestJob = -1;
        for (int i = 0; i < num_proc; i++)
        {
            if (!procs[i].isCompleted && procs[i].aTime <= currentTime)
            {
                if (shortestJob == -1 || procs[i].remTime < procs[shortestJob].remTime)
                {
                    shortestJob = i;
                }
            }
        }

        if (shortestJob == -1)
        {
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

        p *proc = &procs[shortestJob];
        proc->remTime--;
        usleep(1000); // 1ms

        if (proc->remTime == 0)
        {
            proc->isCompleted = true;
            completed++;
            proc->taTime = currentTime - proc->aTime;
            proc->wTime = proc->taTime - proc->bTime;
            totalTurnaroundTime += proc->taTime;
            totalWaitTime += proc->wTime;

            printf("%4d %14d %14d %14d\n", proc->pid, proc->bTime, proc->wTime, proc->taTime);
        }
    }

    printf("\nAverage Waiting Time = %.2f\n", totalWaitTime / num_proc);
    printf("Average Turnaround Time = %.2f\n", totalTurnaroundTime / num_proc);
}

int main()
{
    int num_processes;

    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);

    p *procs = (p *)malloc(sizeof(p) * num_processes);

    for (int i = 0; i < num_processes; i++)
    {
        printf("Enter arrival time (in ms), burst time (in ms) for process %d: ", i + 1);
        scanf("%ld %d", &procs[i].aTime, &procs[i].bTime);
        procs[i].pid = i + 1;
        procs[i].remTime = procs[i].bTime;
        procs[i].isCompleted = false;
    }

    sched_procs(procs, num_processes);
    free(procs);
    return 0;
}
