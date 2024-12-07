
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Simulation.h"
#include "Auxiliary.h"
#include "Action.h"
#include <fstream>
using std::string;
using std::vector;

//constructor
Simulation:: Simulation(const string &configFilePath):
    isRunning(false),
    planCounter(0),
    actionsLog({}),
    plans({}),
    settlements({}),
    facilitiesOptions({})
{
    simulationBackup = nullptr;
    std::ifstream file(configFilePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file!" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Parse the line using Auxiliary::parseArguments
        std::vector<std::string> args = Auxiliary::parseArguments(line);

        if (args.empty()) continue; // Skip if parsing failed

        // Determine the type of entry and process accordingly
        if (args[0] == "settlement") {
            std::string name = args[1];
            SettlementType type = static_cast<SettlementType>(std::stoi(args[2]));
            Settlement *newSettlement = new Settlement(name, type);
            Simulation::addSettlement(newSettlement);
            
        } else if (args[0] == "facility") {
            std::string name = args[1];
            FacilityCategory category = static_cast<FacilityCategory>(std::stoi(args[2]));
            int price = std::stoi(args[3]);
            int lifeQualityScore = std::stoi(args[4]);
            int ecoScore = std::stoi(args[5]);
            int envScore = std::stoi(args[6]);
            FacilityType type(name, category, price, lifeQualityScore, ecoScore, envScore);
            Simulation::addFacility(type);

        } else if (args[0] == "plan") {

            // settlement of the plan
            //Settlement settlementPlan =  getSettlement(args[1]);

            // policy of the plan
            Simulation::addPlan(Simulation::getSettlement(args[1]), StringToPolicy(args[2], planCounter));
        }        
    }
    file.close();
};

//creates diffferent actions according to use input
void Simulation::start(){
    open();
    std::cout << "The simulation has started\n";
    //recieving input as long as the isRunnig flag is true
    while(isRunning){
        std::string userInput;
        std::cout << "Enter a command\n";
        std::getline(std::cin, userInput);
        vector<string> commands = Auxiliary::parseArguments(userInput);

        //creates a new "AddPlan" action, activate it and adds it to the actionLog
        if(commands[0] == "plan"){
            const string settlementName = commands[1];
            const string policy = commands[2];
            AddPlan *toAdd = new AddPlan(settlementName, policy);
            toAdd->setUserInput(userInput);
            toAdd->act(*this);
            addAction(toAdd);
        }

        //creates a new "AddSettlement" action, activate it and adds it to the actionLog
        else if(commands[0] == "settlement"){
            const string settlementName = commands[1];
            SettlementType type = static_cast<SettlementType>(std::stoi(commands[2]));
            AddSettlement *toAdd = new AddSettlement(settlementName, type);
            toAdd->setUserInput(userInput);
            toAdd->act(*this);
            addAction(toAdd);
        }

        //creates a new "AddFacility" action, activate it and adds it to the actionLog
        else if(commands[0] == "facility"){
            FacilityCategory category = static_cast<FacilityCategory>(std::stoi(commands[2]));
            AddFacility *toAdd = new AddFacility(commands[1], category, std::stoi(commands[3]), std::stoi(commands[4]), std::stoi(commands[5]), std::stoi(commands[6]));
            toAdd->setUserInput(userInput);
            toAdd->act(*this);
            addAction(toAdd);
        }

        //creates a new "SimulateStep" action, activate it and adds it to the actionLog
        else if(commands[0] == "step"){
            SimulateStep *toAdd = new SimulateStep(std::stoi(commands[1]));
            toAdd->setUserInput(userInput);
            toAdd->act(*this);
            addAction(toAdd);
        }

        //creates a new "ChangePlanPolicy" action, activate it and adds it to the actionLog
        else if(commands[0] == "changePolicy"){
            ChangePlanPolicy *toAdd = new ChangePlanPolicy(std::stoi(commands[1]), commands[2]);
            toAdd->setUserInput(userInput);
            toAdd->act(*this);
            addAction(toAdd);
        }

        //creates a new "PrintPlanStatus" action, activate it and adds it to the actionLog
        else if(commands[0] == "planStatus"){
            PrintPlanStatus *toAdd = new PrintPlanStatus(std::stoi(commands[1]));
            toAdd->setUserInput(userInput);
            toAdd->act(*this);
            addAction(toAdd);

        }

        //creates a new "BackupSimulation" action, activate it and adds it to the actionLo
        else if(commands[0] == "backup"){
            BackupSimulation *toAdd = new BackupSimulation();
            toAdd->setUserInput(userInput);
            toAdd->act(*this);
            addAction(toAdd);
        }

        //creates a new "PrintActionLog" action, activate it and adds it to the actionLo
        else if(commands[0] == "log"){
            PrintActionsLog *toAdd = new PrintActionsLog();
            toAdd->setUserInput(userInput);
            toAdd->act(*this);
            addAction(toAdd);

        }

        //creates a new "RestoreSimulation" action, activate it and adds it to the actionLo
        else if(commands[0] == "restore"){
            RestoreSimulation *toAdd = new RestoreSimulation();
            toAdd->setUserInput(userInput);
            toAdd->act(*this);
            addAction(toAdd);

        }

        //creates a new "Close" action, activate it and adds it to the actionLo
        else if(commands[0] == "close"){
            Close *toAdd = new Close();
            toAdd->setUserInput(userInput);
            toAdd->act(*this);
            addAction(toAdd);
        }
    }
}

//adds a new plan to the plans vector, updates the planCounter
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    Plan newPlan(planCounter, settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(newPlan);
    planCounter++;
}

//adds an action to the actionLog
void Simulation::addAction(BaseAction *action){
   actionsLog.push_back(action); 
}

//adds a new settlement to the settlements vector
//returns false if the settlement already exists, and true if it was added succesfully
bool Simulation::addSettlement(Settlement *settlement)
{
    string settlementName = settlement->getName();
    bool exists = isSettlementExists(settlementName);
    if (exists == true){
        delete settlement;
        return false;
    }
    else{
        settlements.push_back(settlement);
        return true;
    }
}

//adds a new facility to the facilitiesOptions vector
//returns false if the facility already exists, and true if it was added succesfully
bool Simulation::addFacility(FacilityType facility){
    for(size_t i=0; i < facilitiesOptions.size(); i++){
        if (facilitiesOptions[i].getName() == facility.getName()){
            return false;
        }
    }
    facilitiesOptions.push_back(facility);
    return true;
}

//checks if a settlement exists according to its name
bool Simulation::isSettlementExists(const string &settlementName){
    bool isFound = false;
        for(size_t i=0; i < settlements.size() && isFound == false; i++){
            if (settlements[i]->getName() == settlementName){
                isFound = true;
            }
        }
    return isFound;
}

//getter for a settlement according to its name
Settlement &Simulation::getSettlement(const string &settlementName) {
    for(size_t i=0; i < settlements.size(); i++){
        if (settlementName == settlements[i]->getName()){
            return *(settlements[i]);
        }
//של נגה
        // if (settlementName == settlements[i]->getName()){
        //     Settlement *toReturn = new Settlement(settlementName, settlements[i]->getType());
        //     return *toReturn;
        // }

    }
    Settlement *invalidSettlement = nullptr;
    return *invalidSettlement;
}

//getter for a plan according to plan ID
Plan &Simulation::getPlan(const int planID)
{
    return plans[planID];
}

//convert string to matching policy
SelectionPolicy *Simulation::StringToPolicy(const string &policy, int planId)
{
    NaiveSelection *chosenPolicy = nullptr;
    if (policy == "nve"){
        NaiveSelection *chosenPolicy = new NaiveSelection();
        return chosenPolicy;
    }
    else if (policy == "eco"){
        EconomySelection *chosenPolicy = new EconomySelection();
        return chosenPolicy;
    }
    else if (policy == "env"){
        SustainabilitySelection *chosenPolicy = new SustainabilitySelection();
        return chosenPolicy;
    }
    else if (policy == "bal"){
        Plan currPlan = getPlan(planId);
        BalancedSelection *chosenPolicy= new BalancedSelection(currPlan.getlifeQualityScore(), currPlan.getEconomyScore(), currPlan.getEnvironmentScore());
        return chosenPolicy;
    }
    return chosenPolicy;
}

//getter for number of plans
int Simulation::getPlanCounter()
{
    return planCounter;
}

//getter for vactionsLog
vector<BaseAction*> Simulation::getActionsLog()
{
    return actionsLog;
}

//calls the "step" method in each of the plans
void Simulation::step()
{
    for(size_t i=0; i < plans.size(); i++){
        plans[i].step();
    }
}

//changes the isRunning flag to false
void Simulation::close(){
    string output = "";

    for(Plan plan : plans){
        output += plan.toString();
    }
    isRunning = false;

    std::cout << output << std::endl;
}

//changes the isRunning flag to open
void Simulation::open(){
    isRunning = true;
}

//copy constructor
Simulation::Simulation(const Simulation &other)
    :isRunning(other.isRunning),
    planCounter(other.planCounter),
    actionsLog(),
    plans(),
    settlements(),
    facilitiesOptions(other.facilitiesOptions)
{
    for(size_t i = 0; i < other.actionsLog.size(); i++){
        actionsLog.push_back(other.actionsLog[i]->clone()); 
    }

    for(size_t i = 0; i < other.settlements.size(); i++){
        settlements.push_back(new Settlement(*other.settlements[i]));
    }

    for (size_t i = 0; i < other.plans.size(); i++){
        string settlemantName = plans[i].getSettlement().getName();
        Settlement newSettlamant = getSettlement(settlemantName);
        Plan newPlan = Plan(plans[i].getPlanId(), newSettlamant, plans[i].getSelectionPolicy(), facilitiesOptions);
        plans.push_back(newPlan);
    }
}

//distractor
Simulation::~Simulation()
{
    for(size_t i = 0; i < actionsLog.size(); i++){
        delete actionsLog[i];
    } 
    actionsLog.clear();

    for(size_t i = 0; i < settlements.size(); i++){
        delete settlements[i];
    } 
    settlements.clear();

    plans.clear();
    facilitiesOptions.clear();
}

//move constructor
Simulation::Simulation(Simulation &&other)
    :isRunning(other.isRunning),
    planCounter(other.planCounter),
    actionsLog(std::move(other.actionsLog)),
    plans(std::move(other.plans)),
    settlements(std::move(other.settlements)),
    facilitiesOptions(std::move(other.facilitiesOptions))
{}

//move assignment
Simulation& Simulation::operator=(Simulation &&other)
{
    if (this != &other){
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        this->actionsLog = std::move(other.actionsLog);
        this->plans = std::move(other.plans);
        this->settlements = std::move(other.settlements);
        this->facilitiesOptions = std::move(other.facilitiesOptions);

    }
    return *this;
}

//copy assignment
Simulation& Simulation::operator=(const Simulation &other)
{
    if(this != &other){
        isRunning = other.isRunning;
        planCounter = other.planCounter;
       // facilitiesOptions = other.facilitiesOptions;
    facilitiesOptions.clear();
    for(size_t i=0; i<other.facilitiesOptions.size(); i++){
        facilitiesOptions.push_back(other.facilitiesOptions[i]);
    }

        plans.clear();
        for(size_t i = 0; i < this->plans.size(); i++){
            this->plans.push_back( other.plans[i]);
        } 

        for(size_t i = 0; i < actionsLog.size(); i++){
            delete actionsLog[i];
        }
        actionsLog.clear();

        for(size_t i = 0; i < other.actionsLog.size(); i++){
            actionsLog.push_back(other.actionsLog[i]->clone());
        }

        for(size_t i = 0; i < settlements.size(); i++){
            delete settlements[i];
        }

        settlements.clear();
        for(size_t i = 0; i < other.settlements.size(); i++){
            settlements.push_back(new Settlement(other.settlements[i]->getName(), other.settlements[i]->getType()));
        }
    }
    return *this;
}
