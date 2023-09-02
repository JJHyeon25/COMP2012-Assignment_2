/*
 * File Created: Mon Mar 16 2020
 * Author:
 * SID:
*/

#include "Scene.h"
#include "Property.h"
#include "TodoFarmland.h"
#include "TodoCattlefarm.h"
#include "Employee.h"
#include "TodoFarmer.h"
#include "TodoFeeder.h"
#include "TodoCow.h"

const int SCENE_W = 45;
const int SCENE_H = 10;
const int PROPERTY_X = 35;
const int SCENE_INIT_MONEY = 200;

Scene::Scene()
: m_objects(nullptr), m_num_objects(0), m_money(SCENE_INIT_MONEY) {
    nextRound();
}

Scene::~Scene() {
    //shallowremove employees
    for(int i = m_num_objects-1; i >= 0; i--) {
        if (m_objects[i]->getObjectType()==ObjectType::EMPLOYEE)
            shallowRemoveEmployee(dynamic_cast<Employee*>(m_objects[i]));
    }
    // remove properties
    for(int i = m_num_objects-1; i >= 0; i--) {
        if (m_objects[i]->getObjectType()==ObjectType::PROPERTY)
            delete m_objects[i];
    }
    delete[] m_objects;
}

void Scene::shallowRemoveEmployee(Employee* e) {
    if (m_num_objects <= 0) return;
    if (m_num_objects == 1 && e == m_objects[0]) {
        m_num_objects = 0;
        delete [] m_objects;
        m_objects = nullptr;
        return;
    }
    int i;
	for (i = 0; i < m_num_objects; i++) {
		if (m_objects[i] == e)
			break;
	}
    Object ** newObjects = new Object*[--m_num_objects];
	for (int j = 0; j < m_num_objects; j++)
		newObjects[j] = m_objects[j];
	if (m_num_objects != i)
		newObjects[i] = m_objects[m_num_objects];
    delete [] m_objects;
    m_objects = newObjects;
}

bool Scene::checkOverlap(const Property* newproperty) {
    int x, y, sz_x, sz_y;
    newproperty->getXY(x, y);
    newproperty->getSize(sz_x, sz_y);
    for(int xx=x; xx<x+sz_x; xx++)
        for(int yy=y; yy<y+sz_y; yy++)
            if(getObjectAt(xx, yy) != nullptr) return true;
    return false;
}


void Scene::addProperty(int property, int x, int y) {
    Property* newProperty = nullptr;
    switch(property) {
    case FARMLAND:
        newProperty = new Farmland(x,y);
        break;
    case CATTLEFARM:
        newProperty = new Cattlefarm(x, y);
        break;
    }
    if (newProperty == nullptr) return;
    if (newProperty->getCost() > m_money || checkOverlap(newProperty)) {
        delete newProperty;
        return;
    }
    addObject(newProperty);
    m_money -= newProperty->getCost();
}


bool Scene::hire(Property* p, int employee) {
    Employee* newEmployee = nullptr;
    switch(employee){
    case EMPLOYEE:
        newEmployee = new Employee();
        break;
    case FARMER:
        newEmployee = new Farmer();
        break;
    case FEEDER:
        newEmployee = new Feeder();
        break;
    case COW:
        newEmployee = new Cow();
        break;
    }
    if (newEmployee == nullptr) return false;
    if (newEmployee->getCost() > m_money || ! p->assignEmployee(newEmployee)) {
        delete newEmployee;
        return false;
    }
    addObject(newEmployee);
    m_money -= newEmployee->getCost();
    return true;
}


Object* Scene::getObjectAt(int s_x, int s_y) const {
	int x, y, sz_x, sz_y;
    // If employee is at s_x, s_y, get employee
    // else, get property
    // otherwise return null
	for (int i = 0; i < m_num_objects; i++) {
        if (m_objects[i]->getObjectType() == ObjectType::PROPERTY)
            continue;
		m_objects[i]->getXY(x,y);
        m_objects[i]->getSize(sz_x,sz_y);
        if ( s_x >= x && s_x < x + sz_x && s_y >= y && s_y < y + sz_y)
			return m_objects[i];
	}
	for (int i = 0; i < m_num_objects; i++) {
        if (m_objects[i]->getObjectType() == ObjectType::EMPLOYEE)
            continue;
		m_objects[i]->getXY(x,y);
        m_objects[i]->getSize(sz_x,sz_y);
        if ( s_x >= x && s_x < x + sz_x && s_y >= y && s_y < y + sz_y)
			return m_objects[i];
	}
	return nullptr;
}

//perform shallow copying
void Scene::addObject(Object* newobj) {
	Object** newobjects = new Object*[m_num_objects + 1];
	for (int i = 0; i < m_num_objects; i++)
		newobjects[i] = m_objects[i];
	newobjects[m_num_objects++] = newobj;
	if (m_num_objects != 1)
		delete [] m_objects;
	m_objects = newobjects;
}

void Scene::getConstObjects(const Object**& obj, int& count) const {
	count = m_num_objects;
	if (m_num_objects == 0) {
		obj =nullptr;
		return;
	}
	obj = new const Object*[m_num_objects];
	for (int i = 0; i < m_num_objects; i++)
		obj[i] = m_objects[i];
}

// TODO: Start to implement your code.

bool Scene::isGameOver() const{
	if (getMoney() < 0){
		return true;
	}

	return false;
}

void Scene::removeProperty(Property* gone){
	if (gone == nullptr)
		return;

	if (m_num_objects <= 0)
		return;

	if (m_num_objects == 1 && (m_objects[0] == gone)){
		delete gone;
		delete [] m_objects;
		m_num_objects = 0;
	    m_objects = nullptr;
	    return;
	}

	const Employee** temp = nullptr;
	gone->getConstEmployeeList(temp);

	for (int i = 0; i < gone->getNumEmployee(); ++i){
		Employee* a = const_cast<Employee*>(temp[i]);
		shallowRemoveEmployee(a);
	}

	for (int k = 0; k < gone->getNumEmployee(); ++k){
		temp[k] = nullptr;
	}
	delete[] temp;
	temp = nullptr;

	int i {0};
	for (; i < m_num_objects; i++) {
		if (m_objects[i] == gone)
			break;
	}

	Object ** newObjects = new Object*[--m_num_objects];
	for (int j = 0; j < m_num_objects; ++j)
		newObjects[j] = m_objects[j];

	if (m_num_objects != i)
		newObjects[i] = m_objects[m_num_objects];

	delete m_objects[i];
	m_objects[i] = nullptr;
	delete [] m_objects;

	m_objects = newObjects;

	newObjects = nullptr;
}

void Scene::nextRound(){
	int dead_cow{0};

	for (int i = 0; i < m_num_objects; i++) {
		switch(m_objects[i]->getSymbol()){
			case 'C':
				m_money += dynamic_cast<Cattlefarm*>(m_objects[i])->makeMoney();
				break;
			case 'R':
				m_money += dynamic_cast<Farmland*>(m_objects[i])->makeMoney();
				break;
		}
	}

	for (int i = 0; i < m_num_objects; i++) {
		switch(m_objects[i]->getSymbol()){
		case 'e':
			m_money -= dynamic_cast<Employee*>(m_objects[i])->getSalary();
			dynamic_cast<Employee*>(m_objects[i])->updateWorkAge();
			dynamic_cast<Employee*>(m_objects[i])->updateState();
			break;
		case 'r':
			m_money -= dynamic_cast<Farmer*>(m_objects[i])->getSalary();
			dynamic_cast<Farmer*>(m_objects[i])->updateWorkAge();
			dynamic_cast<Farmer*>(m_objects[i])->updateState();
			break;
		case 'c':
			m_money -= dynamic_cast<Cow*>(m_objects[i])->getSalary();
			dynamic_cast<Cow*>(m_objects[i])->updateWorkAge();
			dynamic_cast<Cow*>(m_objects[i])->updateState();
			if (!dynamic_cast<Cow*>(m_objects[i])->isAlive()){
				m_objects[i] = nullptr;
				++dead_cow;
			}
			break;
		case 'd':
			m_money -= dynamic_cast<Feeder*>(m_objects[i])->getSalary();
			dynamic_cast<Feeder*>(m_objects[i])->updateWorkAge();
			dynamic_cast<Feeder*>(m_objects[i])->updateState();
			break;
		}
	}

	for (int i = 0; i < m_num_objects; i++) {
		switch(m_objects[i]->getSymbol()){
			case 'C':
				dynamic_cast<Cattlefarm*>(m_objects[i])->removeDiedCow();
				break;
		}
	}

	int count {0};
	m_num_objects -= dead_cow;
	Object ** newObjects = new Object*[m_num_objects];
	for (int j = 0; j < m_num_objects; j++)
		newObjects[j] = m_objects[j];

	for (int j = 0; j < m_num_objects; ++j){
		if (newObjects[j] == nullptr){
			for (; dead_cow > 0 ; --dead_cow, ++count)
				if ((m_objects[m_num_objects + dead_cow - 1] != nullptr)){
					newObjects[j] = m_objects[m_num_objects + dead_cow - 1];
					--dead_cow;
					++count;
					break;
				}
		}
	}

	for (int i = 0; i < m_num_objects + count; ++i){
		m_objects[i] = nullptr;
	}
	delete [] m_objects;
	m_objects = newObjects;
	newObjects = nullptr;
}

bool Scene::upgrade(Property* up){
	if (up == nullptr)
		return false;

	if (m_money > up->getUpgradeCost()){
		up->upgrade();
		m_money -= up->getUpgradeCost();
		return true;
	}

	return false;
}



bool Scene::fire(Employee* fired){
	if (fired == nullptr)
		return false;

	if (m_num_objects <= 0)
		return false;

	if (m_num_objects == 1 && (m_objects[0] == fired)){
		delete fired;
		delete [] m_objects;
		m_num_objects = 0;
		m_objects = nullptr;
		return true;
	}

	int index{0};
	for (; index < m_num_objects; ++index) {
		if (m_objects[index] == fired)
		break;
	}

	int x{0}, y{0}, count{0};
	dynamic_cast<Employee*>(m_objects[index])->getXY(x,y);

	for (int i = 0; i < m_num_objects; ++i){
		if (m_objects[i]->getObjectType() != ObjectType::EMPLOYEE){
			if (dynamic_cast<Property*>(m_objects[i])->isInRange(x,y)){
				if(dynamic_cast<Property*>(m_objects[i])->fireEmployee(dynamic_cast<Employee*>(m_objects[index]))){
					m_objects[index] = nullptr;
					++count;
					break;
				}
			}
		}
	}

	if (count == 0)
		return false;

	Object ** newObjects = new Object*[--m_num_objects];
	for (int j = 0; j < m_num_objects + 1; ++j){
		if (j < index)
			newObjects[j] = m_objects[j];
		else if(j == index)
			continue;
		else
			newObjects[j - 1] = m_objects[j];
	}

	for (int i = 0; i < m_num_objects + 1; ++i){
		m_objects[i] = nullptr;
	}
	delete [] m_objects;
	m_objects = newObjects;
	newObjects = nullptr;

	return true;
}
