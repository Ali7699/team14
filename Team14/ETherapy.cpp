#include "ETherapy.h"
#include "Scheduler.h"

ETherapy::ETherapy(int d) : Treatment('E', d) { }

bool ETherapy::canAssign() {
    return false;

}

void ETherapy::moveToWait(Scheduler*This,int CASE) {


}