#pragma once
class Resource {
private:
    char Type;  // E, U or X
    int ID;     
    int Capacity; // Only used for Gyms (X-type), default=-1 for devices
public:
    Resource(char t, int id, int cap = 1) : Type(t), ID(id), Capacity(cap) {
        if (Type != 'X')
            Capacity = -1;
    }

    // Getters
    char getType() const { return Type; }
    int getCapacity() const { return Capacity; }
    int getID()const { return ID; }

    //setters
    void setType(char t) { Type = t; }
    void setCapacity(int t) { Capacity = t; }
    void setId(int t) { ID = t; }

};