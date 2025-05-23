#pragma once
#include "Scheduler.h"
using namespace std;
class UI {

public:

	//takes copies and prints
	void printAll(
		int timestep,
		int X,
		LinkedQueue<Patient*>& alllist,
		EU_Waitlist& U_Waitlist,
		EU_Waitlist& E_Waitlist,
		X_Waitlist& X_Waitlist,
		priQueue<Patient*>& EarlyList,
		priQueue<Patient*>& LateList,
		LinkedQueue<Resource*>& AvailE,
		LinkedQueue<Resource*>& AvailU,
		LinkedQueue<Resource*>& AvailX,
		priQueue<Patient*>& InTreatment,
		ArrayStack<Patient*>& Finished
	) {
		system("cls");

		printNumbers(timestep); 
		printAllList(alllist);
		printWaitinglists(U_Waitlist, E_Waitlist, X_Waitlist);
		printEarly(EarlyList);
		printLate(LateList);
		printE(AvailE);
		printU(AvailU);
		printX(AvailX);
		printInTreatment(InTreatment);
		printfinished(Finished);
		
		system("pause");
	}


	void printNumbers(int timestep) {
		
		cout << "Current Timestep:" << timestep;
			cout << "\n";
	}

	
	//helpers
	void printListLine(int currentline) {
		/*
			0 = ALL List
			1 = Waiting Lists
			2 = Early List
			3 = Late List
			4 = Avail E - devices
			5 = Avail U - devices
			6 = Avail X - rooms
			7 = In - treatment List
			8 = finished patients
		*/
		string temp;
		switch (currentline) {
		case 0:
			temp = "ALL List";
			break;
		case 1:
			temp = "Waiting Lists";
			break;
		case 2:
			temp = "Early List";
			break;
		case 3:
			temp = "Late List";
			break;
		case 4:
			temp = "Avail E - devices";
			break;
		case 5:
			temp = "Avail U - devices";
			break;
		case 6:
			temp = "Avail X - rooms";
			break;
		case 7:
			temp = "In - treatment List";
			break;
		case 8:
			temp = "Finished patients";
			break;
		default:
			break;
		}

		for (int i = 0; i < 20; i++) {
			cout << "=";
		}
		cout << " " << temp << " ";
		for (int i = 0; i < 20; i++) {
			cout << "=";
		}
		cout << "\n";


	}

	void WaitingHelper(LinkedQueue<Patient*>& List, char TYPE) {
		int count = List.count();
		Patient* garbage;
		cout << count << " " << TYPE << "-therapy patients:";
		for (int i = 0; i < count; i++) {
			List.dequeue(garbage);
			cout << " " << garbage->getId() << ",";
			List.enqueue(garbage);
		}
		cout << "\n";

	}

	//print lists
	void printAllList(LinkedQueue<Patient*>& alllist) {
		
		printListLine(0);
		
		int count = alllist.count();
		
		cout << count << "  patients remaining: ";


		for (int i = 0; i < count; i++) {
			Patient* tempPatient;
			alllist.dequeue(tempPatient);
			alllist.enqueue(tempPatient);
			if (i < 11) { //only print first 10, but enque all
				cout << "P" << tempPatient->getId() << "_" << tempPatient->getVT() << ",";
			}
			
		}
		cout << ".....\n";
	}

	void printWaitinglists(EU_Waitlist&U, EU_Waitlist& E, X_Waitlist& X) {
		printListLine(1);
		WaitingHelper(U, 'U');
		WaitingHelper(E, 'E');
		WaitingHelper(X, 'X');
	}
	
	void printEarly(priQueue<Patient*>& input) {
		printListLine(2);
		int count = input.count();
		cout << count << " patients:";
		LinkedQueue<Patient*> storage;

		int garb;
		for (int i = 0; i < count; i++) {
			Patient* garbage;
			input.dequeue(garbage,garb);
			cout << " " << garbage->getId() << ",";
			storage.enqueue(garbage);
		}
		for (int i = 0; i < count; i++) {
			Patient* garbage;
			storage.dequeue(garbage);
			input.enqueue(garbage,garbage->getPT());
		}

		cout << "\n";
	}

	void printLate(priQueue<Patient*>& input) {
		printListLine(3);
		int count = input.count();
		cout << count << " patients:";
		LinkedQueue<Patient*> storage;

		int garb;
		for (int i = 0; i < count; i++) {
			Patient* garbage;
			input.dequeue(garbage, garb);
			cout << " " << garbage->getId() << ",";
			storage.enqueue(garbage);
		}
		for (int i = 0; i < count; i++) {
			Patient* garbage;
			storage.dequeue(garbage);
			input.enqueue(garbage, garbage->getoriginalPT()+garbage->getPenalty());
		}
		cout << "\n";
	}

	void printE(LinkedQueue<Resource*>& input) {
		printListLine(4);
		int count = input.count();
		cout << count << "  Electro device:";
		Resource* garbage;
	
		for (int i = 0; i < count; i++) {
			input.dequeue(garbage);
			cout << " " << garbage->getID() << ",";
			input.enqueue(garbage);
		}
		cout << "\n";
	}

	void printU(LinkedQueue<Resource*>& input) {
		printListLine(5);
		int count = input.count();
		cout << count << "  Ultra device:";
		Resource* garbage;

		for (int i = 0; i < count; i++) {
			input.dequeue(garbage);
			cout << " " << garbage->getID() << ",";
			input.enqueue(garbage);
		}
		cout << "\n";
	}

	void printX(LinkedQueue<Resource*>& input) {
		printListLine(6);
		int count = input.count();
		cout << count << " rooms";
		Resource* garbage;
		for (int i = 0; i < count; i++) {
			input.dequeue(garbage);
			cout << " " <<"R"<< garbage->getID() <<"["<<garbage->getPatientCount()<<","<<garbage->getCapacity()<<"]"<<",";
			input.enqueue(garbage);
		}
		cout << "\n";
	}

	void printInTreatment(priQueue<Patient*>& input) {
		printListLine(7);
		int count = input.count();
		cout << count << " ==>";
		
		
		//we must store priority for easiest implementation

		int pri[200];
		Patient* temp;
		LinkedQueue<Patient*> storage;

		for (int i = 0; i < count; i++) {
			Patient* garbage;
			input.dequeue(garbage, pri[i]);
			cout << " P" << garbage->getId() << "_" << garbage->getNextTreatment()->getResource()->getType() << garbage->getNextTreatment()->getResource()->getID() << ",";
			storage.enqueue(garbage);
		}


		for (int i = 0; i < count; i++) {
			Patient* garbage;
			storage.dequeue(garbage);
			input.enqueue(garbage, pri[i]);
		}

		cout << "\n";
	}

	void printfinished(ArrayStack<Patient*>& input) {
		printListLine(8);
		int count = input.count();
		cout << count << " finished patients:";
		Patient* temp;
		ArrayStack<Patient*>Tempstack;
		for (int i = 0; i < count; i++) {
			input.pop(temp);
			cout << " " << temp->getId() << ",";
			Tempstack.push(temp);
		}
		for (int i = 0; i < count; i++) {
			Tempstack.pop(temp);
			input.push(temp);
		}

		cout << "\n";
	}
};
