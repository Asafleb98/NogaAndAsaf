#include "SelectionPolicy.h"
#pragma once
#include <vector>
#include "Facility.h"
#include <algorithm>
using std::vector;

//------NaiveSelection-----
NaiveSelection::NaiveSelection(): lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    lastSelectedIndex++;
    return facilitiesOptions[lastSelectedIndex];
    }

//const string NaiveSelection::toString() const;

//NaiveSelection NaiveSelection::*clone() const

NaiveSelection::~NaiveSelection() = default;

//------EconomySelection-----

EconomySelection::EconomySelection(): lastSelectedIndex(-1) {}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    //to deal with lastSelectedIndex = -1 and the size of the vector
    lastSelectedIndex=(lastSelectedIndex++)%(facilitiesOptions.size());
    while(facilitiesOptions[lastSelectedIndex].getCategory() != FacilityCategory::ECONOMY){
        lastSelectedIndex=(lastSelectedIndex++)%(facilitiesOptions.size());
    }
    return facilitiesOptions[lastSelectedIndex];
}
    
//const string EconomySelection::toString() const{}

//EconomySelection EconomySelection::*clone() const{}

EconomySelection::~EconomySelection() = default;


//------SustainabilitySelection-----

SustainabilitySelection::SustainabilitySelection(): lastSelectedIndex(-1) {}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    //to deal with lastSelectedIndex = -1 and the size of the vector
    lastSelectedIndex=(lastSelectedIndex++)%(facilitiesOptions.size());
    while(facilitiesOptions[lastSelectedIndex].getCategory() != FacilityCategory::ECONOMY){
        lastSelectedIndex=(lastSelectedIndex++)%(facilitiesOptions.size());
    }
    return facilitiesOptions[lastSelectedIndex];
}

//const string SustainabilitySelection::toString() const{}

//SustainabilitySelection SustainabilitySelection::*clone() const{}

SustainabilitySelection::~SustainabilitySelection() = default;

//------BalancedSelection-----


BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):
    LifeQualityScore(LifeQualityScore),
    EconomyScore(EconomyScore),
    EnvironmentScore(EnvironmentScore)
{}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    int difference = -1;
    int minValue;
    int savedIndex = 0;
    for(int i = 0; i <= facilitiesOptions.size()-1 && difference != 0; i++){
        int maxScore = std::max(LifeQualityScore + facilitiesOptions[i].getLifeQualityScore(), EconomyScore + facilitiesOptions[i].getEconomyScore(), EnvironmentScore + facilitiesOptions[i].getEnvironmentScore());
        int minScore = std::min(LifeQualityScore + facilitiesOptions[i].getLifeQualityScore(), EconomyScore + facilitiesOptions[i].getEconomyScore(), EnvironmentScore + facilitiesOptions[i].getEnvironmentScore());
        difference = maxScore - minValue;
        if(i == 0){
            minValue = difference;
        }
        else if (difference < minValue){
            minValue = difference;
            savedIndex = i;
        }
    }
    return facilitiesOptions[savedIndex];
}


//const string toString() const override;


//BalancedSelection *clone() const override;


BalancedSelection:: ~BalancedSelection() = default;
