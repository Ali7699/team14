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

	/*FIRST TASK: ALLOCATE RESOURCES*/
		//E devices

	std::getline(inputfile, line); //we are now on line 1
	std::stringstream ss(line);//intialize string stream



	int temporary;

	ss >> temporary; //we now intake first number
	
	for (int i = 0; i < temporary; i++) {
		Resource* x = new Resource('E', i, -1);
		E_Devices.enqueue(x);

	}
		//U devices
	ss >> temporary; //intake second number
	for (int i = 0; i < temporary; i++) {
		Resource* x = new Resource('U', i, -1);
		U_Devices.enqueue(x);

	}
		//X capcitys requires us to shift to 2nd line first before intilaizing for capcitys
	ss >> temporary; //third number

	int Xcount = temporary; //we cant intialize without capacity so we store
	int Xcapacitys[50]; //array to store capacities
	
	std::getline(inputfile, line);//move to second line
	ss.str(line);//reset string stream

	//this loop stores capacities
	for (int i = 0; i < Xcount; i++) {
		ss >> Xcapacitys[i];
	}

	//now intialize
	for (int i = 0; i < Xcount; i++) {
		Resource* x = new Resource('X', i, Xcapacitys[i]);
		X_Rooms.enqueue(x);
	}
	
	/*SECOND TASK: READ PROPABILITIES*/
	std::getline(inputfile, line);//move to 3rd line
	ss.str(line);//reset string stream

	ss >> Pcancel;
	ss >> Presc;

	/*LAST TASK: intialize patient*/
	std::getline(inputfile, line);//move to 4th line
	ss.str(line);
	int patientCount;
	ss >> patientCount;

	totalPatients = patientCount;

	for (int i = 0; i < patientCount; i++) {
		std::getline(inputfile, line); //move to ith line where 5<=i<patientcount
		ss.str(line);

		char TYPE;
		bool type;
		int pt;
		int vt;
		ss >> TYPE;
		if (TYPE == 'N') {
			type = false;
		}
		else if (TYPE == 'R') {

			type = true;
		}
		ss >> pt;
		ss >> vt;

		Patient* x = new Patient(i, type, pt, vt);
		ALL_Patients.enqueue(x);
		int treatmentcount;
		ss >> treatmentcount;

		for (int j = 0; j < treatmentcount; j++) {
			char treatmentType;
			int duration;
			ss >> treatmentType;
			ss >> duration;
			Treatment* y = Treatment::createTreatment(treatmentType, duration);
			x->addTreatment(y);
		}

	}

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