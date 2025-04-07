#pragma once
#include"Resource.h"



//including scheduler causes circular depednacy so this is safer
class Scheduler;
class Patient;

class Treatment {
protected:
    // Duration
    int duration;         
    // Assignment time
    int ST;    
     
    // E for E-therapy, U for U-therapy, X for X-therapy
    char type;           

public:
    
    
    Treatment(char t, int d) : type(t), duration(d), ST(-1) { } 
    
    virtual ~Treatment() {}

    // Virtual function to check if the required Resource is available
    virtual bool canAssign( int currentTime) = 0;

    // Virtual function to move patient to waiting list
    virtual void moveToWait( Patient* patient) = 0;
    
    static Treatment* createTreatment(char type, int duration);


    // getters
    char getType() const { return type; }
    int getDuration() const { return duration; }
    int getAT() const { return ST; }
   

    // setters
    void setAT(int time) { ST = time; }

    // Check if treatment is completed at current time
    bool isCompleted(int currentTime) const {
        return ST != -1 && (currentTime - ST) >= duration; // if assignment time is not null and is duration has passed, return true
    }
};

