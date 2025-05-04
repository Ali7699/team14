#include "XTherapy.h"
#include "Scheduler.h"

XTherapy::XTherapy(int d) : Treatment('X', d) { }

bool XTherapy::canAssign() {


	return false;

}

void XTherapy::moveToWait(Scheduler* This, int CASE) {




}