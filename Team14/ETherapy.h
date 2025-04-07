#pragma once
#include "Treatment.h"

class ETherapy :public Treatment {
    //phase 2 implementation
public:
    ETherapy(int d) : Treatment('E', d) {}

    bool canAssign( int currentTime) override {
        return false;
    }

    void moveToWait( Patient* patient) override {

    }
};