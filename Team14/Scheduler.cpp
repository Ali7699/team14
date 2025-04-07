#pragma once
#include "Scheduler.h"


Scheduler::Scheduler()
	: timeStep(0), totalPatients(0), finishedPatients(0), X(0), rndmchar('E'),Pcancel(0),Presc(0) {
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


bool Scheduler::loadInputFile() {

	//input file must always be called input
	//Place it in Input folder in project folder so its processed
	std::ifstream inputfile("Input/input");

	if (!inputfile) {
		return false;
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
	ss.clear();

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
	ss.clear();

	ss >> Pcancel;
	ss >> Presc;

	/*LAST TASK: intialize patient*/
	std::getline(inputfile, line);//move to 4th line
	ss.str(line);
	ss.clear();

	int patientCount;
	ss >> patientCount;

	totalPatients = patientCount;

	for (int i = 0; i < patientCount; i++) {
		std::getline(inputfile, line); //move to ith line where 5<=i<patientcount
		ss.clear();
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
	return true;
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



void Scheduler::departAll() {
	if (ALL_Patients.isEmpty())return ;
	Patient* temp;
	
	while (!ALL_Patients.isEmpty()) {
		ALL_Patients.peek(temp);
		
		if (temp->getVT() > timeStep) {
			break;
			}
		
		ALL_Patients.dequeue(temp);
		temp->updateStatus(timeStep);
		if (temp->getStatus() == Patient::LATE) {
			Late_Patients.enqueue(temp,temp->getPT());
			return;
		}
		else if (temp->getStatus() == Patient::ERLY) {
			Early_Patients.enqueue(temp, temp->getPT());
			return;
		}
		
	}
}

void Scheduler::departEarly(char destination) {
    return false;
}

void Scheduler::departLate(char destination) {
    return false;
}

void Scheduler::departU_Waiting() {
    return false;
}

void Scheduler::departE_Waiting() {
    return false;
}

void Scheduler::departX_Waiting() {
    return false;
}

void Scheduler::departIn_Treatment(char destination) {
    return false;
}