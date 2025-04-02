#pragma once
#include"Treatment.h"
#include"Patient.h"
class ETherapy :public Treatment {
    //phase 2 implementation
public:
    ETherapy(int d) : Treatment('E', d) {}

    bool canAssign(Scheduler& scheduler, int currentTime) override {

    }

    void moveToWait(Scheduler& scheduler, Patient* patient) override {

    }
};