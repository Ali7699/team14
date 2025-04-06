#pragma once
#include <cstdlib>
#include <ctime>

#include "Scheduler.h"
#include "UI.h"
int main() {
	UI ui;
	Scheduler schedule;
	
	schedule.setUI(&ui);

};