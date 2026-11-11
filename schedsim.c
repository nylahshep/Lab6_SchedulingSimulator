// C program for implementation of Simulation 
#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"


// Function to find the waiting time for all  
// processes

void findWaitingTimeRR(ProcessType plist[], int n, int quantum)
{
    int rem_bt[n];
    int t = 0;
    int done;

    // Copy burst times and initialize waiting times
    for (int i = 0; i < n; i++) {
        rem_bt[i] = plist[i].bt;
        plist[i].wt = 0;
    }

    // Keep going until every process finishes
    do {
        done = 1;

        for (int i = 0; i < n; i++) {
            if (rem_bt[i] > 0) {
                done = 0;

                // Process needs more than one quantum
                if (rem_bt[i] > quantum) {
                    t += quantum;
                    rem_bt[i] -= quantum;
                }
                else {
                    // Process finishes during this turn
                    t += rem_bt[i];
                    plist[i].wt = t - plist[i].bt;
                    rem_bt[i] = 0;
                }
            }
        }
    } while (!done);
}


// Function to find the waiting time for all  
// processes 

void findWaitingTimeSJF(ProcessType plist[], int n)
{
    int rem_bt[n];
    int completed = 0;
    int t = 0;

    // Initialize remaining burst times and waiting times
    for (int i = 0; i < n; i++) {
        rem_bt[i] = plist[i].bt;
        plist[i].wt = 0;
    }

    while (completed < n) {
        int shortest = -1;
        int min_bt = INT_MAX;

        // Find shortest remaining process that has arrived
        for (int i = 0; i < n; i++) {
            if (plist[i].art <= t &&
                rem_bt[i] > 0 &&
                rem_bt[i] < min_bt) {

                min_bt = rem_bt[i];
                shortest = i;
            }
        }

        // No process available: CPU stays idle
        if (shortest == -1) {
            t++;
            continue;
        }

        // Run selected process for one time unit
        rem_bt[shortest]--;
        t++;

        // Record waiting time when process finishes
        if (rem_bt[shortest] == 0) {
            completed++;
            plist[shortest].wt =
                t - plist[shortest].art - plist[shortest].bt;
        }
    }
}


// Function to find the waiting time for all  
// processes 
void findWaitingTime(ProcessType plist[], int n)
{ 
    // waiting time for first process is 0, or the arrival time if not 
    plist[0].wt = 0 + plist[0].art;
  
    // calculating waiting time 
    for (int  i = 1; i < n ; i++ ) 
        plist[i].wt =  plist[i-1].bt + plist[i-1].wt; 
} 
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  
// Function to sort the Process acc. to priority
int my_comparer(const void *this, const void *that)
{
    const ProcessType *p1 = (const ProcessType *)this;
    const ProcessType *p2 = (const ProcessType *)that;

    // Sort by priority (highest first)
    if (p1->pri < p2->pri)
        return 1;

    if (p1->pri > p2->pri)
        return -1;

    // Equal priority: preserve FCFS order
    return 0;
} 

void findavgTimeFCFS(ProcessType plist[], int n)
{
    // Calculate waiting times
    findWaitingTime(plist, n);

    // Calculate turnaround times
    findTurnAroundTime(plist, n);

    // Display results
    printf("\n*********\nFCFS\n");
}
//Function to calculate average time 
void findavgTimePriority(ProcessType plist[], int n)
{
    // Sort processes by priority (highest first)
    qsort(plist, n, sizeof(ProcessType), my_comparer);

    // Calculate waiting times using FCFS
    findWaitingTime(plist, n);

    // Calculate turnaround times
    findTurnAroundTime(plist, n);

    // Display results
    printf("\n*********\nPriority\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}


void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turn  
    // around time 
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
  // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
  
  // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);
  
  // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
    printMetrics(proc_list, n);
    
  // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 