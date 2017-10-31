#include "helper.h"
#include <unistd.h>
using namespace std;


// Task(TaskType type, int release_time, int deadline, int exe_time, int period)
int main(){
	EDF* edf = new EDF();
	edf->push(new Task(1, 0, 3, 1, 10));
	edf->push(new Task(2, 0, 4, 1, 10));
	edf->push(new Task(3, 0, 5, 1, 10));
	edf->push(new Task(4, 0, 6, 1, 10));
	edf->push(new Task(5, 0, 7, 1, 10));
	edf->push(new Task(6, 0, 8, 1, 10));
	edf->push(new Task(7, 0, 9, 1, 10));
	while(!edf->empty()){
		usleep(200000);
		edf->increaseTime_1();
	}

	// LST* lst = new LST();
	// lst->push(new Task(PeriodicTask, 0, 3, 1, 10));
	// lst->push(new Task(PeriodicTask, 0, 4, 1, 10));
	// lst->push(new Task(PeriodicTask, 0, 5, 1, 10));
	// lst->push(new Task(PeriodicTask, 0, 6, 1, 10));
	// lst->push(new Task(PeriodicTask, 0, 7, 1, 10));
	// lst->push(new Task(PeriodicTask, 0, 8, 1, 10));
	// lst->push(new Task(PeriodicTask, 0, 9, 1, 10));
	// while(!lst->empty()){
	// 	usleep(200000);
	// 	lst->increaseTime_1();
	// }


	// RMS* rms = new RMS();
	// rms->push(new Task(1, 0, 10, 10, 10));
	// rms->push(new Task(2, 0, 10, 3, 0));
	// // rms->push(new Task(3, 5, 6, 2, 0));
	// // rms->push(new Task(5, 2, 2, 4, 4));

	// while(!rms->empty()){
	// 	usleep(200000);
	// 	rms->increaseTime_1();
	// 	// rms->getU();
	// }



}
