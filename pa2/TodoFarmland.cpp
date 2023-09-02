/*
 * File Created: Sun Mar 15 2020
 * Author:
 * SID:
*/

#include "TodoFarmland.h"
#include "Employee.h"
#include <cstdlib>

// Farmland cost 50 upgrade cost 10
// Farmland size 5, 5
// Farmland init max num. of employee is FARMLAND_MAX_NUM_EMPLOYEE_MIN
// Farmland Upgrade:
//      level increases by 1;
//      max num. of employee increases by 1 until FARMLAND_MAX_NUM_EMPLOYEE_MAX
// Farmland Makemoney:
//      getRandInt(0, num. of employees in work x 2 + num. of farmers in work x 6 + level x 3)

const int FARMLAND_COST = 50;
const int FARMLAND_UPGRADE_COST = 10;
const int FARMLAND_SIZE_X = 5; const int FARMLAND_SIZE_Y = 5;
const int FARMLAND_MAX_NUM_EMPLOYEE_MIN = 3;
const int FARMLAND_MAX_NUM_EMPLOYEE_MAX = (FARMLAND_SIZE_X-2) * (FARMLAND_SIZE_Y-2);

#ifdef __HARVEST_MOON_DEBUG__
    inline void debug_getRandInt(string s) {cout << s << endl;};
#else
    inline void debug_getRandInt(string s) {};
#endif

//return random int in [low, high).
int getRandInt(int low, int high) {
    int res = rand() % high + low;
    debug_getRandInt("getRandInt: low is "+to_string(low));
    debug_getRandInt("getRandInt: high is "+to_string(high));
    debug_getRandInt("getRandInt: res is "+to_string(res));
    return res;
}

// TODO: Start to implement your code.

//strange
Farmland::Farmland(int x, int y)
:Property(FARMLAND_COST, FARMLAND_UPGRADE_COST, FARMLAND_MAX_NUM_EMPLOYEE_MIN){
	setSize(FARMLAND_SIZE_X, FARMLAND_SIZE_Y);
	setXY(x, y);
}

//strange
bool Farmland::checkEmployee(Employee* employee) const{
	if (employee == nullptr)
		return false;
	if (employee->getSymbol() == 'e' || employee->getSymbol() == 'r'){
		return true;
	}
	else
		return false;
}

void Farmland::upgrade(){
	if(getMaxNumEmployee() < FARMLAND_MAX_NUM_EMPLOYEE_MAX){
		int i = getMaxNumEmployee();
		setMaxNumEmployee(++i);
	}
	Property::upgrade();
}

//strange
int Farmland::makeMoney() const{
	const Employee** temp = nullptr;
	getConstEmployeeList(temp);

	int num_employee{0}, num_farmer{0};

	for (int i = 0; i < getNumEmployee(); ++i){
		if ((temp[i]->getSymbol() == 'e') && (temp[i]->getState() == ObjectState::WORK))
			++num_employee;
		if ((temp[i]->getSymbol() == 'r') && (temp[i]->getState() == ObjectState::WORK))
			++num_farmer;
	}
	int money = getRandInt(0, num_employee * 2 + num_farmer * 6 + getLevel() * 3);

	for (int i = 0; i < getNumEmployee(); i++){
		temp[i] = nullptr;
	}
	delete[] temp;
	temp = nullptr;

	return money;
}

string Farmland::getName() const{
	return "Farmland";
}

char Farmland::getSymbol() const{
	return 'R';
}
