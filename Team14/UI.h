#pragma once
#include "Scheduler.h"
using namespace std;
class UI {

public:


	void printAll(
			int timestep,int X,
			LinkedQueue<Patient*> alllist,          
			EU_Waitlist U_Waitlist,                 
			EU_Waitlist E_Waitlist,                 
			X_Waitlist X_Waitlist,                  
			priQueue<Patient*> EarlyList,           
			priQueue<Patient*> LateList,            
			LinkedQueue<Resource*> AvailE,          
			LinkedQueue<Resource*> AvailU,         
			LinkedQueue<Resource*> AvailX,         
			priQueue<Patient*> InTreatment,         
			ArrayStack<Patient*> Finished           
	) {
		printNumbers(timestep,X); 

		printAllList(alllist);
		printWaitinglists(U_Waitlist, E_Waitlist, X_Waitlist);
		printEarly(EarlyList);
		printLate(LateList);
		printE(AvailE);
		printU(AvailU);
		printX(AvailX);
		printInTreatment(InTreatment);
		printfinished(Finished);

		cout << "Press any key to display next timestep\n";
		int garbage;
		cin >> garbage;
	}


	void printNumbers(int timestep,int X) {
		
		cout << "Current Timestep: " << timestep;
		
		
		cout << "X = " << X << " :";

			if (X < 10)
				cout << "Early?Wait";
			else if (X < 20)
				cout << "Late?Wait (penalty)";
			else if (X < 40)
				cout << "2 Wait?Treat";
			else if (X < 50)
				cout << "Treat?Wait";
			else if (X < 60)
				cout << "Treat?Finish";
			else if (X < 70)
				cout << "X-Wait?Finish (cancel)";
			else if (X < 80)
				cout << "Early reschedule";
			else
				cout << "No action";
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
		}
		cout << "\n";

	}

	//print lists
	void printAllList(LinkedQueue<Patient*>alllist) {
		
		printListLine(0);
		
		int count = alllist.count();
		
		cout << count << "  patients remaining: ";

		if (count > 10)count = 10; //max print 10 patients

		for (int i = 0; i < count; i++) {
			Patient* tempPatient;
			alllist.dequeue(tempPatient);
			cout << "P" << tempPatient->getId() << "_" << tempPatient->getVT() << ",";
		}
		cout << ".....\n";
	}

	void printWaitinglists(EU_Waitlist U, EU_Waitlist E, X_Waitlist X) {
		printListLine(1);
		WaitingHelper(U, 'U');
		WaitingHelper(E, 'E');
		WaitingHelper(X, 'X');
	}
	
	void printEarly(priQueue<Patient*>input) {
		printListLine(2);
		int count = input.count();
		cout << count << " patients:";
		Patient* garbage;
		int garb;
		for (int i = 0; i < count; i++) {
			input.dequeue(garbage,garb);
			cout << " " << garbage->getId() << ",";
		}
		cout << "\n";
	}

	void printLate(priQueue<Patient*>input) {
		printListLine(3);
		int count = input.count();
		cout << count << " patients:";
		Patient* garbage;
		int garb;
		for (int i = 0; i < count; i++) {
			input.dequeue(garbage, garb);
			cout << " " << garbage->getId() << ",";
		}
		cout << "\n";
	}

	void printE(LinkedQueue<Resource*>input) {
		printListLine(4);
		int count = input.count();
		cout << count << "  Electro device:";
		Resource* garbage;
	
		for (int i = 0; i < count; i++) {
			input.dequeue(garbage);
			cout << " " << garbage->getID() << ",";
		}
		cout << "\n";
	}

	void printU(LinkedQueue<Resource*>input) {
		printListLine(5);
		int count = input.count();
		cout << count << "  Ultra device:";
		Resource* garbage;

		for (int i = 0; i < count; i++) {
			input.dequeue(garbage);
			cout << " " << garbage->getID() << ",";
		}
		cout << "\n";
	}

	void printX(LinkedQueue<Resource*>input) {
		printListLine(6);
		int count = input.count();
		cout << count << " rooms";
		Resource* garbage;
		for (int i = 0; i < count; i++) {
			input.dequeue(garbage);
			cout << " " <<"R"<< garbage->getID() <<"["<<garbage->getPatientCount()<<","<<garbage->getCapacity()<<"]"<<",";
		}
		cout << "\n";
	}

	void printInTreatment(priQueue<Patient*>input) {
		printListLine(7);
		int count = input.count();
		cout << count << " ==>";
		Patient* temp;
		int garbage;
		for (int i = 0; i < count; i++) {
			input.dequeue(temp,garbage);
			cout << " P" << temp->getId() << "_" << temp->getCurrentTreatment()->getType() << temp->getCurrentTreatment()->getID() << ",";
		}
		cout << "\n";
	}

	void printfinished(ArrayStack<Patient*>input) {
		printListLine(8);
		int count = input.count();
		cout << count << " finished patients:";
		Patient* temp;
		for (int i = 0; i < count; i++) {
			input.pop(temp);
			cout << " " << temp->getId() << ",";
		}
		cout << "\n";
	}
};
