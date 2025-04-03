#pragma once
#include "Treatment.h"

class XTherapy :public Treatment {
   
public:
    XTherapy(int d) : Treatment('X', d) {}

    

    bool canAssign( int currentTime) override {

    }

    void moveToWait( Patient* patient) override {

    }
};
