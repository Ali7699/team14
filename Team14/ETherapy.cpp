#include "ETherapy.h"
#include "Scheduler.h"
#include "Patient.h"

ETherapy::ETherapy(int d) : Treatment('E', d) { }

bool ETherapy::canAssign() {
    return false;

}

void ETherapy::moveToWait(Scheduler*This,Patient*input,int CASE) {
    This->AddtoE(input, CASE);
}