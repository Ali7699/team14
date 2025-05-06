#pragma once
#include "EU_Waitlist.h"

class X_Waitlist:public EU_Waitlist {
public:
	
	//this function is responsible for 1.determining if an eligible patients exists to cancel 2.choosing one of them to deque.
	//it is NOT responiosble for when its called, might be called when list is empty so naturally no patient return
	bool cancel(Patient*&outpatient,int timestep) {
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
			
			//THIS RETURNS A REFRENCE
			LinkedQueue<Treatment*> &TempTreatmentList=TempPatient->getTreatmentlist();
			
			
			bool Xchecker=false;

			int count = TempTreatmentList.count();
			//avoids nullptr abuse
			if (!TempTreatmentList.isEmpty()) {
				for (int i = 0; i < count; i++) {
					Treatment* Temptreatment;
					TempTreatmentList.dequeue(Temptreatment); //if we successfully deque
					if (Temptreatment->getType() == 'X') {
							Xchecker = true;

					}
					TempTreatmentList.enqueue(Temptreatment); //we are directly editing the datamemeber list so we must restore it


				}

				if (Xchecker && (count == 1)) {
					//we now effectively checked if the patient has X ANDDD its his last treatment
					//so we can store the patient
					Arr[index] = TempPatient;
					index++;
				}
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
		//we found the chosen patient, update his waiting time since now hes done, and flag as canceled
		{
			ChosenPatient->setCancel(1);

			int WIS = ChosenPatient->getWIS();
			int TW = ChosenPatient->getTW();
			//the logic here:
			//patient has been waiting since last instance, and add that to the he has intially waited TW
			ChosenPatient->setTW((timestep-WIS) + TW);
		}

		
		//now finally find The Chosenpatient and remove him

		Node<Patient*>* prev = nullptr;
		current = frontPtr;
		
		while (current != nullptr) {
			if (current->getItem() == ChosenPatient) {
				if (prev == nullptr) {
					//prev only is ever nullptr first run, therefore Patient is in first node
					frontPtr = current->getNext();
					if (frontPtr == nullptr) {
						backPtr = nullptr; // List is now empty
					}
				}
				else {
					prev->setNext(current->getNext());
					// Update backPtr if removing the last node
					if (current->getNext() == nullptr) {
						backPtr = prev;
					}
				}
				outpatient = current->getItem();
				delete current;
				return true;
			}
			prev = current;
			current = current->getNext();
		}

		return false;
	}
	

};