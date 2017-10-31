#ifndef HELPER_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <queue>
#include <sstream>
#include <map>

#define HELPER_H

using namespace std;

struct Task
{
	int id;
	int deadline;
	int release_time;
	int exe_time;
	int slack_time;
	int period;
	Task(int id, int release_time, int deadline, int exe_time, int period){
		this->id = id;
		this->release_time = release_time;
		this->deadline = deadline;
		this->exe_time = exe_time;
		this->period = period;
		this->slack_time = deadline - exe_time;
	}
	string print(){
		stringstream ss;
		ss << "id: "<<this->id<< " release_time: "<<this->release_time<<" deadline: "
			<<this->deadline<<" exe_time: "<<this->exe_time<< " period: "<<this->period;
		return ss.str();
	}
};


struct compareByDeadline
{
  bool operator()(const Task* lhs, const Task* rhs) const
  {
    return lhs->deadline > rhs->deadline;
  }
};


struct compareBySlacktime
{
  bool operator()(const Task* lhs, const Task* rhs) const
  {
    return lhs->slack_time > rhs->slack_time;
  }
};


struct compareByPeriod
{
  bool operator()(const Task* lhs, const Task* rhs) const
  {
    return lhs->period > rhs->period;
  }
};

class Generator{
private:
	map<Task*, int> tasks_map;
	queue<Task*> ready_q;
public:
	Generator(){};
	~Generator(){};
	void increaseTime_1(){
		for(auto it : this->tasks_map){
			this->tasks_map[it.first]--;
			// cout<<it.first->id << " "<<it.second<<endl;
			if(it.second <= 0){
				ready_q.push(it.first);
			}
		}
	}

	bool hasReady(){
		return !this->ready_q.empty();
	}

	void push(Task* task){
		if(this->tasks_map.find(task) != this->tasks_map.end()){
			cout<<"ERROR: Task duplicate!"<<endl;
		}else{
			if(task->release_time != 0)
				this->tasks_map.insert(make_pair(task, task->release_time - 1));
			else
				this->tasks_map.insert(make_pair(task, task->period - 1));
		}
	}

	Task* pop(){
		Task* res = this->ready_q.front();
		// interruption should be removed.
		if(res->period == 0){
			this->tasks_map.erase(res);
		}else{
			this->tasks_map[res] = res->period - 1;
		}
		this->ready_q.pop();
		return res;
	}

	bool empty(){
		return this->ready_q.empty() && this->tasks_map.empty();
	}
};

class EDF{
private:
	double utilization;
	Generator* gen;
	priority_queue<Task*, vector<Task*>, compareByDeadline> pq;
	int remain_calculation_time;
	int time_cnt;
public:
	EDF(){
		gen = new Generator();
		utilization = 0;
		time_cnt = 0;
	};
	~EDF();

	void increaseTime_1(){
		// 1. time_cnt
		// 2. if(running task done) pop nex task
		// 3. reset remain_time
		// 4. gen upate
		this->time_cnt++;
		this->remain_calculation_time--;
		cout<<" Time: "<<time_cnt<<"  ";
		if(this->remain_calculation_time <= 0){
			Task* task = this->pop();
			
			if(task == NULL){
				cout<<"      waiting task "<<endl;
			}else{
				cout<<task->print()<<endl;
			}
		}else{
			cout<<"      running task "<<endl;
		}

		gen->increaseTime_1();
		while(gen->hasReady()){
			this->pushFromGen(gen->pop());
		}
	}

	bool push(Task* task){
		if(task->release_time==0 && task->period != 0){
			if(utilization + (double) task->exe_time / task->period > 1){
				cout<<"ERROR: utilization overload. Task: "<<task->id<< " Added to the Generator"<<endl;
				gen->push(task);
				return false;
			}
			utilization += (double) task->exe_time / task->period;
			gen->push(task);
		}else if(task->release_time != 0){
			gen->push(task);
			return true;
		}
		this->pq.push(task);
		return true;		
	}

	bool pushFromGen(Task* task){
		if(task->period != 0){
			if(utilization + (double) task->exe_time / task->period > 1){
				cout<<"ERROR: utilization overload, skip Task: "<<task->id<< " this time. "<<endl;
				return false;
			}
			utilization += (double) task->exe_time / task->period;			
		}
		cout<<"push: "<<task->id<<endl;
		this->pq.push(task);
		return true;
	}

	Task* pop(){
		if(this->pq.empty()){return NULL;}
		Task* res = this->pq.top();
		this->remain_calculation_time = this->pq.empty() ? 0 : this->pq.top()->exe_time;
		this->pq.pop();

		if(res->period != 0){
			utilization -= (double) res->exe_time / res->period;			
		}
		return res;		
	}

	void getU(){
		cout<<"utilization: "<<utilization<<endl;
	}

	bool empty(){
		return this->pq.empty() && this->gen->empty();
	}
};


class RMS{
private:
	double utilization;
	Generator* gen;
	priority_queue<Task*, vector<Task*>, compareByPeriod> pq;
	int remain_calculation_time;
	int time_cnt;
public:
	RMS(){
		gen = new Generator();
		utilization = 0;
		time_cnt = 0;
	};
	~RMS();

	void increaseTime_1(){
		// 1. time_cnt
		// 2. if(running task done) pop nex task
		// 3. reset remain_time
		// 4. gen upate
		this->time_cnt++;
		this->remain_calculation_time--;
		cout<<" Time: "<<time_cnt<<"  ";
		if(this->remain_calculation_time <= 0){
			Task* task = this->pop();
			
			if(task == NULL){
				cout<<"      waiting task "<<endl;
			}else{
				cout<<task->print()<<endl;
			}
		}else{
			cout<<"      running task "<<endl;
		}

		gen->increaseTime_1();
		while(gen->hasReady()){
			this->pushFromGen(gen->pop());
		}
	}

	bool push(Task* task){
		if(task->release_time==0 && task->period != 0){
			if(utilization + (double) task->exe_time / task->period > 1){
				cout<<"ERROR: utilization overload. Task: "<<task->id<< " Added to the Generator"<<endl;
				gen->push(task);
				return false;
			}
			utilization += (double) task->exe_time / task->period;
			gen->push(task);
		}else if(task->release_time != 0){
			gen->push(task);
			return true;
		}
		this->pq.push(task);
		return true;		
	}

	bool pushFromGen(Task* task){
		if(task->period != 0){
			if(utilization + (double) task->exe_time / task->period > 1){
				cout<<"ERROR: utilization overload, skip Task: "<<task->id<< " this time. "<<endl;
				return false;
			}
			utilization += (double) task->exe_time / task->period;			
		}
		cout<<"push: "<<task->id<<endl;
		this->pq.push(task);
		return true;
	}

	Task* pop(){
		if(this->pq.empty()){return NULL;}
		Task* res = this->pq.top();
		this->remain_calculation_time = this->pq.empty() ? 0 : this->pq.top()->exe_time;
		this->pq.pop();

		if(res->period != 0){
			utilization -= (double) res->exe_time / res->period;			
		}
		return res;		
	}

	void getU(){
		cout<<"utilization: "<<utilization<<endl;
	}

	bool empty(){
		return this->pq.empty() && this->gen->empty();
	}
};

class LST{
private:
	double utilization;
	Generator* gen;
	priority_queue<Task*, vector<Task*>, compareBySlacktime> pq;
	int remain_calculation_time;
	int time_cnt;
public:
	LST(){
		gen = new Generator();
		utilization = 0;
		time_cnt = 0;
	};
	~LST();

	void increaseTime_1(){
		// 1. time_cnt
		// 2. if(running task done) pop nex task
		// 3. reset remain_time
		// 4. gen upate
		this->time_cnt++;
		this->remain_calculation_time--;
		cout<<" Time: "<<time_cnt<<"  ";
		if(this->remain_calculation_time <= 0){
			Task* task = this->pop();
			
			if(task == NULL){
				cout<<"      waiting task "<<endl;
			}else{
				cout<<task->print()<<endl;
			}
		}else{
			cout<<"      running task "<<endl;
		}

		gen->increaseTime_1();
		while(gen->hasReady()){
			this->pushFromGen(gen->pop());
		}
	}

	bool push(Task* task){
		if(task->release_time==0 && task->period != 0){
			if(utilization + (double) task->exe_time / task->period > 1){
				cout<<"ERROR: utilization overload. Task: "<<task->id<< " Added to the Generator"<<endl;
				gen->push(task);
				return false;
			}
			utilization += (double) task->exe_time / task->period;
			gen->push(task);
		}else if(task->release_time != 0){
			gen->push(task);
			return true;
		}
		this->pq.push(task);
		return true;		
	}

	bool pushFromGen(Task* task){
		if(task->period != 0){
			if(utilization + (double) task->exe_time / task->period > 1){
				cout<<"ERROR: utilization overload, skip Task: "<<task->id<< " this time. "<<endl;
				return false;
			}
			utilization += (double) task->exe_time / task->period;			
		}
		cout<<"push: "<<task->id<<endl;
		this->pq.push(task);
		return true;
	}

	Task* pop(){
		if(this->pq.empty()){return NULL;}
		Task* res = this->pq.top();
		this->remain_calculation_time = this->pq.empty() ? 0 : this->pq.top()->exe_time;
		this->pq.pop();

		if(res->period != 0){
			utilization -= (double) res->exe_time / res->period;			
		}
		return res;		
	}

	void getU(){
		cout<<"utilization: "<<utilization<<endl;
	}

	bool empty(){
		return this->pq.empty() && this->gen->empty();
	}
};



#endif

