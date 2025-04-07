#pragma once
#include "Scheduler.h"
#include "UI.h"

Scheduler::Scheduler()
	: timeStep(0), totalPatients(0), finishedPatients(0), rndmchar('E'),Pcancel(0),Presc(0) {
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
	while (finishedPatients != totalPatients) {
		updateNumbers();
		randomWaiting();
		departAll();
		
		int X = rand() % 101;

		if (X < 10) {
			// move next patient from Early to RandomWaiting
			departEarly(rndmchar);
		}
		else if (X < 20) {
			// move next patient from Late to RandomWaiting using PT + penalty time
			departLate(rndmchar);
		}
		else if (X < 40) {
			// move 2 next patients from RandomWaiting to In-treatment
			switch (rndmchar) {
			case 'U':
				departU_Waiting();
				departU_Waiting();
			case 'E':
				departE_Waiting();
				departE_Waiting();
			case 'X':
				departX_Waiting();
				departX_Waiting();

			}
		}
		else if (X < 50) {
			// move next patient from In-treatment to RandomWaiting
			departIn_Treatment(rndmchar);
		}
		else if (X < 60) {
			// move next patient from In-treatment to Finish
			departIn_Treatment('F');
		}
		else if (X < 70) {
			// move random patient from X-Waiting to Finish (cancel process)
			cancel();
		}
		else if (X < 80) {
			// choose random patient from Early to appropriate list (accepted reschedule)
			reschedule();
		}
		
		if (ui) {
			ui->printAll(
				timeStep,                // timestep
				X,                       // X
				ALL_Patients,            // alllist
				U_Waiting,               // U_Waitlist
				E_Waiting,               // E_Waitlist
				X_Waiting,               // X_Waitlist
				Early_Patients,          // EarlyList
				Late_Patients,           // LateList
				E_Devices,               // AvailE
				U_Devices,               // AvailU
				X_Rooms,                 // AvailX
				In_Treatment,            // InTreatment
				Finished_patients        // Finished
			);
		}
	}
}


void Scheduler::randomWaiting() {
	int randomNum = rand() % 101;

	// Update rndmchar based on the random number
	if (randomNum < 33) {
		rndmchar = 'E';
	}
	else if (randomNum < 66) {
		rndmchar = 'U';
	}
	else {
		rndmchar = 'X';
	}
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
	if (Early_Patients.isEmpty())return;
	Patient* temp;
	int garbage;
	Early_Patients.dequeue(temp,garbage);
	switch (destination) {
	case 'E':
		E_Waiting.enqueue(temp);
	case 'U':
		U_Waiting.enqueue(temp);
	case 'X':
		X_Waiting.enqueue(temp);
	}

}

void Scheduler::departLate(char destination) {
	if (Late_Patients.isEmpty())return;
	Patient* temp;
	int garbage;
	Late_Patients.dequeue(temp, garbage);
	switch (destination) {
	case 'E':
		E_Waiting.insertSorted(temp,false);
	case 'U':
		U_Waiting.insertSorted(temp, false);
	case 'X':
		X_Waiting.insertSorted(temp, false);
	}
}

void Scheduler::departU_Waiting() {
	if (U_Waiting.isEmpty())return;
	Patient* temp;
	U_Waiting.dequeue(temp);
	In_Treatment.enqueue(temp,1); //finish time logic not implmented, since we move randomly. all patients have the same finish time.
}

void Scheduler::departE_Waiting() {
	if (E_Waiting.isEmpty())return;
	Patient* temp;
	E_Waiting.dequeue(temp);
	In_Treatment.enqueue(temp, 1); //finish time logic not implmented, since we move randomly. all patients have the same finish time.
}

void Scheduler::departX_Waiting() {
	if (X_Waiting.isEmpty())return;
	Patient* temp;
	X_Waiting.dequeue(temp);
	In_Treatment.enqueue(temp, 1); //finish time logic not implmented, since we move randomly. all patients have the same finish time.
}

void Scheduler::departIn_Treatment(char destination) {
	//can go to randomwaiting or finished
	if (In_Treatment.isEmpty())return;
	Patient* temp;
	int garbage;
	In_Treatment.dequeue(temp, garbage);
	switch (destination) {
		case 'F':
			Finished_patients.push(temp);
		case 'E':
			E_Waiting.insertSorted(temp);
		case 'U':
			U_Waiting.insertSorted(temp);
		case 'X':
			X_Waiting.insertSorted(temp);
		}
}

void Scheduler::updateNumbers() {
	timeStep++;
	finishedPatients = Finished_patients.count();
}