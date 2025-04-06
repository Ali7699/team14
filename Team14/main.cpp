#pragma once
#include <cstdlib>
#include <ctime>

#include "Scheduler.h"
int main() {
	UI ui;
	Scheduler schedule;
	
	schedule.setUI(&ui);

};