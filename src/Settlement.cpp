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
    string typeString;
    switch (type) {
        case SettlementType::VILLAGE:
        typeString = "Village";
        break;
        case SettlementType::CITY:
        typeString = "CITY";
        break;
        case SettlementType::METROPOLIS:
        typeString = "METROPOLIS";
        break;
    }
    return "Settlement [Name: " + name + ",Type: " + typeString + "]";
}