#pragma once
#include <cstdlib>
#include <ctime>

#include "Scheduler.h"
#include "UI.h"
int main() {
	srand(static_cast<unsigned int>(time(0)));

	UI ui;
	Scheduler schedule;
	
	schedule.setUI(&ui);
	
	std::cout << "Choose input file";
	int input;
	std::cin >> input;
	schedule.Simulation(input);
	return 0;
	
	
	//test
	//test2
};