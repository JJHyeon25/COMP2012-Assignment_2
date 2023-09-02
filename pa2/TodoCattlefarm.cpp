/*
 * File Created: Sun Mar 15 2020
 * Author:
 * SID:
*/

#include "TodoCattlefarm.h"
#include "Employee.h"
#include "TodoCow.h"
#include <cstdlib>

// Cattlefarm cost 80 upgrade cost 16
// Cattlefarm size 6, 6
// Cattlefarm init max num. of employee is CATTLEFARM_MAX_NUM_EMPLOYEE_MIN
// Cattlefarm Upgrade:
//      level increases by 1;
//      max num. of employees increases by 1 until CATTLEFARM_MAX_NUM_EMPLOYEE_MAX
// Cattlefarm Makemoney:
//      min(num. of cows, num. of feeders in work) x level x 10;

const int CATTLEFARM_COST = 80;
const int CATTLEFARM_UPGRADE_COST = 16;
const int CATTLEFARM_SIZE_X = 6; const int CATTLEFARM_SIZE_Y = 6;
const int CATTLEFARM_MAX_NUM_EMPLOYEE_MIN = 6;
const int CATTLEFARM_MAX_NUM_EMPLOYEE_MAX = (CATTLEFARM_SIZE_X-2) * (CATTLEFARM_SIZE_Y-2);

// TODO: Start to implement your code.

//strange
Cattlefarm::Cattlefarm(int x, int y)
:Property(CATTLEFARM_COST, CATTLEFARM_UPGRADE_COST, CATTLEFARM_MAX_NUM_EMPLOYEE_MIN){
	setSize(CATTLEFARM_SIZE_X, CATTLEFARM_SIZE_Y);
	setXY(x, y);
}

//strange
bool Cattlefarm::checkEmployee(Employee* employee) const{
	if (employee == nullptr)
		return false;
	if (employee->getSymbol() == 'd' || employee->getSymbol() == 'c'){
		return true;
	}
	else
		return false;
}

void Cattlefarm::upgrade(){
	if(getMaxNumEmployee() < CATTLEFARM_MAX_NUM_EMPLOYEE_MAX){
		int i = getMaxNumEmployee();
		setMaxNumEmployee(++i);
	}
	Property::upgrade();
}

//strange
int Cattlefarm::makeMoney() const{
	const Employee** temp = nullptr;
	getConstEmployeeList(temp);

	int num_feeder{0}, num_cow{0};

	for (int i = 0; i < getNumEmployee(); ++i){
		if ((temp[i]->getSymbol() == 'd') && (temp[i]->getState() == ObjectState::WORK))
			++num_feeder;
		if ((temp[i]->getSymbol() == 'c') && (temp[i]->getState() == ObjectState::WORK))
			++num_cow;
	}
	int money = min(num_cow, num_feeder) * getLevel() * 10;

	for (int i = 0; i < getNumEmployee(); i++){
		temp[i] = nullptr;
	}
	delete[] temp;
	temp = nullptr;

	return money;
}

//strange
void Cattlefarm::removeDiedCow(){
	const Employee** temp = nullptr;
	getConstEmployeeList(temp);

	if (temp == nullptr)
		return;

	int thresh = getNumEmployee();
	for (int i = 0; i < thresh; ++i){
		if (temp[i]->getSymbol() == 'c'){
			Cow* a = dynamic_cast<Cow*>(const_cast<Employee*>(temp[i]));
			if (!a->isAlive())
				fireEmployee(a);
		}
	}

	for (int i = 0; i < thresh; ++i){
		temp[i] = nullptr;
	}
	delete[] temp;
	temp = nullptr;
}

string Cattlefarm::getName() const{
	return "Cattlefarm";
}

char Cattlefarm::getSymbol() const{
	return 'C';
}
