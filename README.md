# Compile: 
$ gcc Scheduler.c -o Scheduler.exe

# Execute: 
$ Scheduler.exe < input1.txt > out1.txt | Scheduler.exe < input2.txt > out2.txt

# Subject:  
C programming

# Problem:  
Building and simulating a  Round-Robin  scheduler using linked list and queue data structures

# Description: 
In time shared systems, tasks submitted to the system are assigned to a limited  number of processors for execution in a time shared manner. 
Time allocation of processors to tasks is known as scheduling  problem. One of the most prevalent scheduling policy employed in embedded systems is Round-Robin. 
The policy always executes the next task from the waiting task list (implemented as a queue) in a circular fashion. 
In this assignment, you are expected to write a C console program simulating a Round-Robin scheduler on a single processor system.  
The functionalities and constraints are specified below.

•  Each task has a task identifier which is a string. You may assume that no two tasks are given the same identifier. Additionally, each task is assigned with a positive integer denoting the runtime (the total time the task must spend on the processor) of the respective task.
•  There is a (waiting) task list, implemented as a queue, keeping all the tasks submitted to the system but not completed  yet,  i.e.,  partially completed tasks.
•  At a time, the scheduler picks the next task (according to the policy) from the task list and executes it for 1 unit time. This step decrements the runtime of the task by 1 unit. In case the runtime reaches 0, the task  is  added  to  the  completed  tasks  queue. Otherwise, the task is reinserted to the task list. This process is called a simulation step and repeated.
•  New tasks can be submitted to the system at any time. An entry in the completed tasks queue keeps the respective task identifier, its initial runtime and completion time (the number of time units elapsed since the submission till the completion).  Note that, the completion time is initial runtime plus the time spent in the task list.
•  The system time is 0 at the beginning, and increases by 1 unit at each simulation step.
•  Whenever the task list becomes empty, the processor goes on idle. Assume that as the processor goes on idle, the system time progresses 1 unit only, regardless of the actual real time spent during which.

Program: Your program should accept commands from the command prompt in a loop.

# Commands:  
(The format is as follows)

•  Start a new task :           
    > SNT task_name runtime
•  Simulate k  units :         
    > S k
•  Display system time:         
    > DST
•  Display total (idle) time:   
    > DTIT
•  Display the information of the most recently completed task:  
    > DIMRCT
•  Dump (but not delete) completed tasks queue content into a text file (in increasing order of completion time,  i.e.,  the most recent last):  
    > DCTQ file_name
•  Empty completed tasks queue:     
    > ECTS
•  Quit the program:                
    > QUIT
