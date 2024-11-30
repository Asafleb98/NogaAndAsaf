#include <iostream>
#include "Facility.h"

using namespace std;

int main() {
    // Create a FacilityType object
    FacilityType hospitalType("Hospital", FacilityCategory::LIFE_QUALITY, 100, 20, 5, 15);

    // Test FacilityType methods
    cout << "Testing FacilityType:" << endl;
    cout << "Name: " << hospitalType.getName() << endl;
    cout << "Category: " << static_cast<int>(hospitalType.getCategory()) << endl;
    cout << "Cost: " << hospitalType.getCost() << endl;
    cout << "Life Quality Score: " << hospitalType.getLifeQualityScore() << endl;
    cout << "Economy Score: " << hospitalType.getEconomyScore() << endl;
    cout << "Environment Score: " << hospitalType.getEnvironmentScore() << endl;
    cout << endl;

    // Create a Facility object using the first constructor
    Facility hospital("Hospital", "New York", FacilityCategory::LIFE_QUALITY, 100, 20, 5, 15);
    cout << "Testing Facility (Constructor 1):" << endl;
    cout << "Name: " << hospital.getName() << endl;
    cout << "Settlement: " << hospital.getSettlementName() << endl;
    cout << "Status: " << (hospital.getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS ? "Under Construction" : "Operational") << endl;
    cout << "Time Left: " << hospital.getTimeLeft() << endl;
    cout << endl;

    // Create a Facility object using the second constructor
    Facility hospital2(hospitalType, "Los Angeles");
    cout << "Testing Facility (Constructor 2):" << endl;
    cout << "Name: " << hospital2.getName() << endl;
    cout << "Settlement: " << hospital2.getSettlementName() << endl;
    cout << "Status: " << (hospital2.getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS ? "Under Construction" : "Operational") << endl;
    cout << "Time Left: " << hospital2.getTimeLeft() << endl;
    cout << endl;

    // Test the step method
    cout << "Simulating Construction Progress (step):" << endl;
    while (hospital2.getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS) {
        cout << "Time Left: " << hospital2.getTimeLeft() << ", Status: " 
             << (hospital2.getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS ? "Under Construction" : "Operational") << endl;
        hospital2.step();
    }

    cout << "Final Status: " << (hospital2.getStatus() == FacilityStatus::OPERATIONAL ? "Operational" : "Under Construction") << endl;
    cout << endl;

    return 0;
}
