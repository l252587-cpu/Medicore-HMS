//#include<iostream>
#include "Person.h"
//#include<cstring>

//using namespace std;

person::person()
{
	ID = 0;
	name = " ";
	password = " ";
}
person::person(int id,string name,string password)
{
	this->ID = id;
	this->name = name;
	this->password = password;
}
int person::getid() {
	return ID;

}
string person::getpassword()
{
	return password;
}
string person::getname()
{
	return name;
}

