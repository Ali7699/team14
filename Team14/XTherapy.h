#pragma once
#include "Treatment.h"

class XTherapy :public Treatment {
   
public:
    XTherapy(int d) : Treatment('X', d) {}

    

    bool canAssign( int currentTime) override {
        return false;
    }

    void moveToWait( Patient* patient) override {

    }
};
