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
	std::ifstream inputfile("Input/input.txt");

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
	loadInputFile();
	 //print the first time
	while (finishedPatients != totalPatients) {
		int X = rand() % 101;
		
		//print all
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

		updateNumbers();
		randomWaiting();
		departAll();
		
		

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
			//the issue is, we dont know which is empty, and which is full, so we cant rely on random waiting.
			//hence we made RandomWaitingPremium
			
			for (int i = 0; i < 2; i++) {
				RandomWaitingPremium();
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

void Scheduler::RandomWaitingPremium() {
	//terniary is way more efficent here
	int U = (U_Waiting.count() > 0) ? 1 : 0;
	int E = (E_Waiting.count() > 0) ? 1 : 0;
	int X = (X_Waiting.count() > 0) ? 1 : 0;

	int PossibleOptions = U + E + X;

	if (PossibleOptions == 0) {
		return; 
	}

	int choice = rand() % PossibleOptions;

	
	if (U && choice == 0) {
		departU_Waiting();
	}
	else if (E && (choice == 1 || (U == 0 && choice == 0))) {
		departE_Waiting();
	}
	else if (X && (choice == 2 || (U + E == 0 && choice == 0))) {
		departX_Waiting('T'); //always goes to treatment list when called from here
	}
}


void Scheduler::organizeTreatmentList(Patient*input)
{
	//if normal, ignore
	if (input->getType() == 0)
		return;

	LinkedQueue<Treatment*>& Temp = input->getTreatmentlist();
	int count = Temp.count();

	if (Temp.isEmpty()) {
		return;
	}
	

	//now get latencys , we store in arr for easy sort
	typedef struct {
		int latency;
		char type;
	} Latency;

	Latency arr[3];

	//We no sort latencys
	{
		arr[0].type = 'E';
		arr[0].latency = E_Waiting.calcTreatmentLatency('E');

		arr[1].type = 'U';
		arr[1].latency = U_Waiting.calcTreatmentLatency('U');

		arr[2].type = 'X';
		arr[2].latency = X_Waiting.calcTreatmentLatency('X');

		//hardcoded sort, more efficent since we only ever have 3 elements

		if (arr[0].latency > arr[1].latency) {
			Latency temp = arr[0];
			arr[0] = arr[1];
			arr[1] = temp;
		}
		if (arr[1].latency > arr[2].latency) {
			Latency temp = arr[1];
			arr[1] = arr[2];
			arr[2] = temp;
		}
		if (arr[0].latency > arr[1].latency) {
			Latency temp = arr[0];
			arr[0] = arr[1];
			arr[1] = temp;
		}

	}
	
	Treatment* treatments[3] = { nullptr, nullptr, nullptr };
	for (int i = 0; i < count; ++i) {
		 Temp.dequeue(treatments[i]); // Remove from queue and store in arr for easy access
	}

	// Reorder based on latency order
	//arr[0] has lowest latency, so we look for treatments from that type then 1 then 2. 
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < count; ++j) {
			if (treatments[j] && treatments[j]->getType() == arr[i].type) {
				Temp.enqueue(treatments[j]); 
				treatments[j] = nullptr;    
				break;                      
			}
		}
	}
}


	

bool Scheduler::reschedule() {
	bool x = Early_Patients.Reschedule();
	return x;

}

bool Scheduler::cancel() {
	Patient* temp;
	//we CANNOT use cancel function as it depends on treatment logic. What if a patient has 3 treatments and theyre in X? they would never cancel. 
	//hence we must do it manually, although cancel works fine.
	
	departX_Waiting('F');

	
	return true;
}


void Scheduler::departAll() {
	if (ALL_Patients.isEmpty())return ;
	
	
	while (!ALL_Patients.isEmpty()) {
		Patient* temp;
		ALL_Patients.peek(temp);
		
		if (temp->getVT() > timeStep) {
			break;
			}
		
		ALL_Patients.dequeue(temp);
		temp->updateStatus(timeStep);
		if (temp->getStatus() == Patient::LATE) {
			Late_Patients.enqueue(temp,temp->getPT()+temp->getPenalty());
		}
		else if (temp->getStatus() == Patient::ERLY) {
			Early_Patients.enqueue(temp, temp->getPT());
		}
		
	}
}

void Scheduler::departEarly() {
	while (!Early_Patients.isEmpty()) {
		Patient* Temp;
		int PT;
		Early_Patients.peek(Temp, PT);
		
		//if PT is now, we deque 
		// defensive programming: if timestep is bigger we still move them
		//else break and return
		if (PT <= timeStep) {
			Early_Patients.dequeue(Temp, PT);
			organizeTreatmentList(Temp);

			char Next=Temp->getNextTreatment();
			 
			if (Next == 'E') {
				E_Waiting.enqueue(Temp);
			}
			else if (Next == 'U') {
				U_Waiting.enqueue(Temp);
			}
			else if (Next == 'X')
				X_Waiting.enqueue(Temp);
		}
		else {
			//end loop if PT is not now
			break;
		}




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
		break;
	case 'U':
		U_Waiting.insertSorted(temp, false);
		break;
	case 'X':
		X_Waiting.insertSorted(temp, false);
		break;
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

void Scheduler::departX_Waiting(char destination) {
	if (X_Waiting.isEmpty())return;
	Patient* temp;
	X_Waiting.dequeue(temp);
	if (destination == 'F') {
		Finished_patients.push(temp);
	}
	else if (destination == 'T') {
		In_Treatment.enqueue(temp, 1); //finish time logic not implmented, since we move randomly. all patients have the same finish time.
	}
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
			break;
		case 'E':
			E_Waiting.insertSorted(temp);
			break;
		case 'U':
			U_Waiting.insertSorted(temp);
			break;
		case 'X':
			X_Waiting.insertSorted(temp);
			break;
		}
}

void Scheduler::updateNumbers() {
	timeStep++;
	finishedPatients = Finished_patients.count();
}