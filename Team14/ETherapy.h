#pragma once
#include "Treatment.h"

class ETherapy :public Treatment {
    //phase 2 implementation
public:
    ETherapy(int d);

    bool canAssign() override;

    void moveToWait() override;
};