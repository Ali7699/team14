#pragma once
#include"LinkedQueue.h"
#include "Patient.h"
#include "Treatment.h"
class EU_Waitlist:public LinkedQueue<Patient*> {
public:
	
	//takes treatment type, and returns latency of the list
	int calcTreatmentLatency(char inputtype) {
		Node<Patient*>* current = frontPtr;
		
		//this is what we return
		int treatmentLatency=0;

		while (current != nullptr) {
			Patient* tempPatient = current->getItem();
			
			//temp list is a REFRENCE to treatment list
			LinkedQueue<Treatment*>&tempList = tempPatient->getTreatmentlist();

			//We cannot access templist nodes directly
			//hence we use public features deqque and enque

			Treatment* tempTreatment;
			
			

			int treatmentCount = tempList.count();

			//we MUSt check not empty to avoid exploiting nullptr
			if (!tempList.isEmpty()) {
				for (int i = 0; i < treatmentCount; i++) {
					
					tempList.dequeue(tempTreatment);

					if (tempTreatment->getType() == inputtype) {
						treatmentLatency += tempTreatment->getDuration();

					}
					tempList.enqueue(tempTreatment);

				}
			}
			
			current = current->getNext();
		}
		return treatmentLatency; // if no treatment type found or empty, we simply return intial value 0
	}
	
	//Inserts sorted late patients and patients from treatment lists
	bool insertSorted(Patient* input, bool IsFromTreatment = true) {

		if (input == nullptr) return false;

		//check: are we inserteing a late patient or a patient who finished treatment?
		//NOTE Penalty is only ever applied first go. Late patients who are entring second treatment are NOT late. 
		//hence, Caller must identify if late patient is from first time or not
		int x = input->getPT();
		int y = input->getVT();
		// AppointmentTime integer is whats used to sort in the list
		int AppointmentTime;

		//late AND NOT coming from a treatment
		if (x >= y && !IsFromTreatment) {
			AppointmentTime = x + ((x - y) / 2);
		}
		else {
			AppointmentTime = x;
		}

		//Case A: Que is empty insert right away
		if (isEmpty()) {
			enqueue(input);
			return true;
		}
		//Case B: Input is lowest PT, hence needs insertion at head
		Patient* TempPatient = frontPtr->getItem();
		if (TempPatient->getPT() > AppointmentTime) {

			Node<Patient*>* newNode = new Node<Patient*>(input);
			newNode->setNext(frontPtr);
			frontPtr = newNode;
			return true;
		}


		//Case C: Input belongs somewhere in the middle
		Node<Patient*>* current = frontPtr;
		while (current->getNext() != nullptr) {
			Patient* NextPatient = current->getNext()->getItem();
			Patient* CurrentPatient = current->getItem();
			// Current<= Appointment Time < Next 
			//If appointment time is equal, it always goes after
			if (CurrentPatient->getPT() <= AppointmentTime && AppointmentTime < NextPatient->getPT()) {

				Node<Patient*>* newNode = new Node<Patient*>(input);
				newNode->setNext(current->getNext());
				current->setNext(newNode);

				return true;

			}
			current = current->getNext();


		}
		//Case D: Input is largest appointment time , enqued normally
		enqueue(input);
		return true;
	}

};