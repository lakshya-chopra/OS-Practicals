#include<stdio.h> 

void findWaitingTime(int processes[], int n,  
                          int bt[], int wt[]) 
{ 
    // waiting time for first process is 0 
    wt[0] = 0; 
  
    for (int  i = 1; i < n ; i++ ) 
        wt[i] =  bt[i-1] + wt[i-1] ; 
} 
  
void findTurnAroundTime( int processes[], int n,  
                  int bt[], int wt[], int tat[]) 
{ 

    for (int  i = 0; i < n ; i++) 
        tat[i] = bt[i] + wt[i]; 
} 
  
void findavgTime( int processes[], int n, int bt[]) 
{ 
    int wt[n], tat[n], total_wt = 0, total_tat = 0; 
  
    findWaitingTime(processes, n, bt, wt); 
  
    findTurnAroundTime(processes, n, bt, wt, tat); 
  
    printf("Processes\tBurst time\tWaiting time\tTurn around time\n"); 
  
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + wt[i]; 
        total_tat = total_tat + tat[i]; 

        printf("%4d %17d %17d %17d\n",i+1,bt[i],total_wt,total_tat);

    } 
    float s=(float)total_wt / (float)n;
    float t=(float)total_tat / (float)n;
    printf("Average waiting time = %f",s);
    printf("\n");
    printf("Average turn around time = %f",t); 
} 
  
int main() 
{ 
    int processes[] = { 1, 2, 3}; 
    int n = sizeof processes / sizeof processes[0]; 
  
    int  burst_time[] = {10, 5, 8}; 
  
    findavgTime(processes, n,  burst_time); 
    return 0; 
} 