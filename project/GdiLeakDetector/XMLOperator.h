#ifndef _XMLOPERATOR_H
#define _XMLOPERATOR_H
#include "Struct_Define.h"
#include "Markup.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

class xmloperator
{
public:
	xmloperator();
	void AddHandleType(PHANDLE_INFORMATION p);
	void AddHandleValue(PHANDLE_INFORMATION p);
	void AddParameterinfo(PHANDLE_INFORMATION p);
	void AddRegisters(unsigned long p,char *name);
	void AddCallStacks(PHANDLE_INFORMATION p);
	void AddModules(PHANDLE_INFORMATION p);
	void Add(PHANDLE_INFORMATION p);
	void Createxml();
	void SetFileName(string filename);
	void Start(string name);
	void End(string name);
	void Read(PHANDLE_INFORMATION p);
	void Find_All_Data( string &Tag, string &Value);
	vector<string> Split(const string& s,const string& match,bool removeEmpty, bool fullMatch);
	void ShowHandle(string Handle,string &test);
	void Find_Data( string &Tag, string &Value);
	string Show_Data(PHANDLE_INFORMATION p);
	string test;
	std::vector<std::string> Stack;
protected:
	CMarkup  *xml;
	string filename;
	
private:
	
};


#endif