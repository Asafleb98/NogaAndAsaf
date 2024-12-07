#include "SelectionPolicy.h"
#include <vector>
#include "Facility.h"
#include <algorithm>
using std::vector;

//------NaiveSelection-----
NaiveSelection::NaiveSelection(): lastSelectedIndex(-1) {}

//selects the next facility to add to the plan according to naive order
const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    //to deal with lastSelectedIndex = -1 and the size of the vector
    lastSelectedIndex=(lastSelectedIndex + 1)%(facilitiesOptions.size());
    return facilitiesOptions[lastSelectedIndex];
    }

//converts policy to a matching string
const string NaiveSelection::toString() const{
    return ("SelectionPolicy: nve");
}

//creats a copy of the current policy
NaiveSelection *NaiveSelection::clone() const
{
    return new NaiveSelection(*this);
}


//------EconomySelection-----

EconomySelection::EconomySelection(): lastSelectedIndex(-1) {}

//selects the next facility to add to the plan by naive order, only if its from type "economy"
const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    //to deal with lastSelectedIndex = -1 and the size of the vector
    lastSelectedIndex=(lastSelectedIndex + 1)%(facilitiesOptions.size());
    while(facilitiesOptions[lastSelectedIndex].getCategory() != FacilityCategory::ECONOMY){
        lastSelectedIndex=(lastSelectedIndex + 1)%(facilitiesOptions.size());
    }
    return facilitiesOptions[lastSelectedIndex];
}

//converts policy to a matching string   
const string EconomySelection::toString() const{
    return ("SelectionPolicy: eco");
}

//creats a copy of the current policy
EconomySelection *EconomySelection::clone() const{
    return new EconomySelection(*this);
}


//------SustainabilitySelection-----

SustainabilitySelection::SustainabilitySelection(): lastSelectedIndex(-1) {}

//selects the next facility to add to the plan by naive order, only if its from type "enviroment"
const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    //to deal with lastSelectedIndex = -1 and the size of the vector
    lastSelectedIndex=(lastSelectedIndex + 1)%(facilitiesOptions.size());
    while(facilitiesOptions[lastSelectedIndex].getCategory() != FacilityCategory::ENVIRONMENT){
        lastSelectedIndex=(lastSelectedIndex + 1)%(facilitiesOptions.size());
    }
    return facilitiesOptions[lastSelectedIndex];
}

//converts policy to a matching string
const string SustainabilitySelection::toString() const{
    return ("SelectionPolicy: env");
}

//creats a copy of the current policy
SustainabilitySelection *SustainabilitySelection::clone() const{
    return new SustainabilitySelection(*this);
}


//------BalancedSelection-----


BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):
    LifeQualityScore(LifeQualityScore),
    EconomyScore(EconomyScore),
    EnvironmentScore(EnvironmentScore)
{}

//selects the next facility to add to the plan according to which will create the best score balance
const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    int difference = -1;
    int minValue;
    int savedIndex = 0;
    for(size_t i = 0; i < facilitiesOptions.size() && difference != 0; i++){
        int maxScore = std::max(LifeQualityScore + facilitiesOptions[i].getLifeQualityScore(), std::max(EconomyScore + facilitiesOptions[i].getEconomyScore(), EnvironmentScore + facilitiesOptions[i].getEnvironmentScore()));
        int minScore = std::min(LifeQualityScore + facilitiesOptions[i].getLifeQualityScore(), std::min(EconomyScore + facilitiesOptions[i].getEconomyScore(), EnvironmentScore + facilitiesOptions[i].getEnvironmentScore()));
        difference = maxScore - minScore;
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

//converts policy to a matching string
const string BalancedSelection::toString() const{
     return ("SelectionPolicy: bal");
}

//creats a copy of the current policy
BalancedSelection *BalancedSelection:: clone() const{
    return new BalancedSelection(*this);
};


