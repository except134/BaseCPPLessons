#include "ExceptEnginePCH.h"

using namespace ee;

VariableBase* VariableBase::root	= nullptr;
MyConsole* VariableBase::console	= nullptr;

int VariableBase::Init(MyConsole* c) 
{
	assert(console == nullptr && "ConsoleVariableBase::init(): console variables is already initialized");

	console = c;

	VariableBase* variable = root;
	while (variable) {
		variable->AddVariable();
		variable = variable->next;
	}

	return 1;
}

int VariableBase::Shutdown() 
{
	assert(console && "ConsoleVariableBase::shutdown(): console variables is not initialized");

	console = nullptr;

	VariableBase* variable = root;
	while (variable) {
		variable->ClearVariable();
		variable = variable->next;
	}

	return 1;
}

MyConsole::MyConsole() : initialized(0) 
{
	VariableBase::Init(this);

	initialized = 1;
}

MyConsole::~MyConsole() 
{
	VariableBase::Shutdown();
}

int MyConsole::CheckName(const wchar_t* name) const 
{
	if (variables.find(name) != variables.end()) 
		return 0;
	return 1;
}

int MyConsole::IsVariable(const wchar_t* name) const 
{
	return (variables.find(name) != variables.end());
}

MyConsole::Var* MyConsole::GetVariable(const wchar_t* name) const 
{
	auto it = variables.find(name);
	if ( it == variables.end()) {
		LOG.Write(L"Variable system: can't find \"%s\" variable\n", name);
		return nullptr;
	}
	return (MyConsole::Var*)(&it->second);
}

void MyConsole::AddVariable(const wchar_t* name, const wchar_t* desc, VariableInt* variable) 
{
	if (CheckName(name) == 0) {
		LOG.Write(L"Variable system: \"%s\" name is already defined\n", name);
		return;
	}
	
	Var v;
	v.desc = desc;
	v.int_v = variable;
	v.float_v = nullptr;
	v.string_v = nullptr;
	variables.insert(std::make_pair(name, v));
}

void MyConsole::AddVariable(const wchar_t* name, const wchar_t* desc, VariableFloat* variable) 
{
	if (CheckName(name) == 0) {
		LOG.Write(L"Variable system: \"%s\" name is already defined\n", name);
		return;
	}

	Var v;
	v.desc = desc;
	v.int_v = nullptr;
	v.float_v = variable;
	v.string_v = nullptr;
	variables.insert(std::make_pair(name, v));
}

void MyConsole::AddVariable(const wchar_t* name, const wchar_t* desc, VariableString* variable) 
{
	if (CheckName(name) == 0) {
		LOG.Write(L"Variable system: \"%s\" name is already defined\n", name);
		return;
	}

	Var v;
	v.desc = desc;
	v.int_v = nullptr;
	v.float_v = nullptr;
	v.string_v = variable;
	variables.insert(std::make_pair(name, v));
}

void MyConsole::RemoveVariable(const wchar_t* name) 
{
	auto it = variables.find(name);
	if (it == variables.end()) 
		LOG.Write(L"Variable system: can't find \"%s\" variable\n", name);
	else 
		variables.erase(it);
}

void MyConsole::SetInt(const wchar_t* name, int value) 
{
	Var* v = GetVariable(name);
	if (v == nullptr || v->int_v == nullptr) 
		LOG.Write(L"Variable system: \"%s\" is not an int type variable\n", name);
	else 
		v->int_v->Set(value);
}

int MyConsole::GetInt(const wchar_t* name) const 
{
	Var* v = GetVariable(name);
	if (v == nullptr || v->int_v == nullptr) {
		LOG.Write(L"Variable system: \"%s\" is not an int type variable\n", name);
		return 0;
	}
	return v->int_v->Get();
}

void MyConsole::SetFloat(const wchar_t* name, float value) 
{
	Var* v = GetVariable(name);
	if (v == nullptr || v->float_v == nullptr) 
		LOG.Write(L"Variable system: \"%s\" is not float type variable\n", name);
	else 
		v->float_v->Set(value);
}

float MyConsole::GetFloat(const wchar_t* name) const 
{
	Var* v = GetVariable(name);
	if (v == nullptr || v->float_v == nullptr) {
		LOG.Write(L"Variable system: \"%s\" is not float type variable\n", name);
		return 0.0f;
	}
	return v->float_v->Get();
}

void MyConsole::SetString(const wchar_t* name, const wchar_t* value) 
{
	Var* v = GetVariable(name);
	if (v == nullptr || v->string_v == nullptr) 
		LOG.Write(L"Variable system: \"%s\" is not a string type variable\n", name);
	else 
		v->string_v->Set(std::wstring(value));
}

const wchar_t* MyConsole::GetString(const wchar_t* name) const 
{
	Var* v = GetVariable(name);
	if (v == nullptr || v->string_v == nullptr) {
		LOG.Write(L"Variable system: \"%s\" is not a string type variable\n", name);
		return L"";
	}
	return v->string_v->Get().c_str();
}

