#pragma once
class Treatment;
class Patient;

class XTherapy :public Treatment {
   
public:
    XTherapy(int d) : Treatment('X', d) {}

    

    bool canAssign(Scheduler& scheduler, int currentTime) override {

    }

    void moveToWait(Scheduler& scheduler, Patient* patient) override {

    }
};
