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

	const int originalPT;

	//	arriVal Time
	int VT;	
	
	int penalty;
	
	//this is the time a patient started waiting again after finishing a treatment
	//Waiting Instance start
	int WIS; //this is intially arrival time, and we update it when a patient finishes a treatment

	//total time patient is in waiting (any time not inTreatmentList)
	int TW;	
	
	// total time patient is in treatment
	int TT;		
	//did a patient cancel or not
	bool cancelCheck;
	//did a patient reschedule or not
	bool rescCheck;


	PatientStatus Status;

	LinkedQueue<Treatment*> Treatmentlist;

public:
	Patient(int id, bool type, int pt, int vt)
		: PID(id), Type(type), PT(pt), VT(vt), Status(IDLE),WIS(vt),TW(0),TT(0),cancelCheck(0),rescCheck(0),originalPT(pt){
		
		if (VT > PT) {
			penalty = ((VT - PT) / 2);
		}
	}  

	
	//getters
	
	int getId()const{return PID;}
	
	//0 is normal, 1 is Recovering
	bool getType() const {return Type;}
	//gets appointment time
	int getPT()const {return PT;}
	//gets arrival time
	int getVT()const {return VT;}
	
	int getPenalty()const { return penalty; }

	int getTW()const { return TW; }

	int getWIS()const { return WIS; }

	int getTT() { 
		return TT;
	}

	bool getCancel()const { return cancelCheck; }

	bool getResc()const { return rescCheck; }

	int getFT() { return VT + TW + TT; }

	int getoriginalPT() { return originalPT; }

	//Peeks the first element of the treatment list
	Treatment* getNextTreatment() {
		Treatment* temp;
		Treatmentlist.peek(temp);
		return temp;
	}
	
	//this takes a refrence, not a copy and must be used carefully
	LinkedQueue<Treatment*>& getTreatmentlist() {
		return Treatmentlist;
	}


	PatientStatus getStatus()const {return Status;}
	


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

	void setTW(int tw) { TW = tw; }

	void setTT(int tt) { TT = tt; }

	void setWIS(int wis) { WIS = wis; }




	//functions
	void updateStatus(int currentTime) {
		if (currentTime > VT) {
			Status = IDLE;
		}
		else if (VT > PT) {
			Status = LATE;
		}
		else if (VT <= PT) {
			Status = ERLY;
		}
	}
	int statusForOut() {
		if (VT < originalPT) {
			//early
			return 1;
		}
		else if (VT > originalPT) {
			return 2;
		}
		return 0;

	}

	void setCancel(bool input) {
		cancelCheck = input;
	}

	void setResc(bool input) {
		rescCheck = input;
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