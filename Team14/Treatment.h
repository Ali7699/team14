#pragma once
#include"Resource.h"



//including scheduler causes circular depednacy so this is safer
class Scheduler;

class Treatment {
protected:
    // Duration
    int duration;         
    // Assignment time
    int ST;    
     
    // E for E-therapy, U for U-therapy, X for X-therapy
    char type;           
   
    
    //pointer to resource patient may be using
    Resource* OccupiedResource;
public:
    
    
    Treatment(char t, int d) : type(t), duration(d), ST(-1) ,OccupiedResource(nullptr){ } 
    
    virtual ~Treatment() {}

    // Virtual function to check if the required Resource is available
    virtual bool canAssign() = 0;

    // Virtual function to move patient to waiting list
    virtual void moveToWait(Scheduler* This, int CASE) = 0;
    
    static Treatment* createTreatment(char type, int duration);


    // getters

    //Type can only be strictly E, U or X
    char getType() const { return type; }
    int getDuration() const { return duration; }
    int getST() const { return ST; }
    Resource* getResource() const { return OccupiedResource; }


    //setters

    //assignment time
    void setST(int time) { ST = time; }
    
    void setResource(Resource* input) {
        OccupiedResource = input;
    }





    // Check if treatment is completed at current time
    
        bool isCompleted(int currentTime) const {
        return ST != -1 && (currentTime - ST) >= duration; // if assignment time is not null and is duration has passed, return true
    }
};

