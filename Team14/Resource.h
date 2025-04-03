#pragma once
class Resource {
private:
    char Type;  // E, U or X
    int ID;   
    int capacity;
public:
    Resource(char t, int id, int cap=-1) : Type(t), ID(id),capacity(cap){
   
        //capacity is only for gym rooms
        if (Type != 'X')
            capacity = -1;
       
     
    }

    // Getters
    char getType() const { return Type; }
    int getCapacity() const { return capacity; }
    int getID()const { return ID; }
    
    
    //setters
    void setType(char t) { Type = t; }
    void setCapacity(int t) { capacity = t; }
    void setId(int t) { ID = t; }

};