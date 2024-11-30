#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Simulation.h"
using std::string;
using std::vector;

Simulation:: Simulation(const string &configFilePath){};


void Simulation::start(){};


void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){};


void Simulation::addAction(BaseAction *action){};


bool Simulation::addSettlement(Settlement settlement){};


bool Simulation::addFacility(FacilityType facility){};


bool Simulation::isSettlementExists(const string &settlementName){};


Settlement Simulation::&getSettlement(const string &settlementName){};


Plan Simulation::&getPlan(const int planID){};


void Simulation::step(){};


void Simulation::close(){};


void Simulation::open(){};