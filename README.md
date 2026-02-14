# round-robin-project
in this project we have an input file which contain processes and it will simulate the RR algorithm and finds the best quantom for it. 
input file looks like:
char (one of three (T,W,R) character for calculating quantom time by terminologies such as:
turn around time , waiting time , response time )
integer (dispatcher latency (time that cpu costs to switch between process))
number of process : arival time , cpu burst , i/o burst , ...
let's see an example of input file to understand it better:
W
1
P1:0,4,3,2
P2:1,5,2,3
P3:3,2,4,1
and for output programm will create a file with calculating terminologies and gant chart
example of output:

the best quantom time:4
----------------------------------------------------------------------------------------------------
 | 1 | 4ms | 2 | 9ms | 3 | 12ms | 2 | 14ms | 1 | 17ms | 3 | 19ms | 2 | 23ms
----------------------------------------------------------------------------------------------------
avrage of response time:9.66667
avrage of waiting time:7
avrage of turn around time:17.6667
cpu utilization:73.913%
