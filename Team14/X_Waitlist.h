#pragma once
#include "EU_Waitlist.h"

class X_Waitlist:public EU_Waitlist {
public:
	bool cancel() {
		//intilizations
		Node<Patient*>*current = frontPtr;
		Patient* Arr[100];
		int index = 0;
		
		if (isEmpty()) {
			return false;
		}

		//first, identify eligible patients
		while (current!= nullptr) {
			Patient*TempPatient=current->getItem();
			LinkedQueue<Treatment*> TempTreatmentList=TempPatient->getTreatmentlist();
			int treatmentcount=0;
			bool Xchecker=false;

			for (int i = 0; i < 3; i++) {
				Treatment* Temptreatment;
				if (TempTreatmentList.dequeue(Temptreatment)) {//if we successfully deque
					treatmentcount++;
					if (Temptreatment->getType() == 'X') {
						 Xchecker = true;


					}

				}
				

			}
			if (Xchecker && (treatmentcount == 1)) {
				//we now effectively checked if the patient has X and its his last treatment
				//so we can store the patient
				Arr[index] = TempPatient;
				index++;
			}

			current = current->getNext();
		}
		
		//all eligible are now stored
		if (index == 0) {
			//no eligible patients found
			return false;
		}


		int randomindex = std::rand() % (index);
		
		Patient* ChosenPatient = Arr[randomindex];
		
		//now finally find The Chosenpatient and remove him

		//Case A: Patient is first on the list
		current = frontPtr;
		if (randomindex == 0) {
			
			frontPtr = current->getNext();
			
			//edge case for single node list, we need to update back ptr
			if (frontPtr == nullptr) {  
				backPtr = nullptr;      
			}
			delete current;
			return true;
		}
		
		//Case B: patient is anywhere else
		while (current!= nullptr&&current->getNext()!=nullptr) {
			
			if (current->getNext()->getItem() == ChosenPatient) {
				Node<Patient*>* prev = current;
				current = current->getNext();
				prev->setNext(current->getNext());

				delete current;
				return true;

			}
			current = current->getNext();
		}
		return false;
	}
	

};