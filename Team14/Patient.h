#pragma once
#include "treatment.h"
#include "LinkedQueue.h"


class Patient {
public:
	enum PatientStatus {
		IDLE,	// (VT > current timestep)
		ERLY,	// (VT < PT)
		LATE,	//  PT (VT > PT)
		WAIT,	// in waitlist
		SERV,   // (in treatment)
		FNSH	//finished ALL treatments
	};
private:
	//patient ID
	int PID;
	//0 is Normal, 1 is Recovering
	bool Type;
	// aPpointment Time
	int PT;	
	//	arriVal Time
	int VT;	
	
	int penalty;

	int TW;	
	// total time patient is in treatment
	int TT;		
	//pointer to resource patient may be using
	Resource* CurrentTreatment;
	
	
	PatientStatus Status;

	LinkedQueue<Treatment*> Treatmentlist;

public:
	Patient(int id, bool type, int pt, int vt)
		: PID(id), Type(type), PT(pt), VT(vt), Status(IDLE),CurrentTreatment(nullptr) {
		penalty = ((VT - PT) / 2);
	}  

	
	//getters
	
	int getId()const{return PID;}
	bool getType() const {return Type;}
	//gets appointment time
	int getPT()const {return PT;}
	//gets arrival time
	int getVT()const {return VT;}
	
	int getPenalty()const { return penalty; }
	
	//this takes a refrence, not a copy and must be used carefully
	LinkedQueue<Treatment*>& getTreatmentlist() {
		return Treatmentlist;
	}


	PatientStatus getStatus()const {return Status;}
	
	Resource* getCurrentTreatment()const { return CurrentTreatment; }

	
	 //setters
	void setPID(int id) { PID = id; }
	// 0 = normal NP, 1 = recovering RP
	void setType(bool type) { Type = type; }
	//set appointment time
	void setPT(int pt) { PT = pt; }
	void setVT(int vt) { VT = vt; }
	void setTreatmentList(const LinkedQueue<Treatment*>&list) {
			Treatmentlist = list;
	}
	void setCurrentTreatment( Resource* input) { CurrentTreatment = input; }


	//functions
	void updateStatus(int currentTime) {
		if (currentTime < VT) {
			Status = IDLE;
		}
		else if (VT < PT) {
			Status = ERLY;
		}
		else if (VT > PT) {
			Status = LATE;
		}
	}

	void addTreatment(Treatment* t) {
	//we always enque normally, RP logic handeled later
		Treatmentlist.enqueue(t);
	}
	
	bool removeTreatment(Treatment*&currenttreatment) {
		return Treatmentlist.dequeue(currenttreatment); 
		
	}

	bool isEmpty() {
		return Treatmentlist.isEmpty();
	}



};