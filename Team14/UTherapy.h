#pragma once
#include "Treatment.h"

class UTherapy :public Treatment {


public:
    UTherapy(int d);

    bool canAssign()override;

    void moveToWait(Scheduler* This, Patient* input, int CASE) override;
};
