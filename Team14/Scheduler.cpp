#pragma once
#include "Scheduler.h"
#include "UI.h"

Scheduler::Scheduler()
	: timeStep(0), totalPatients(0), finishedPatients(0),Pcancel(0),Presc(0) {
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
		departAll();
		departEarly();
		departLate();
		reschedule(X);
		cancel(X);
		departE_Waiting();
		departU_Waiting();
		departX_Waiting();
		departIn_Treatment();
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

	//We now sort latencys
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


	

bool Scheduler::reschedule(int x) {
	
	bool success = false;
	if (Presc > x) {
		success=Early_Patients.Reschedule();
	}
	return success;

}

bool Scheduler::cancel(int x) {
	Patient* temp;

	if (Pcancel > x) {
		bool success=X_Waiting.cancel(temp);
		if (success) {
			Finished_patients.push(temp);
			return true;
		}
		
	}
	return false;
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

			char Next=Temp->getNextTreatment()->getType();
			 
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

void Scheduler::departLate() {
	while (!Late_Patients.isEmpty()) {
		Patient* Temp;
		int PT;
		Late_Patients.peek(Temp, PT);

		//if PT is now, we deque 
		// defensive programming: if timestep is bigger we still move them (in case we missed patient from last timestep)
		//else break and return
		if (PT <= timeStep) {
			Early_Patients.dequeue(Temp, PT);
			organizeTreatmentList(Temp);

			char Next = Temp->getNextTreatment()->getType();

			if (Next == 'E') {
				E_Waiting.insertSorted(Temp,false);
			}
			else if (Next == 'U') {
				U_Waiting.insertSorted(Temp, false);
			}
			else if (Next == 'X')
				X_Waiting.insertSorted(Temp, false);
		}
		else {
			//end loop if PT is not now
			break;
		}
	}
}

void Scheduler::departE_Waiting() {
	
	//if  E resources isnt empty andd there is a patient waiting
	while (!E_Devices.isEmpty()&&!E_Waiting.isEmpty()) {

		Resource* Edevice;
		Patient* Temp;
		
		

		E_Devices.dequeue(Edevice);
		E_Waiting.dequeue(Temp);
		
		//update total waiting time
		//TW=TW+WIS+timestep
		Temp->setTW(Temp->getTW() + Temp->getWIS() + timeStep);

		Temp->getNextTreatment()->setResource(Edevice);
		Temp->getNextTreatment()->setST(timeStep);



		int finishTime = Temp->getNextTreatment()->getDuration() + timeStep;
		//intreatment is a min que by finish time of patients
		In_Treatment.enqueue(Temp,finishTime);


	}
}

void Scheduler::departU_Waiting() {
	
	while (!U_Devices.isEmpty() && !U_Waiting.isEmpty()) {

		Resource* Udevice;
		Patient* Temp;



		E_Devices.dequeue(Udevice);
		E_Waiting.dequeue(Temp);

		//update total waiting time
		//TW=TW+WIS+timestep
		Temp->setTW(Temp->getTW() + Temp->getWIS() + timeStep);

		Temp->getNextTreatment()->setResource(Udevice);
		Temp->getNextTreatment()->setST(timeStep);



		int finishTime = Temp->getNextTreatment()->getDuration() + timeStep;
		//intreatment is a min que by finish time of patients
		In_Treatment.enqueue(Temp, finishTime);


	}

}

void Scheduler::departX_Waiting() {
	
	while (!X_Rooms.isEmpty() && !X_Waiting.isEmpty()) {

		
		Patient* Temp;
		Resource* Xroom;
		X_Rooms.peek(Xroom);
		E_Waiting.dequeue(Temp);
		
		//safty check is full
		if (Xroom->isFull()) {
			X_Rooms.dequeue(Xroom);
			//we need to reveify Xrooms hasnt become empty as a consequnce of safety check
			continue;
		}
		//update total waiting time
		//TW=TW+WIS+timestep
		Temp->setTW(Temp->getTW() + Temp->getWIS() + timeStep);
		Temp->getNextTreatment()->setST(timeStep);

		
		



		Temp->getNextTreatment()->setResource(Xroom);
		Xroom->setPatientCount(Xroom->getPatientCount()+1);
		//Check again if the room is now full
		if (Xroom->isFull()) {
			X_Rooms.dequeue(Xroom);
		}

		int finishTime = Temp->getNextTreatment()->getDuration() + timeStep;
		//intreatment is a min que by finish time of patients
		In_Treatment.enqueue(Temp, finishTime);

	}
}

void Scheduler::departIn_Treatment() {
	while (!In_Treatment.isEmpty()) {
		Patient* Temp;
		Treatment* FinishedTreatment;


		int FinishTime;
		In_Treatment.peek(Temp, FinishTime);
		
		if (FinishTime >= timeStep) {
			return;
		}

		//update treatment time
		//TT=TT+ (timeStep-ST)  (explanation: We add the time we spent in this treatment isntant to the total treatment time)
		Temp->setTT(Temp->getTT() + (timeStep - Temp->getNextTreatment()->getST()));


		//now we return the resource we were occupying

		if (Temp->getNextTreatment()->getType() == 'X') {
			
			//if its an X, remeber it might still be in Xrooms que
			//the 
			if (Temp->getNextTreatment()->getResource()->isFull()) {
				// we verified its full and hence floating

				X_Rooms.enqueue(Temp->getNextTreatment()->getResource());
			}
			//in all cases, now decrease capacity, delete treatment instance
			Temp->getNextTreatment()->getResource()->setPatientCount(Temp->getNextTreatment()->getResource()->getPatientCount() - 1);
			LinkedQueue<Treatment*>& tempQue = Temp->getTreatmentlist();
			
			tempQue.dequeue(FinishedTreatment);
		}
		
		
		else if (Temp->getNextTreatment()->getType() == 'E') {
			
			E_Devices.enqueue(Temp->getNextTreatment()->getResource());

			LinkedQueue<Treatment*>& tempQue = Temp->getTreatmentlist();

			tempQue.dequeue(FinishedTreatment);
		}
		
		
		else if (Temp->getNextTreatment()->getType() == 'U'){
			U_Devices.enqueue(Temp->getNextTreatment()->getResource());

			LinkedQueue<Treatment*>& tempQue = Temp->getTreatmentlist();

			tempQue.dequeue(FinishedTreatment);
		}

		//now reset the treatment que for RP

		organizeTreatmentList(Temp);
		
		Treatment* Next = Temp->getNextTreatment();
		
		//Case 1: que is empty and were done
		if (Temp->getTreatmentlist().isEmpty()) {
			In_Treatment.dequeue(Temp, FinishTime);
			Finished_patients.push(Temp);
		}
		else if (Next->getType() == 'E') {
			In_Treatment.dequeue(Temp, FinishTime);
			E_Waiting.insertSorted(Temp, true);
		}
		else if (Next->getType() == 'U') {
			In_Treatment.dequeue(Temp, FinishTime);
			U_Waiting.insertSorted(Temp, true);
		}
		else if (Next->getType() == 'X') {
			In_Treatment.dequeue(Temp, FinishTime);
			X_Waiting.insertSorted(Temp, true);
		}



	}
	
}

void Scheduler::updateNumbers() {
	timeStep++;
	finishedPatients = Finished_patients.count();

}