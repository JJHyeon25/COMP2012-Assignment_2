/*
 * File Created: Sun Mar 15 2020
 * Author:
 * SID:
*/

#ifndef FARMLAND_H_
#define FARMLAND_H_

#include "Property.h"
extern const int FARMLAND_SIZE_X;
extern const int FARMLAND_SIZE_Y;
// TODO: Start to implement your code.

class Farmland : public Property{
	public:
	Farmland(int, int);
	bool checkEmployee(Employee*) const override;
	void upgrade();
	int makeMoney() const override;
	string getName() const override;
	char getSymbol() const override;
};

#endif /*FARMLAND_H_*/
