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

	std::string filename = "Input/input" + std::to_string(input) + ".txt";

	

	
	bool silent;
	std::cout << "Silent mode? 0 for No, 1 for yes";
	std::cin >> silent;




	schedule.Simulation(input,silent);
	schedule.output();

	
	
	return 0;
	
	//test
	//test2
};