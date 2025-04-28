#pragma once
#include "LinkedQueue.h"
#include "Patient.h"
#include "EarlyPList.h"
#include "priQueue.h"
#include "EU_Waitlist.h"
#include "X_Waitlist.h"
#include "Resource.h"
#include "ArrayStack.h"

#include "Treatment.h"


#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>




class UI;
//forward declaration alleviates circuilar dependancy and allows ui Pointer.

class Scheduler {
private:
	LinkedQueue<Patient*> ALL_Patients;
	EarlyPList Early_Patients;
	priQueue<Patient*> Late_Patients;
	
	EU_Waitlist U_Waiting;
	EU_Waitlist E_Waiting;
	X_Waitlist X_Waiting;

	LinkedQueue<Resource*> E_Devices;
	LinkedQueue<Resource*> U_Devices;
	LinkedQueue<Resource*> X_Rooms;

	priQueue<Patient*> In_Treatment;

	ArrayStack<Patient*> Finished_patients;

	UI* ui;
	
	//other data members 
	
	int timeStep;
	int totalPatients;
	int finishedPatients;

	int Pcancel;
	int Presc;

	//random variable X

	//current random Waiting List, can be E, U, or X
	//it is phase 1 exclusive as is Randomwaiting function
	char rndmchar;

public:
	// constructor
	Scheduler();
	//destructor
	~Scheduler();

	void setUI(UI* uii) {
		ui = uii;
	}
	
	
	// Loads Input file, intializes all data members
	bool loadInputFile();


	//Main function (Contains simulation while loop, Calls all other functions) this function diffres m from phase 1 to 2, 
	//phase 1 we just do a switch case for data memeber X
	void Simulation();
	
	//phase 1 function: updates rndmchar every time step. we use rndchar to detrmine destinatioons
	void randomWaiting();
	
	void RandomWaitingPremium();
	
	//this helper goes through treatment list of any patient, identifies if Recovering, and organizes treatment list according to current latency
	void organizeTreatmentList(Patient*);
	
	bool reschedule();

	bool cancel();
	
	
	//3.List departers. these move from a specific list to some location
	//in phase 1, destination is based on random input
	//in phase 2, destination is Based on timestep, resource availability, and Treatment list

	//moves from All, same for phase 1 and 2 (always dependant on PT and VT)
	void departAll();


	void departEarly();

	void departLate();

	void departU_Waiting();

	void departE_Waiting();

	void departX_Waiting(char destination);

	void departIn_Treatment(char destination);

	//helpers
	void updateNumbers();
};
