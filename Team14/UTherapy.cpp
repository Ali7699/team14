

#include "UTherapy.h"
#include "Scheduler.h"
#include "Patient.h"


UTherapy::UTherapy(int d) : Treatment('U', d) { }


bool UTherapy::canAssign() {
    return false;



}

void UTherapy::moveToWait(Scheduler* This, Patient* input, int CASE) {
    This->AddtoU(input, CASE);
}