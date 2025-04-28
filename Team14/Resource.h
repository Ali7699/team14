#pragma once
class Resource {
private:
    char Type;  // E, U or X
    int ID;
    int capacity;
    int patientCount;

public:
    Resource(char t, int id, int cap = 0, int Count = 0) : Type(t), ID(id), capacity(cap), patientCount(Count) {
        //capacity is only for gym rooms
        if (Type != 'X') {
            capacity = -1;
            patientCount = -1;
        }
    }

    // Getters
    char getType() const { return Type; }
    int getCapacity() const { return capacity; }
    int getID()const { return ID; }
    int getPatientCount()const { return patientCount; }
    
    //setters
    void setType(char t) { Type = t; }
    void setCapacity(int t) { capacity = t; }
    void setId(int t) { ID = t; }
    void setPatientCount(int t) { patientCount = t; }

    bool isFull() {
        if (patientCount >= capacity) {
            return true;
        }
        else if (patientCount < capacity) {
            return false;
        }

    }
};