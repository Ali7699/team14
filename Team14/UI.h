#pragma once
class scheduler;

class UI {

public:


	void printAll() {

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
		std::string temp;
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
			std::cout << "=";
		}
		std::cout << " " << temp << " ";
		for (int i = 0; i < 20; i++) {
			std::cout << "=";
		}
		std::cout << "\n";


	}

	void print
};
