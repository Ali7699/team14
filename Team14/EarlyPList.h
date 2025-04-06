#pragma once
#include "priQueue.h"
#include "Patient.h"
#include <cstdlib>

class EarlyPList : public priQueue<Patient*>
{
public:


	//reschudle is called if propability permits 
	/* Reschedule is Responsible for Choosing random patient and updating their new PT randomly, 
	NOT for when its called
		*/
	bool Reschedule() {  
		//validate prique has patients
		if (isEmpty()) {
			return false;
		}
		//count the number of patients
		int count = 0;
		priNode<Patient*>* current = head;
		while (current != nullptr) {
			count++;
			current = current->getNext();
		}
		//reset temp head
		current = head; 
		//get random patient
		int index = std::rand() % count; 
		
		
		priNode<Patient*>* prev = nullptr;
		current = head;
		//go to that random patient
		for (int i = 0; i < index; i++) {
			prev = current;
			current = current->getNext();
		}
		
		//safety catch
		if (current == nullptr) return false;
		
		


		int garbage; 
		
		//priority not needed so we store in garbage
		Patient* TempPatient = current->getItem(garbage);
		
		//now get rid of current
		if (prev == nullptr) {
			// A.We never entered the loop. prev is still nullptr. hence its just the head
			head = current->getNext();
		}
		else {
			//B. We DID enter the loop, so delete normally
			prev->setNext(current->getNext());
		}
		delete current;
		
		
		int OriginalAppointment = TempPatient->getPT(); 

		//generate new later random appointment time, new appointment up to 50 timesteps later
		int NewAppointment = OriginalAppointment + (std::rand() % 51);

		TempPatient->setPT(NewAppointment);
		
		//enque with new appointment time (This is a Min que so appointment time is directly priority)
		enqueue(TempPatient, NewAppointment);
		return true;

	}
};