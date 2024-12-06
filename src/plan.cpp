
#include <vector>
#include <iostream>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include "Plan.h"
#include <string>
using std::vector;

//constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy* selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId)
    , settlement(settlement)
    , selectionPolicy(selectionPolicy)
    , facilityOptions(facilityOptions)
    , status(PlanStatus::AVALIABLE)
    , facilities({})
    , underConstruction({})
    , life_quality_score(0)
    , economy_score(0)
    , environment_score(0)
    , settlementType(static_cast<int>(settlement.getType()))
    {}

//copy constructor
Plan::Plan(const Plan &other)
    : plan_id(other.plan_id)
    , settlement(other.settlement)
    , selectionPolicy(other.selectionPolicy->clone())
    , facilityOptions(other.facilityOptions)
    , status(other.status)
    , facilities()
    , underConstruction()
    , life_quality_score(other.life_quality_score)
    , economy_score(other.economy_score)
    , environment_score(other.environment_score)
    , settlementType(other.settlementType)
{
    for(size_t i = 0; i < other.facilities.size(); i++){
        facilities.push_back(new Facility(*facilities[i])); 
    }
    for(size_t i = 0; i < other.underConstruction.size(); i++){
        underConstruction.push_back(new Facility(*underConstruction[i]));
    }
}

//distractor
Plan::~Plan()
{
   for(size_t i = 0; i < facilities.size(); i++){
        delete facilities[i];
    } 
    facilities.clear();
    for(size_t i = 0; i < underConstruction.size(); i++){
        delete underConstruction[i];
    } 
    underConstruction.clear();
    delete selectionPolicy;
}

//move constructer
Plan::Plan(Plan &&other)
: plan_id(other.plan_id)
, settlement(other.settlement)
, selectionPolicy(other.selectionPolicy)
, facilityOptions(std::move(other.facilityOptions))
, status(other.status)
, facilities(std::move(other.facilities))
, underConstruction(std::move(other.underConstruction))
, life_quality_score(other.life_quality_score)
, economy_score(other.economy_score)
, environment_score(other.environment_score)
, settlementType(other.settlementType)
{
    other.selectionPolicy = nullptr;
}

//getter for life quality score
const int Plan::getlifeQualityScore() const{
    return life_quality_score;
}

//getter for economy score
const int Plan::getEconomyScore() const{
    return economy_score;
}

//getter for enviroment score
const int Plan::getEnvironmentScore() const{
    return environment_score;
}

//setter for the plan's selection policy
void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
    delete this->selectionPolicy;
    this->selectionPolicy = selectionPolicy;
}

//adds new facilities if possible and advances all facilities under construction by 1 time unit
void Plan::step(){
    //adds new facilities as long as the status of the plan is "Avaliable"
    while(status == PlanStatus::AVALIABLE){
        Facility* newFacilityPtr = Plan::chooseNextFacility();
        Plan::addFacility(newFacilityPtr);
        if(underConstruction.size() == settlementType + 1){
            status = PlanStatus::BUSY;
        }
    }

    //decrees all under construction by 1
    for(size_t i=0; i < underConstruction.size(); i++){
        Facility* currFacility = underConstruction[i];
        currFacility->step();
        if(currFacility->getStatus() == FacilityStatus::OPERATIONAL){
            //add the Facility to the facilities vector
            facilities.push_back(currFacility);
            //erase the Facility from underConstruction vector
            underConstruction.erase(underConstruction.begin() + i);
            //updates the scores of the plan after adding this facility to the settlement
            Plan::updateScores(currFacility);
            //update i so it will match the index of the "underConstruction" vector
            i--;
        }
    }
    //update status
    if(underConstruction.size() < settlementType + 1){
        status = (PlanStatus::AVALIABLE);
    }
    else{
        status = (PlanStatus::BUSY);
    }
}

//prints all the information about the plan
void Plan::printStatus(){
    std::cout << this->toString() << std::endl;
}

//adds a new facility to the underConstruction vector
void Plan::addFacility(Facility* facility){
    underConstruction.push_back(facility);
}

// chooses the next facility to be added to the plan according to the selection policy
Facility* Plan::chooseNextFacility() const{
    Facility newFacility (selectionPolicy->selectFacility(facilityOptions),settlement.getName());
    Facility* newFacilityPtr = &newFacility;
    return newFacilityPtr;
}

//updates the scores of the plan according to the new facility that was added
void Plan::updateScores(Facility* newFacility) {
    life_quality_score += newFacility->getLifeQualityScore();
    economy_score += newFacility->getEconomyScore();
    environment_score += newFacility->getEnvironmentScore();
}

//converts the plan status to the matching string
string Plan::statusToString() const{
    string currStatus;
    switch(status){
        case PlanStatus::AVALIABLE: currStatus= "AVALIABLE";
        break;
        case PlanStatus::BUSY: currStatus= "BUSY";
        break;
    }
    return currStatus;
}

//getter for plan ID
int Plan::getPlanId(){
    return plan_id;
}

//getter for selection policy
SelectionPolicy *Plan::getSelectionPolicy()
{
    return selectionPolicy;
};

//converts all the information about the plan to string" 
 const string Plan::toString() const{
    string facilitiesString = "";

    //all the operational facilities 
    for(size_t i = 0; i < facilities.size(); i++){
            facilitiesString += "Facility Name: " + facilities[i]->getName() +"\n";
            facilitiesString += "Facility Status: OPERATIONAL\n";
        }
    
    //all the facilities under construction
    for(size_t i = 0; i < underConstruction.size(); i++){
        facilitiesString += "Facility Name: " + underConstruction[i]->getName() +"\n";
        facilitiesString += "Facility Status: UNDER_CONSTRUCTIONS\n";
    }

    return  (
            "Plan ID: " + std::to_string(plan_id) + "\n" +
            "PlanStatus: " + statusToString() + "\n" +
            settlement.toString() + "\n" +
            selectionPolicy->toString() + "\n" +
            "Life Quality Score: " + std::to_string(life_quality_score) + "\n" +
            "Economy Score: " + std::to_string(environment_score) + "\n" +
            "Envrionmental Score: " + std::to_string(economy_score) + "\n" +
            facilitiesString);
 }

