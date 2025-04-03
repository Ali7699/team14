#pragma once
class Treatment;
class Patient;

class UTherapy :public Treatment {


public:
    UTherapy(int d) : Treatment('U', d) {}

    bool canAssign(Scheduler& scheduler, int currentTime) override {
       
    }

    void moveToWait(Scheduler& scheduler, Patient* patient) override {
    
    }
};
