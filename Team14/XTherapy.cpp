#include "XTherapy.h"
#include "Scheduler.h"
#include "Patient.h"

XTherapy::XTherapy(int d) : Treatment('X', d) { }

bool XTherapy::canAssign() {


	return false;

}

void XTherapy::moveToWait(Scheduler* This, Patient* input, int CASE) {

	This->AddtoX(input, CASE);
}