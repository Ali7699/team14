#pragma once
#include "Scheduler.h"
#include "UI.h"
#include "Treatment.h"
#include <iomanip>

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


bool Scheduler::loadInputFile(int input) {

	//input file must always be called input
	//Place it in Input folder in project folder so its processed
	
	std::string filename = "Input/input" + std::to_string(input) + ".txt";

	std::ifstream inputfile(filename);

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

void Scheduler::output()
{

	std::ofstream outFile("Output/output.txt");
	outFile << "PID  PTvpe   PT   VT   FT   WT   TT   Cancel   Resc\n";
	
	//these store info for next print
	//S is shorthand for sigma or summation
	int Ncount=0, Rcount = 0, STW = 0, STT = 0, SNTW = 0, SNTT = 0, SRTW = 0, SRTT = 0, Scancel = 0, Sresc = 0, EarlyCount = 0, LateCount = 0, Spenalty = 0;
	
	//print patients, and recover info
	while (!Finished_patients.isEmpty()) {
		Patient* temp;

		Finished_patients.pop(temp);
		bool type = temp->getType();
		char TYPECHAR='N';
		
		if (type == 0) { 
			Ncount++; 
			TYPECHAR = 'N';
			SNTW += temp->getTW();
			SNTT += temp->getTT();
		}
		else if (type == 1){
			Rcount++;
			TYPECHAR = 'R';
			SRTW += temp->getTW();
			SRTT += temp->getTT();
		}

		STW += temp->getTW();
		STT += temp->getTT();
		
		char cancelChar = 'F';
		char rescChar = 'F';

		if (temp->getCancel()) { 
			Scancel++;
			cancelChar = 'T';
		}
		if (temp->getResc()) {
			Sresc++;
			rescChar = 'T';
		}

		int status = temp->statusForOut();
		
		if (status == 1) EarlyCount++;
		else if (status == 2) { 
		
			LateCount++;
			Spenalty += temp->getPenalty();
		}


		//We use libary for alignment for clear reading
		//libary is necessary as numbers could be 2 digit or 1 digit
		outFile << std::left
			<< std::setw(5) << ("P" + std::to_string(temp->getId()))
			<< std::setw(8) << TYPECHAR         
			<< std::setw(5) << temp->getoriginalPT() 
			<< std::setw(5) << temp->getVT()     
			<< std::setw(5) << temp->getFT()     
			<< std::setw(5) << temp->getTW()    
			<< std::setw(5) << temp->getTT()    
			<< std::setw(9) << cancelChar        
			<< std::setw(5) << rescChar          
			<< "\n";





	}

	//c is shorthand for count
	int c = finishedPatients;

	outFile << "Total number of timesteps = " << timeStep << "\n";
	outFile << "Total number of all, N, and R patients = " << c << ", " << Ncount << ", " << Rcount << "\n";
	outFile << "Average total waiting time for all, N, and R patients = " << (STW) / c << ", " << SNTW / c << ", " << SRTW / c << "\n";
	outFile << "Average total treatment time for all, N, and R patients = " << STT / c << ", " << SNTT / c << ", " << SRTT / c << "\n";
	outFile << "Percentage of patients of an accepted cancellation (%) = " << 100 * Scancel / c << "%\n";
	outFile << "Percentage of patients of an accepted rescheduling (%) = " << 100 * Sresc / c << "%\n";
	outFile << "Percentage of early patients (%) = "<<100*EarlyCount/c<< "%\n";
	outFile << "Percentage of late patients (%) = " << 100 * LateCount / c << "%\n";
	int Avgpenalty=0;
	if (LateCount != 0) {
		 Avgpenalty = Spenalty / LateCount;
	}
	else {
		Avgpenalty = 0;
	}
	outFile << "Average late penalty = " << Avgpenalty << " timestep(s)\n";

}



void Scheduler::Simulation(int input,bool silent) {
	loadInputFile(input);
	 //print the first time
	while (finishedPatients != totalPatients) {
		int X = rand() % 101;

		//print all
		if (ui&&!silent) {
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






void Scheduler::AddtoE(Patient*input,int CASE)
{
	//only 3 cases: going to waitlist from early, going to waitlist from late, or returning from intreatment
	//cases: 1 is early, 2 is late, 3 is from 
	switch (CASE) {
	case 1:
		E_Waiting.enqueue(input);
		return;
	case 2:
		E_Waiting.insertSorted(input, false);
		return;
	case 3:
		E_Waiting.insertSorted(input, true);
		return;
	}

}

void Scheduler::AddtoU(Patient* input, int CASE)
{
	//only 3 cases: going to waitlist from early, going to waitlist from late, or returning from intreatment
//cases: 1 is early, 2 is late, 3 is from 
	switch (CASE) {
	case 1:
		U_Waiting.enqueue(input);
		return;
	case 2:
		U_Waiting.insertSorted(input, false);
		return;
	case 3:
		U_Waiting.insertSorted(input, true);
		return;
	}
}

void Scheduler::AddtoX(Patient* input, int CASE)
{
	//only 3 cases: going to waitlist from early, going to waitlist from late, or returning from intreatment
//cases: 1 is early, 2 is late, 3 is from 
	switch (CASE) {
	case 1:
		X_Waiting.enqueue(input);
		return;
	case 2:
		X_Waiting.insertSorted(input, false);
		return;
	case 3:
		X_Waiting.insertSorted(input, true);
		return;
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

		bool success=X_Waiting.cancel(temp,timeStep);

		if (success) {
			Finished_patients.push(temp);
			return true;
		}
		
	}
	return false;
}


void Scheduler::departAll() {
	
	while (!ALL_Patients.isEmpty()) {
		Patient* temp;
		ALL_Patients.peek(temp);

		int arrivalTime = temp->getVT();
		
		if (arrivalTime > timeStep) {
			break;
			}
		
		ALL_Patients.dequeue(temp);
		temp->updateStatus(timeStep);
		Patient::PatientStatus stat = temp->getStatus();
		if (temp->getStatus() == Patient::LATE) {

		//Enque Late patient with priority= ORIGINAL PT +penalty
			int penalty = temp->getPenalty();
			int pri = temp->getPT() + penalty;


		//update appointment to be VT+penalty
			int NewPT = temp->getVT() + penalty;

			temp->setPT(NewPT);

			Late_Patients.enqueue(temp,pri);
			
		}
		else if (temp->getStatus() == Patient::ERLY) {
			Early_Patients.enqueue(temp, temp->getPT());
		}
		
	}
}

void Scheduler::departEarly() {
	
	//THIS IS CASE 1 FOR MOVING TO A WAITLIST

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

			Treatment* Next = Temp->getNextTreatment();
			 
			//This is is CASE 1, hence input with int 1
			Next->moveToWait(this, Temp, 1);
		}
		else {
			//end loop if PT is not now
			break;
		}


	}

}

void Scheduler::departLate() {
	
	//THIS IS STRICTLY CASE 2 : MOVING FROM LATE TO WAITLIST
	while (!Late_Patients.isEmpty()) {
		Patient* Temp;
		int garbage;

		//Remeber, Prio of late patients is NOT PT, its is original PT+penalty
		//New PT is VT +penalty
		Late_Patients.peek(Temp, garbage);

		int PT=Temp->getPT();

		//if PT is now, we deque 
		//else break and return
		if (PT <= timeStep) {
			Late_Patients.dequeue(Temp, PT);
			organizeTreatmentList(Temp);

			Treatment* Next = Temp->getNextTreatment();

			//Move to wait with case 2 
			Next->moveToWait(this, Temp, 2);

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



		U_Devices.dequeue(Udevice);
		U_Waiting.dequeue(Temp);

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
		X_Waiting.dequeue(Temp);
		
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


		// Exit the function if no treatments finish at current timestep
		if (FinishTime > timeStep) {
			return;
		}

		// Update treatment time
		//explanation: 
		


		// Return the resource based on treatment type
		char treatmentType = Temp->getNextTreatment()->getType();
		LinkedQueue<Treatment*>& tempQue = Temp->getTreatmentlist();

		// Handle resource return based on treatment type
		switch (treatmentType) {
		case 'X':
			if (Temp->getNextTreatment()->getResource()->isFull()) {
				X_Rooms.enqueue(Temp->getNextTreatment()->getResource());
			}
			Temp->getNextTreatment()->getResource()->setPatientCount(
				Temp->getNextTreatment()->getResource()->getPatientCount() - 1);
			tempQue.dequeue(FinishedTreatment);
			break;

		case 'E':
			E_Devices.enqueue(Temp->getNextTreatment()->getResource());
			tempQue.dequeue(FinishedTreatment);
			break;

		case 'U':
			U_Devices.enqueue(Temp->getNextTreatment()->getResource());
			tempQue.dequeue(FinishedTreatment);
			break;
		}



		// Remove patient from treatment queue
		In_Treatment.dequeue(Temp, FinishTime);

		// Reorganize treatment list
		organizeTreatmentList(Temp);

		// Check if the patient has more treatments
		if (Temp->getTreatmentlist().isEmpty()) {
			// Patient has completed all treatments
			Finished_patients.push(Temp);
		}
		else {
			// Get the next treatment type and move to appropriate waiting queue
			Treatment* Next = Temp->getNextTreatment();
			if (!Next) {
				// Safety check - if getNextTreatment returns nullptr, 
				// this could be why patients get stuck
				Finished_patients.push(Temp);
			}
			else {
				//this is strictly case 3 : moving frtom in treatment to waitlist
				Next->moveToWait(this, Temp, 3);
			}

		}
	}
}

void Scheduler::updateNumbers() {
	timeStep++;
	finishedPatients = Finished_patients.count();

}