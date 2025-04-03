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

	//other data members 
	
	int timeStep;
	int totalPatients;
	int finishedPatients;

	int Pcancel;
	int Presc;

	//random variable X
	int X;

	//current random Waiting List, can be E, U, or X
	//it is phase 1 exclusive as is Randomwaiting function
	char rndmchar;

public:
	// constructor
	Scheduler();
	//destructor
	~Scheduler();

	// Loads Input file, intializes all data members
	void loadInputFile();


	//Main function (Contains simulation while loop, Calls all other functions) 
	void Simulation();
	
	//phase 1 function: updates rndmchar
	void randomWaiting();
	
	
	
	
	bool reschedule();

	bool cancel();
	
	
	//3.List departers. these move from a specific list to some location
	//in phase 1, destination is based on random input
	//in phase 2, destination is Based on timestep, resource availability, and Treatment list

	//moves from All, same for phase 1 and 2 (always dependant on PT and VT)
	bool departAll(); 


	bool departEarly(char destination);

	bool departLate(char destination);

	bool departU_Waiting();

	bool departE_Waiting();

	bool departX_Waiting();

	bool departIn_Treatment(char destination);

};
