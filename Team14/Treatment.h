#pragma once
#include"Resource.h"
#include"Scheduler.h"
class Treatment {
protected:
    // Duration
    int duration;         
    // Assignment time
    int ST;    
    // pointer to the assigned Resource
    Resource* AR;    
    // E for E-therapy, U for U-therapy, X for X-therapy
    char type;           

public:
    Treatment(char t, int d) : type(t), duration(d), ST(-1), AR(nullptr) {}
    virtual ~Treatment() {}

    // Virtual function to check if the required Resource is available
    virtual bool canAssign(Scheduler& scheduler, int currentTime) = 0;

    // Virtual function to move patient to waiting list
    virtual void moveToWait(Scheduler& scheduler, Patient* patient) = 0;

    // getters
    char getType() const { return type; }
    int getDuration() const { return duration; }
    int getAT() const { return ST; }
    Resource* getAR() const { return AR; }

    // setters
    void setAT(int time) { ST = time; }
    void setAR(Resource* Resource) { AR = Resource; }

    // Check if treatment is completed at current time
    bool isCompleted(int currentTime) const {
        return ST != -1 && (currentTime - ST) >= duration; // if assignment time is not null and is duration has passed, return true
    }
};

