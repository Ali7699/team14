#pragma once
#include "Scheduler.h"


Scheduler::Scheduler() {
	int timeStep = 0;
	int totalPatients = 0;
	int finishedPatients = 0;
	int X = 0;
	char rndmchar = 'E';
}


Scheduler::~Scheduler() {
	ALL_Patients.~LinkedQueue();
	Early_Patients.~EarlyPList();
	Late_Patients.~priQueue();
	U_Waiting.~EU_Waitlist();   
	E_Waiting.~EU_Waitlist();   
	X_Waiting.~X_Waitlist();
	E_Devices.~LinkedQueue();
	U_Devices.~LinkedQueue();
	X_Rooms.~LinkedQueue();
	In_Treatment.~priQueue();
	Finished_patients.~ArrayStack();

}


void Scheduler::loadInputFile() {

	//input file must always be called input
	//Place it in Input folder in project folder so its processed
	std::ifstream inputfile("Input/input");

	if (!inputfile) {
		//If we cant read, call UI, tell simulation failed, and update output correctly. 
		//this should be handeled by the loader directly, never simulation function
	
	}

	std::string line;

	//line 1: allocate resources

	std::getline(inputfile, line);

















}


void Scheduler::Simulation() {

}


void Scheduler::randomWaiting() {

}



bool Scheduler::reschedule() {
	bool x = Early_Patients.Reschedule();
	return x;

}

bool Scheduler::cancel() {
	Patient* temp;
	bool x = X_Waiting.cancel(temp);
	if (x == true) Finished_patients.push(temp);

	return x;
}



bool Scheduler::departAll() {
    return false;
}

bool Scheduler::departEarly(char destination) {
    return false;
}

bool Scheduler::departLate(char destination) {
    return false;
}

bool Scheduler::departU_Waiting() {
    return false;
}

bool Scheduler::departE_Waiting() {
    return false;
}

bool Scheduler::departX_Waiting() {
    return false;
}

bool Scheduler::departIn_Treatment(char destination) {
    return false;
}