/*
 * File Created: Sun Mar 15 2020
 * Author:
 * SID:
*/

#include "Property.h"
#include "Employee.h"

void Property::writeToStream(ostream& os) const {
	Object::writeToStream(os);
	os << "Cost:" << getCost() << endl;
    os << "Upgrade Cost:" << getUpgradeCost() << endl;
    os << "Level:" << getLevel() << endl;
    os << "# of Employee:" << getNumEmployee() << " / " << getMaxNumEmployee() << endl;
}

void Property::upgrade() {
    m_level++;
}

void Property::getConstEmployeeList(const Employee**& employee) const {
	if (m_num_employee == 0) {
		employee =nullptr;
		return;
	}
	employee = new const Employee*[m_num_employee];
	for (int i = 0; i < m_num_employee; i++)
		employee[i] = m_employee_list[i];
}

// TODO: Start to implement your code.

//strange
Property::Property(int build_cost, int up_cost, int max_employee)
: m_cost(build_cost),m_upgrade_cost(up_cost),m_max_num_employee(max_employee){
}

//strange
Property::~Property(){
	if (m_employee_list == nullptr)
		return;
	else{
		for (int i = 0; i < m_num_employee; ++i){
			delete m_employee_list[i];
		}
		delete[] m_employee_list;
		m_employee_list = nullptr;
	}
}

int Property::getCost() const{
	return m_cost;
}

int Property::getUpgradeCost() const{
	return m_upgrade_cost;
}

int Property::getLevel() const{
	return m_level;
}

int Property::getNumEmployee() const{
	return m_num_employee;
}

int Property::getMaxNumEmployee() const{
	return m_max_num_employee;
}

void Property::setMaxNumEmployee(int max_employ){
	m_max_num_employee = max_employ;
}

bool isRange(Property* pro, const int s_x, const int s_y){
    int x, y, sz_x, sz_y;
    pro->getXY(x, y);
    pro->getSize(sz_x, sz_y);
    return s_x > x && s_x < x + sz_x - 1 && s_y > y && s_y < y + sz_y - 1;
}

//strange
bool Property::assignEmployee(Employee* employee){
	if (employee == nullptr)
		return false;
	if (m_num_employee == m_max_num_employee)
		return false;
	for (int i = 0; i < m_num_employee; ++i){
		if (m_employee_list[i] == employee)
			return false;
	}
	if (!checkEmployee(employee))
		return false;

	++m_num_employee;

	int x{0}, y{0};
	getXY(x,y);
	int sz_x{0}, sz_y{0};
	getSize(sz_x, sz_y);

	if (m_num_employee == 1){
		m_employee_list = new Employee*[1];
		m_employee_list[0] = employee;
	}
	else{
		Employee** temp = new Employee*[m_num_employee];
		for (int i = 0; i < m_num_employee; ++i){
			if (i == m_num_employee - 1)
				temp[i] = employee;
			else
				temp[i] = m_employee_list[i];
		}
		for (int i = 0; i < m_num_employee - 1; ++i){
			m_employee_list[i] = nullptr;
		}
		delete[] m_employee_list;
		m_employee_list = temp;
		temp = nullptr;
	}

	int count{m_num_employee};

	for (int p = x; p < y + sz_y; ++p){
		for (int q = y; q < x + sz_x; ++q){
			if(!isRange(this, p, q))
				continue;
			--count;
			if (count == 0)
				m_employee_list[m_num_employee - 1]->setXY(p, q);
		}
	}

	m_employee_list[m_num_employee - 1]->updateState();

	return true;
}

//strange
bool Property::fireEmployee(Employee* employee){
	if (employee == nullptr)
		return false;
	if (m_employee_list == nullptr)
		return false;
	if (m_num_employee == 1){
		m_num_employee = 0;
		delete m_employee_list[0];
		delete [] m_employee_list;
		m_employee_list = nullptr;
		return true;
	}

	int index{0}, prev_x{0}, prev_y{0}, x{0}, y{0};

	for (; index < m_num_employee; ++index){
		if (m_employee_list[index] == employee)
			break;
	}

	for (int i = 0; i < m_num_employee; ++i){
		if (i < index)
			continue;

		m_employee_list[i]->getXY(x, y);
		m_employee_list[i]->setXY(prev_x, prev_y);
		prev_x = x; prev_y = y;
	}

	Employee** temp = new Employee*[m_num_employee - 1];
	for (int i = 0; i < m_num_employee - 1; ++i){
		if (i < index)
			temp[i] = m_employee_list[i];
		else{
			temp[i] = m_employee_list[i + 1];
		}
	}

	delete  m_employee_list[index];
	for (int i = 0; i < m_num_employee; ++i){
		m_employee_list[i] = nullptr;
	}

	delete[] m_employee_list;
	m_employee_list = temp;
	temp = nullptr;

	--m_num_employee;

	return true;
}
