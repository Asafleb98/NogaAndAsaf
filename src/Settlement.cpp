#include "Settlement.h"

//Constructor
Settlement::Settlement(const string &name, SettlementType type)
    : name(name)
    , type(type) {}    

const string& Settlement::getName() const {
    return name;
}

SettlementType Settlement:: getType() const{
    return type; 
}

const string Settlement:: toString() const {
 
    return ("Settlement Name: " + this->getName());
}