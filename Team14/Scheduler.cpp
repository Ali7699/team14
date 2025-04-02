#pragma once
#include "Scheduler.h"


Scheduler::Scheduler() {
	int timeStep = 0;
	int totalPatients = 0;
	int finishedPatients = 0;
	int X = 0;
	char rndmchar = 'E';
}

// Destructor
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

// Loads Input file, intializes all data members
void Scheduler::loadInputFile() {

}

// Main function (Contains simulation while loop, Calls all helper functions) 
void Scheduler::Simulation() {

}

// phase 1 function: updates rndmchar
void Scheduler::randomWaiting() {

}



//  List departers. these move from a specific list to some location

// moves from All :Universal(always dependant on PT and VT)
bool Scheduler::departAll() {
    return false;
}

bool Scheduler::reschedule() {
    return false;
}

bool Scheduler::cancel() {
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