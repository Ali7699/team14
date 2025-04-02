#pragma once
#include "LinkedQueue.h"
#include "Patient.h"
#include "EarlyPList.h"
#include "priQueue.h"
#include "EU_Waitlist.h"
#include "X_Waitlist.h"
#include "Resource.h"
#include "ArrayStack.h"

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

public:
	/*
	Function notation

	Phase 1: This Function is only implemented for phase 1
	Phase 2: ""
	Polymorphic: this function is implemented in both phases, but will have a diffrent implemnation in phase 2
	Universal: This function stays the same in both phases
	
	
	*/





	//universal: constructor
	Scheduler();
	//destructor
	~Scheduler();

	//1.Intializer:Universal
	void loadInputFile();


	//2.Main function (Contains simulation while loop, Calls all helper functions) : Polymorphic 
	void Simulation();
	

	//3.List movers

	//moves from All :Universal(always dependant on PT and VT)
	bool moveAll(); 




};
