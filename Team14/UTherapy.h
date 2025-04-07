#pragma once
#include "Treatment.h"

class UTherapy :public Treatment {


public:
    UTherapy(int d) : Treatment('U', d) {}

    bool canAssign( int currentTime) override {
        return false;
    }

    void moveToWait( Patient* patient) override {
    
    }
};
