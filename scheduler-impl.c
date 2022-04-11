//1155144676 Yeung Tang, email: 1155144676@link.cuhk.edu.hk
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
int min(int x, int y); // Return the less one between x and y.

Process MinProc(Process x, Process y); // Return the process arrive earlier; if arrive at the same time, return the one have less pid.

void SortProcess(Process* p, int num); // Sort proc arrording to arrival_time and pid.

Process MinnProc(Process x, Process y){
    Process min_proc;

    if (x.process_id < y.process_id)
        min_proc = x;
    else if (x.process_id > y.process_id)
        min_proc = y;

    return min_proc;
}

void SorttProcess(Process* p, int num){
    Process* a = p;
    Process* b = (Process*)malloc(num * sizeof(Process));
    int seg, start;
    for (seg = 1; seg < num; seg += seg) {
        for (start = 0; start < num; start += seg + seg) {
            int low = start, mid = min(start + seg, num), high = min(start + seg + seg, num);
            int k = low;
            int start1 = low, end1 = mid;
            int start2 = mid, end2 = high;
            // printf("------------4\n");
            while (start1 < end1 && start2 < end2){
                Process minproc = MinnProc(a[start1], a[start2]);
                if (minproc.process_id == a[start1].process_id)
                    b[k++] = a[start1++];
                else
                    b[k++] = a[start2++];
            }
                // b[k++] = MinProc(a[start1], a[start2]);
            // printf("------------5\n");
            while (start1 < end1)
                b[k++] = a[start1++];
            // printf("------------6\n");
            while (start2 < end2)
                b[k++] = a[start2++];
        }
        Process* tmp = a;
        a = b;
        b = tmp;
    }
    if (a != p) {
        int i;
        for (i = 0; i < num; i++)
            b[i] = a[i];
        b = a;
    }
    free(b);
}


void Calculate(Process* proc, int proc_num){
    proc[0].service_time = proc[0].arrival_time;
    proc[0].waiting_time = 0;
    for(int i=1;i<proc_num;i++){
		proc[i].service_time = proc[i-1].service_time+proc[i-1].execution_time;
        if (proc[i].service_time < proc[i].arrival_time)
            proc[i].service_time = proc[i].arrival_time;
		proc[i].waiting_time = proc[i].service_time-proc[i].arrival_time;
        if(proc[i].waiting_time<0)
			proc[i].waiting_time=0;
	}

	for(int i=0;i<proc_num;i++){
		proc[i].turnaround_time = proc[i].execution_time + proc[i].waiting_time;
	}
	//printf("\n\n");
	//printf("Process\tExecution Time\tArrival Time\tWaiting Time\tTurnAround Time\tCompletion Time\t Service time\n");
	int total_waiting_time=0,total_turn_around_time=0;
	for(int i=0;i<proc_num;i++){
		total_waiting_time+=proc[i].waiting_time;
		total_turn_around_time+=proc[i].turnaround_time;
		proc[i].completion_time=proc[i].turnaround_time + proc[i].arrival_time;
		//printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",proc[i].process_id,proc[i].execution_time, proc[i].arrival_time, proc[i].waiting_time,proc[i].turnaround_time,proc[i].completion_time, proc[i].service_time);
	}
	//printf("Average waiting time: %f\n", (float)total_waiting_time/proc_num);
	//printf("Average turnaround time: %f\n\n",(float)total_turn_around_time/proc_num);
}

void outprint(int time_x, int time_y, int pid, int arrival_time, int remaining_time);

void scheduler(Process* proc, LinkedQueue** ProcessQueue, int proc_num, int queue_num, int period){
    printf("Process number: %d\n", proc_num);
    for (int i = 0;i < proc_num; i++)
        printf("%d %d %d\n", proc[i].process_id, proc[i].arrival_time, proc[i].execution_time);

    printf("\nQueue number: %d\n", queue_num);
    printf("Period: %d\n", period);
    for (int i = 0;i < queue_num; i++){
        printf("%d %d %d\n", i, ProcessQueue[i]->time_slice, ProcessQueue[i]->allotment_time);
    }
    Calculate(proc, proc_num); //do calculation for process
    int time_slice[queue_num];

    for (int i = 0;i < queue_num; i++)
    time_slice[i] = ProcessQueue[i]->time_slice; //queue num -1 to use, -- for lower piority]
    int slice_used = 0;
    int slice_offset = 0;
    int current_proc = 0;
    int tmp_time = 0;
    int flag = -1;
    int breaktime = 0;
    int usingqueue = 0;
    Process de_proc;
    Process front_proc;
    Process tmp_proc;
    int totaltime = proc[proc_num-1].completion_time;
    int neww =0;
    int Max =0;
    int newflag =0;
	for(int i=0;i<proc_num;i++){
		proc[i].service_time = 0;
		proc[i].turnaround_time = 0;
	}



    /*
       Test outprint function, it will output "Time_slot:1-2, pid:3, arrival-time:4, remaining_time:5" to output.loc file.
    */

    do {

        for (int i=0; i<proc_num; i++)
        if (tmp_time == proc[proc_num-i-1].arrival_time && ((tmp_time) % period != 0)){
            ProcessQueue[queue_num-1] = EnQueue(ProcessQueue[queue_num-1], proc[proc_num-i-1]);
            //printf("new added");
            }


         //Find which queue is using currently, from top queue to lowest

        for (int i=0; i<queue_num; i++)
             if (!IsEmptyQueue(ProcessQueue[queue_num-1-i])&&flag ==-1){
                    flag = queue_num-1-i;
                    front_proc = FrontQueue(ProcessQueue[flag]);
                    break;

                //printf("At time tmp_time %d, i add jor pid: %d at time slice %d", tmp_time, front_proc.process_id, ProcessQueue[flag]->time_slice)
            }

            if(flag >= 0){
            if(front_proc.turnaround_time == 0)
            front_proc.completion_time = tmp_time + ProcessQueue[flag]->time_slice;

            front_proc.turnaround_time++;
            front_proc.service_time++;
            front_proc.execution_time --;
            for (int i=0; i<proc_num; i++)
                if(front_proc.process_id == proc[i].process_id)
                    proc[i].execution_time = front_proc.execution_time;
            //This is use to check if the process is already finished
        if((tmp_time+1) % period == 0){ //if the time for the big wipe has began
        //printf("lolz big wipeee");
        int cucupid = front_proc.process_id;

        outprint(front_proc.completion_time - ProcessQueue[flag]->time_slice,tmp_time + 1,front_proc.process_id,front_proc.arrival_time ,front_proc.execution_time);
        //outprint(1,1,1,1,1);

        //del all queue and reset values also updatethe exe time
        for (int i=0; i<queue_num; i++)
             while (!IsEmptyQueue(ProcessQueue[i])){
                de_proc = DeQueue(ProcessQueue[i]);
                //printf("At time tmp_time %d, i add jor pid: %d at time slice %d", tmp_time, front_proc.process_id, ProcessQueue[flag]->time_slice)
                int cupid = de_proc.process_id;
                for(int i=0;i<proc_num;i++){
                    proc[i].service_time = 0;
                    proc[i].turnaround_time = 0;


                }
            }

            //sort the pids to queue in later
            SorttProcess(proc, proc_num);


            //queue back the process
           for (int i=0; i<proc_num; i++){
            if (tmp_time+1 >= proc[proc_num-i-1].arrival_time&&proc[proc_num-i-1].execution_time!=0){
                //printf("\nproc[proc_num-i-1] is : %d and my pid is %d\n", proc[proc_num-i-1].execution_time, proc[proc_num-i-1].process_id);
                ProcessQueue[queue_num-1] = EnQueue(ProcessQueue[queue_num-1], proc[proc_num-i-1]);//enqueu to the topmost priority queue
            }
        }


        flag = -1;
        //addd back all queue
        //filter out ext =0 and arrive time > tmp -> put it in
        SortProcess(proc, proc_num);
        }else if(front_proc.execution_time <= 0){

        outprint(front_proc.completion_time - ProcessQueue[flag]->time_slice,tmp_time + 1,front_proc.process_id,front_proc.arrival_time ,front_proc.execution_time);
        front_proc.turnaround_time =0;
        front_proc.service_time =0;
        //Dequeue all
        de_proc = DeQueue(ProcessQueue[flag]);
        flag = -1;


        }else if(front_proc.service_time >= ProcessQueue[flag]->allotment_time){ //This is use to lower the piority of the process
        //resets
        front_proc.turnaround_time = 0;
        front_proc.service_time = 0;
        outprint(front_proc.completion_time - ProcessQueue[flag]->time_slice,front_proc.completion_time,front_proc.process_id,front_proc.arrival_time ,front_proc.execution_time);
        de_proc = DeQueue(ProcessQueue[flag]);

        if(flag-1 >= 0)
        EnQueue(ProcessQueue[flag-1], front_proc);  //downgrade to lower piority queue
        else
        EnQueue(ProcessQueue[flag], front_proc); //back to original queue

        //printf("\n im queueing the process: %d now @ time: %d!! \n", front_proc.process_id, tmp_time);

        flag = -1;
        }else if(front_proc.turnaround_time == ProcessQueue[flag]->time_slice){ //This is use to throw it at the back


        front_proc.turnaround_time = 0;
        outprint(front_proc.completion_time - ProcessQueue[flag]->time_slice,front_proc.completion_time,front_proc.process_id,front_proc.arrival_time ,front_proc.execution_time);
        de_proc = DeQueue(ProcessQueue[flag]);
        EnQueue(ProcessQueue[flag], front_proc); //back to original queue

        flag =-1;
        }

    }
        if (tmp_time >= totaltime) break;
    } while (++tmp_time);





}
