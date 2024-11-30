#pragma once
#include <vector>
#include <iostream>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include "Plan.h"
using std::vector;


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

const int Plan::getlifeQualityScore() const{
    return life_quality_score;
}

const int Plan::getEconomyScore() const{
    return economy_score;
}

const int Plan::getEnvironmentScore() const{
    return environment_score;
}
//צריך להוסי, פה שסוויץ'
void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
    this->selectionPolicy=selectionPolicy;
}
void Plan::step(){
    while(status == PlanStatus::AVALIABLE){
        Facility* newFacilityPtr = Plan::chooseNextFacility();
        Plan::addFacility(newFacilityPtr);
        //הוספתי כי הייתה פה לולאה אינסופית
        if(underConstruction.size() == settlementType + 1){
            status = PlanStatus::BUSY;
        }
    }
    //decrees all under construction by 1
    for(int i=0; i < underConstruction.size(); i++){
        Facility* currFacility = underConstruction[i];
        currFacility->step();
        if(currFacility->getStatus()==FacilityStatus::OPERATIONAL){
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

void Plan::printStatus(){
    string currStatus;
    switch(status){
        case PlanStatus::AVALIABLE: currStatus=  "AVALIABLE";
        break;
        case PlanStatus::BUSY: currStatus=  "BUSY";
        break;
    }
    std::cout <<"plan Status is: " << currStatus << std::endl;
}

//I think that this is all the function
//check if really need the if because we check it in step as well

void Plan::addFacility(Facility* facility){
    underConstruction.push_back(facility);
}
//new function Asaf MAke
Facility* Plan::chooseNextFacility() const{
    Facility newFacility (selectionPolicy->selectFacility(facilityOptions),settlement.getName());
    Facility* newFacilityPtr = &newFacility;
    return newFacilityPtr;
}

void Plan::updateScores(Facility* newFacility) {
    life_quality_score += newFacility->getLifeQualityScore();
    economy_score += newFacility->getEconomyScore();
    environment_score += newFacility->getEnvironmentScore();
}

//Will do it when we know what we have to print
// const string Plan::toString() const;

