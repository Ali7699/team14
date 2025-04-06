#pragma once
#include "Scheduler.h"
using namespace std;
class UI {

public:


	void printAll() {
		
		
		
		
		
		cout << "Press any key to display next timestep\n";
		int garbage;
		std::cin >>garbage ;
	}

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

	void printAllList(LinkedQueue<Patient*>alllist) {
		
		
		int count = 0;
		LinkedQueue<Patient*>temp = alllist;
		while (alllist.isEmpty()) {
			Patient* garbage;
			alllist.dequeue(garbage);
			count++;
		}
		
		cout << count << "  patients remaining: ";

		if (count > 10)count = 10; //max print 10 patients

		for (int i = 0; i < count; i++) {
			Patient* tempPatient;
			temp.dequeue(tempPatient);
			cout << "P" << tempPatient->getId() << "_" << tempPatient->getVT() << ",";
		}
		cout << ".....\n";
	}

	void printWaitinglists() {

	}
	void printEarly() {

	}
	void printLate() {

	}
	void printE() {

	}
	void prinU() {

	}
	void printX() {

	}
	void printInTreatment() {

	}
	void printfinished() {

	}
};
