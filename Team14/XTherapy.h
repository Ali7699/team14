#pragma once
#include "Treatment.h"

class XTherapy :public Treatment {
   
public:
    XTherapy(int d) : Treatment('X', d) {}

    

    bool canAssign() override;

    void moveToWait();
};
