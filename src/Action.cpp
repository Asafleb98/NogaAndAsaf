
#include <string>
#include <vector>
#include "Simulation.h"
#include "Action.h"
#include <iostream>
using std::string;

BaseAction::BaseAction()
{}

// getter for the action's status
ActionStatus BaseAction::getStatus() const
{
    return status;
}

//changes the action's status to "complete"
void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

//changes the action's status to "error" and prints an indication to the user
void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    std::cout << errorMsg << std::endl;
}

//new functions

//saves the user input in a variable
void BaseAction::setUserInput(const string &input)
{
    userInput = input;
}

//converts the plan status to a matching string
string BaseAction::statusToString(ActionStatus status) const
{
    switch (status) {
        case ActionStatus::COMPLETED:
            return "COMPLETED";
        case ActionStatus::ERROR:
            return "ERROR";
    }
}

//end of new functions

//SimulationStep
//constructor
SimulateStep::SimulateStep(const int numOfSteps):
    numOfSteps(numOfSteps)
{}

//advances the simulation by the input number of steps
void SimulateStep::act(Simulation &simulation)
{
    for(int i=1; i<=numOfSteps; i++){
        simulation.step();
    }
    complete();
}

//converts the action to a matching string
const string SimulateStep::toString() const
{
    string currStatus = statusToString(getStatus());
    return (userInput + " " + currStatus + "\n");
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

//AddPlan
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):
    settlementName(settlementName),
    selectionPolicy(selectionPolicy)
{
    errorMsg = "Cannot create this plan";
}

void AddPlan::act(Simulation &simulation)
{
    SelectionPolicy *chosenPolicy = simulation.StringToPolicy(selectionPolicy, simulation.getPlanCounter());
    if (simulation.isSettlementExists(settlementName) == false || chosenPolicy == nullptr){
        delete chosenPolicy;
        error(errorMsg);
    }
    else{
        simulation.addPlan(simulation.getSettlement(settlementName), chosenPolicy);
        complete();  
    }
}

//converts the action to a matching string
const string AddPlan::toString() const
{
    string currStatus = statusToString(getStatus());
    return (userInput + " " + currStatus + "\n");
}

AddPlan *AddPlan::clone() const
{
    return new AddPlan(*this);
}

//AddSettlement
AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType):
    settlementName(settlementName),
    settlementType(settlementType)
{
    errorMsg = "Settlement already exists";
}

void AddSettlement::act(Simulation &simulation)
{
    Settlement *toAdd = &Settlement(settlementName, settlementType);
    bool added = simulation.addSettlement(toAdd);
    if (added == true){
        complete();
    }
    else{
        error(errorMsg);
    } 
}

AddSettlement *AddSettlement::clone() const
{
    return new AddSettlement(*this);
}

//converts the action to a matching string
const string AddSettlement::toString() const
{
    string currStatus = statusToString(getStatus());
    return (userInput + " " + currStatus + "\n");
}

//AddFacility
AddFacility::AddFacility(const string & facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
    facilityName(facilityName),
    facilityCategory(facilityCategory),
    price(price),
    lifeQualityScore(lifeQualityScore),
    economyScore(economyScore),
    environmentScore(environmentScore)  
{
    errorMsg = "Facility already exists";
}

void AddFacility::act(Simulation &simulation)
{
    FacilityType toAdd(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore);
    bool added = simulation.addFacility(toAdd);
    if (added == false){
        error(errorMsg);
    }
    else{
        complete();
    }
}


AddFacility *AddFacility::clone() const
{
    return new AddFacility(*this);
}

//converts the action to a matching string
const string AddFacility::toString() const
{
    string currStatus = statusToString(getStatus());
    return (userInput + " " + currStatus + "\n");
}

//PrintPlanStatus
PrintPlanStatus::PrintPlanStatus(int planId):
    planId(planId)
{
    errorMsg = "Plan doesn't exist";
}

void PrintPlanStatus::act(Simulation &simulation)
{
    int numOfPlans = simulation.getPlanCounter();
    if (planId < 0 || planId >= numOfPlans){
        error(errorMsg);
    }
    else{
        simulation.getPlan(planId).printStatus();
        complete();
    }
}

PrintPlanStatus *PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(*this);
}

//converts the action to a matching string
const string PrintPlanStatus::toString() const
{
    string currStatus = statusToString(getStatus());
    return (userInput + " " + currStatus + "\n");
}

//ChangePlanPolicy
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):
    planId(planId),
    newPolicy(newPolicy)
{
    errorMsg = "Cannot change selection policy";
}

void ChangePlanPolicy::act(Simulation &simulation)
{
    Plan toChange = simulation.getPlan(planId);
    SelectionPolicy *chosenPolicy = simulation.StringToPolicy(newPolicy, planId);
    if(chosenPolicy == nullptr || chosenPolicy->toString() == toChange.getSelectionPolicy()->toString()){
        delete chosenPolicy;
        error(errorMsg);
    }
    else{
        toChange.setSelectionPolicy(chosenPolicy);
        complete();
    }
}

ChangePlanPolicy *ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this);
}

//converts the action to a matching string
const string ChangePlanPolicy::toString() const
{
    string currStatus = statusToString(getStatus());
    return (userInput + " " + currStatus + "\n");
}

//PrintActionLog
PrintActionsLog::PrintActionsLog()
{}

void PrintActionsLog::act(Simulation &simulation)
{
    vector<BaseAction*> actionsLog = simulation.getActionsLog();
    for(size_t i = 0; i < actionsLog.size(); i++){
        std::cout << actionsLog[i]->toString();
    }
    complete();
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

//converts the action to a matching string
const string PrintActionsLog::toString() const
{
    string currStatus = statusToString(getStatus());
    return (userInput + " " + currStatus + "\n");
}

//Close
Close::Close()
{}

void Close::act(Simulation &simulation)
{
    simulation.close();
    complete();
}

Close *Close::clone() const
{
    return new Close(*this);
}

//converts the action to a matching string
const string Close::toString() const
{
    string currStatus = statusToString(getStatus());
    return (userInput + " " + currStatus + "\n");
}

//BackupSimulation
BackupSimulation::BackupSimulation()
{}

void BackupSimulation::act(Simulation &simulation)
{
    if(simulation.simulationBackup == nullptr){
        Simulation *backupSimulation = new Simulation(simulation);
    }
    else{
        simulation.simulationBackup = &simulation;
    }
    complete();
}

BackupSimulation *BackupSimulation::clone() const
{
    return new BackupSimulation(*this);
}

//converts the action to a matching string
const string BackupSimulation::toString() const
{
    string currStatus = statusToString(getStatus());
    return (userInput + " " + currStatus + "\n");
}

//RestoreSimulation
RestoreSimulation::RestoreSimulation()
{
    errorMsg = "No backup avaliable";
}

void RestoreSimulation::act(Simulation &simulation)
{
    if(simulation.simulationBackup == nullptr){
        error(errorMsg);
    }
    else{
        simulation = *simulation.simulationBackup;
    }
}

RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation(*this);
}

//converts the action to a matching string
const string RestoreSimulation::toString() const
{
    string currStatus = statusToString(getStatus());
    return (userInput + " " + currStatus + "\n");
}
