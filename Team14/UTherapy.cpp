

#include "UTherapy.h"
#include "Scheduler.h"

UTherapy::UTherapy(int d) : Treatment('U', d) { }


bool UTherapy::canAssign() {
    return false;



}

void UTherapy::moveToWait(Scheduler* This, int CASE) {
    
}