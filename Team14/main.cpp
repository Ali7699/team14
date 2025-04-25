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
	
	schedule.Simulation();
	return 0;
	//test
	//test2
};