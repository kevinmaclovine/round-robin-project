#include <cstdlib>
#include <exception>
#include <iostream>
#include <iterator>
#include <ostream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <climits>
#include <fstream>
#include <string>

using namespace std;


struct algorithm_factors{
  int quantom;
  int waiting_time;
  int response_time;
  int turn_around_time;
};
  
enum algorithm_factor{
  T,
  R,
  W
};

enum process_states{
  RUNNING,
  WAITING,
  READY,
  TERMINATE,
  NEW
};

class  process{
public:
  int pid;
  int total_burst;
  int arivaltime;
  vector <int> bursts;
  int waitingtime;
  int turnaroundtime;
  int responsetime;
  bool responsetimeflag;
  int cpu_time;
  process_states pstates;
  process(){
    total_burst = 0;
    responsetimeflag = false;
    waitingtime = 0;
    turnaroundtime = 0 ;
    responsetime = 0;
    pstates = NEW;
    cpu_time = arivaltime;  
  }
};
int insertation(vector<process> processes ,process  in_cpu)
{
    int index = 0; 
    for (auto i : processes ){
      if (in_cpu.cpu_time < i.cpu_time )
          break;
      index++;
    }
    return index;
}

void output_function (vector <process> processes, int dl , algorithm_factors container )
{
    ofstream file("output.txt");
    if (!file) {
        cout << "Failed to open file\n";
        exit(-1);
    }
    int p_numbers = processes.size();
    int idle_time = -1*dl;
    int current_time = -1 * dl;
    bool startflag = true;
    vector<process>process_queue;
    vector<process>i_o;
    int completed_process = 0;
    process in_cpu;
    file << "the best quantom time:"<<container.quantom<<endl;
    file << string(100, '-') << '\n';
    //algorithm:
    while(completed_process < processes.size()){
        for(int  i  = 0; i < processes.size() ; i++){                                                                             
          if ((processes[i].pstates == NEW && processes[i].arivaltime <= current_time) || (processes[i].pstates == NEW && processes[i].arivaltime <= current_time+dl && startflag)){                                             
            processes[i].pstates = READY;
            startflag = false;
            if (process_queue.empty() || processes[i].cpu_time <= process_queue.front().cpu_time)
              process_queue.push_back(processes[i]);
            else{
              int index = insertation(process_queue,processes[i]); 
              process_queue.insert(process_queue.begin()+ index , processes[i]);
              }                                                                                                                                                                       
          }else{
            continue;
          }
        }
      while (i_o.empty() != true && i_o.front().cpu_time <= current_time){
        if( process_queue.empty() || i_o.front().cpu_time <= process_queue.front().cpu_time)
          process_queue.push_back(i_o.front());
          //i_o.erase(i_o.begin());
        else{
          int index = insertation(process_queue,i_o.front()); 
          process_queue.insert(process_queue.begin()+ index , i_o.front());
        }
        i_o.erase(i_o.begin());
      }
      if (process_queue.empty()){
        current_time++;
        idle_time++;
        file <<" | idle | " << current_time;
        continue;
      }else{
        current_time+=dl;
        idle_time+=dl;
        in_cpu = process_queue.front();
        process_queue.erase(process_queue.begin());
        in_cpu.pstates = RUNNING;
        if (in_cpu.bursts.front() <= container.quantom){
          current_time += in_cpu.bursts.front();
          in_cpu.bursts.erase(in_cpu.bursts.begin());
          if (in_cpu.bursts.empty()){
            file<<" | "<<in_cpu.pid<<" | "<<current_time <<"ms";
            in_cpu.pstates = TERMINATE;
            completed_process++;
          }else{
            file<<" | "<<in_cpu.pid<<" | "<<current_time<<"ms";
            //file<<current_time;
            in_cpu.pstates = WAITING;
            in_cpu.cpu_time = in_cpu.bursts.front() + current_time;
            in_cpu.bursts.erase(in_cpu.bursts.begin());
            //i_o.push_back(in_cpu);
            if (i_o.empty()){                                                                               
                    i_o.push_back(in_cpu);                                                                        
            }else{                                                                                          
                    int index = insertation(i_o,in_cpu);                                                          
                    i_o.insert(i_o.begin()+ index , in_cpu);                                                      
            }    
          }        
        }else{
          current_time+=container.quantom;
          in_cpu.bursts[0]  -= container.quantom;
          file <<" | "<<in_cpu.pid<<" | "<<current_time << "ms";
          in_cpu.pstates = READY;
          if ( process_queue.empty() || in_cpu.cpu_time < process_queue.front().cpu_time)
            process_queue.push_back(in_cpu);
          else{
            int index = insertation(process_queue,in_cpu); 
            process_queue.insert(process_queue.begin()+ index , in_cpu);
          }
        }
      }
    }
    file <<endl;
    file << string(100, '-') << '\n';
    file << "avrage of response time:"<<(float)container.response_time/(float)p_numbers<<endl;
    file << "avrage of waiting time:"<<(float)container.waiting_time /(float) p_numbers <<endl;
    file<<"avrage of turn around time:"<<(float)container.turn_around_time /(float)p_numbers<<endl;
    int dif =  current_time - idle_time;
    float cu = (float)(dif)/(float)current_time;
    cu = cu *100;
    file<<"cpu utilization:"<<cu<<"%\n";
    file.close();
    cout<<R"(
    
 ,adPPYba,  ,adPPYba,  
a8"     "8a I8[    ""  
8b       d8  `"Y8ba,   
"8a,   ,a8" aa    ]8I  
 `"YbbdP"'  `"YbbdP"'  
    
    ")"<<endl;
};
void round_robin_algorithm(vector<process>processes  , int dl , int max_burst , algorithm_factor af )
{
  vector <process> persistance = processes; 
  vector<process>done;
  int current_time;
  bool startflag;
  vector<process>process_queue;
  vector<process>i_o;
  int completed_process;
  process in_cpu;
  int quantom;
  algorithm_factors min_wating_time , min_response_time , min_turnaround_time;
  min_wating_time.waiting_time = INT_MAX;
  min_response_time.response_time = INT_MAX; 
  min_turnaround_time.turn_around_time = INT_MAX;
  for (int j = 1 ; j <= max_burst ; j++){
    processes = persistance;
    current_time = -1 * dl;
    startflag = true;
    quantom = j;
    done.clear();
    completed_process = 0;
    while(completed_process < processes.size()){
        for(int  i  = 0; i < processes.size() ; i++){                                                                             
          if ((processes[i].pstates == NEW && processes[i].arivaltime <= current_time) || (processes[i].pstates == NEW && processes[i].arivaltime <= current_time+dl && startflag)){                                             
            processes[i].pstates = READY;
            startflag = false;
            if (process_queue.empty() || processes[i].cpu_time <= process_queue.front().cpu_time)
              process_queue.push_back(processes[i]);
            else{
              int index = insertation(process_queue,processes[i]); 
              process_queue.insert(process_queue.begin()+ index , processes[i]);
              }                                                                                                                                                                       
          }else{
            continue;
          }
        }
      while (i_o.empty() != true && i_o.front().cpu_time <= current_time){
        if( process_queue.empty() || i_o.front().cpu_time <= process_queue.front().cpu_time)
          process_queue.push_back(i_o.front());
        else{
          int index = insertation(process_queue,i_o.front()); 
          process_queue.insert(process_queue.begin()+ index , i_o.front());
        }
        i_o.erase(i_o.begin());
      }
      if (process_queue.empty()){
        current_time++;
        continue;
      }else{
        current_time+=dl;
        in_cpu = process_queue.front();
        //cout<<quantom<<"pid:"<<in_cpu.pid<<endl;
        process_queue.erase(process_queue.begin());
        in_cpu.pstates = RUNNING;
        in_cpu.waitingtime -= dl;
        if (in_cpu.bursts.front() <= quantom){
          current_time += in_cpu.bursts.front();
          in_cpu.bursts.erase(in_cpu.bursts.begin());
          if (in_cpu.bursts.empty()){
            in_cpu.pstates = TERMINATE;
            in_cpu.waitingtime += current_time - in_cpu.total_burst - in_cpu.arivaltime;
            //cout<<"waiting time of "<<in_cpu.pid<<"is "<<in_cpu.waitingtime<<endl; 
            in_cpu.turnaroundtime = current_time-in_cpu.arivaltime;
            //cout <<"q:"<<quantom <<" pid:"<<in_cpu.pid<< " curenttime:"<<current_time<<" arival time:"<<in_cpu.arivaltime<<" w:"<<in_cpu.waitingtime<<" r:"<<in_cpu.responsetime<<" t:"<<in_cpu.turnaroundtime<<endl;
            if (in_cpu.responsetimeflag == false){
              in_cpu.responsetime = in_cpu.turnaroundtime;
              in_cpu.responsetimeflag = true;
            }
            done.push_back(in_cpu);
            completed_process++;
          }else{
            in_cpu.pstates = WAITING;
            if (in_cpu.responsetimeflag == false){
              in_cpu.responsetime = current_time-in_cpu.arivaltime;
              //cout<<"ctime for response:"<<quantom<<" "<<current_time<<endl;
              in_cpu.responsetimeflag = true;
            }
            in_cpu.cpu_time = in_cpu.bursts.front() + current_time;
            in_cpu.bursts.erase(in_cpu.bursts.begin());
            if (i_o.empty()){
              i_o.push_back(in_cpu);
            }else{
              int index = insertation(i_o,in_cpu); 
              i_o.insert(i_o.begin()+ index , in_cpu);
            }
          }        
        }else{
          current_time+=quantom;
          in_cpu.bursts[0]  -= quantom; 
          in_cpu.pstates = READY;
          in_cpu.cpu_time = current_time;
          if ( process_queue.empty() || in_cpu.cpu_time < process_queue.front().cpu_time)
            process_queue.push_back(in_cpu);
          else{
            int index = insertation(process_queue,in_cpu); 
            process_queue.insert(process_queue.begin()+ index , in_cpu);
          }
        }
      }  
      //cout<<quantom<<":"<<current_time<<endl;    
    }
    int cal_waiting_time = dl;
    int cal_response_time = 0 ;
    int cal_turnaround_time = 0;
    for(auto ieach : done){
      cal_waiting_time += ieach.waitingtime;
      cal_response_time += ieach.responsetime;
      cal_turnaround_time += ieach.turnaroundtime; 
    }
    if (cal_turnaround_time < min_turnaround_time.turn_around_time ){
      min_turnaround_time.turn_around_time = cal_turnaround_time;
      min_turnaround_time.quantom = quantom;
      min_turnaround_time.response_time = cal_response_time;
      min_turnaround_time.waiting_time = cal_waiting_time;
      }
    if(cal_response_time < min_response_time.response_time){
      min_response_time.response_time = cal_response_time;
      min_response_time.quantom = quantom;
      min_response_time.turn_around_time = cal_turnaround_time;
      min_response_time.waiting_time = cal_waiting_time;
      }
    if(cal_waiting_time < min_wating_time.waiting_time){
      min_wating_time.waiting_time = cal_waiting_time;
      min_wating_time.quantom = quantom;
      min_wating_time.response_time = cal_response_time;
      min_wating_time.turn_around_time = cal_turnaround_time;
      }
  }

  switch (af) {
        case T:
          output_function(persistance , dl, min_turnaround_time);
          return;          
          break;
        case R:
          output_function(persistance, dl, min_response_time);
          return;
          break;
        case W:
          output_function(persistance, dl, min_wating_time);     
          return;
          break;
        default:
          exit(-1);
      }
}
void input_function(char* argument)
{
    ifstream file(argument);
    if(!file){
      perror("no such file");
      exit(-1);
    }
    int max_burst = INT_MIN;
    int dl;
    string line;
    char character;
    vector<process>processes;
    algorithm_factor input_factor;
    file.get(character);
    switch (character) {
      case 'W':
      case 'w':
        input_factor = W;
        break;
      case 'T':
      case 't':
        input_factor = T;
        break;
      case 'R':
      case 'r':
        input_factor = R;
        break;
      default:
        exit(-1);
    }
    getline(file , line);
    file >> dl;
    getline(file,line);
    while(getline(file , line)){
      process inputprocess;
      size_t colonPos = line.find(':');
      string label = line.substr(0, colonPos);   
      string numbers = line.substr(colonPos + 1);
      int labelNumber = 0;
      for (char c : label) {
        if (isdigit(c)) {
            labelNumber = labelNumber * 10 + (c - '0');
        }
      }
      inputprocess.pid = labelNumber;
      stringstream ss(numbers);
      string token;
      getline(ss, token, ',');
      int inputarivaltime = stoi(token);
      inputprocess.arivaltime = inputarivaltime;
      int checker = 1 ;
      while (getline(ss, token, ',')) {
          int inputburst = stoi(token);
          inputprocess.total_burst += inputburst;
          inputprocess.bursts.push_back(inputburst);
          if(checker%2 != 0 && inputburst > max_burst)
            max_burst = inputburst;
          checker++;
      }
      processes.push_back(inputprocess);
    }    
    round_robin_algorithm(processes, dl, max_burst, input_factor);
}

int main(int argc , char** argv)
{
  if (argc != 2){
    perror("invalid argumnet!");
    exit(-1);
  }
  input_function(argv[1]);
  return 0;
}
