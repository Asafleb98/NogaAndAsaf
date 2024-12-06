
#include "Facility.h"
#include <iostream>

using std::string;
using std::ostringstream;

//-----FacilityType-----
 
// Constructor
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : name(name)
    , category(category)
    , price(price)
    , lifeQuality_score(lifeQuality_score)
    , economy_score(economy_score)
    , environment_score(environment_score) {}

// Getters
const string& FacilityType::getName() const {
    return name;
}

int FacilityType::getCost() const {
    return price;
}

int FacilityType::getLifeQualityScore() const {
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const {
    return environment_score;
}

int FacilityType::getEconomyScore() const {
    return economy_score;
}

FacilityCategory FacilityType::getCategory() const {
    return category;
}

//-----Facility-----

//constructor
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : FacilityType(name
    , category
    , price
    , lifeQuality_score
    , economy_score
    , environment_score)
    , settlementName(settlementName)
    , status(FacilityStatus::UNDER_CONSTRUCTIONS)
    , timeLeft(price){}

//constructor
Facility::Facility(const FacilityType &type, const string &settlementName)
:Facility::Facility(type.getName()
, settlementName
, type.getCategory()
, type.getCost()
, type.getLifeQualityScore()
, type.getEconomyScore()
, type.getEnvironmentScore()) {}

//getter for the settlements name
const string& Facility::getSettlementName() const{
    return settlementName;
}

//getter for the remaining time until the facilty is operational
const int Facility::getTimeLeft() const{
    return timeLeft;
}

//reduces the time left by 1, and changes the facility's status if needed
FacilityStatus Facility::step(){
    if(timeLeft > 0){
        timeLeft = timeLeft - 1;
    }

    if(timeLeft == 0){
        setStatus(FacilityStatus::OPERATIONAL);
    }
    return status;
}

//setter for the facilitie's status
void Facility::setStatus(FacilityStatus status){
    this->status=status;
}

//getter for the facilitie's status
const FacilityStatus& Facility::getStatus() const{
    return status;
}

//converts the facility to a matching string
const string Facility:: toString() const{
    return ("Facility Name: " + name);
}