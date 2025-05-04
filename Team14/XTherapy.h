#pragma once
#include "Treatment.h"

class XTherapy :public Treatment {
   
public:
    XTherapy(int d);

    

    bool canAssign() override;

    void moveToWait(Scheduler* This, int CASE)override;
};
