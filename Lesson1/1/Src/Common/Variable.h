#pragma once

namespace ee
{
	template <class Type> class Variable;
	template <class Type> class VariableClamp;

	typedef Variable<int>			VariableInt;
	typedef Variable<float>			VariableFloat;
	typedef Variable<std::wstring>	VariableString;
	typedef VariableClamp<int>		VariableClampInt;
	typedef VariableClamp<float>	VariableClampFloat;

	class MyConsole
	{
	public:
		MyConsole();
		virtual ~MyConsole();

		int				IsVariable(const wchar_t* name) const;

		void			SetInt(const wchar_t* name, int value);
		int				GetInt(const wchar_t* name) const;

		void			SetFloat(const wchar_t* name, float value);
		float			GetFloat(const wchar_t* name) const;

		void			SetString(const wchar_t* name, const wchar_t* value);
		const wchar_t*	GetString(const wchar_t* name) const;

	private:
		template <class Type> friend class Variable;

		int			initialized;

		struct Var
		{
			std::wstring	desc;						
			VariableInt*	int_v;
			VariableFloat*	float_v;
			VariableString*	string_v;
		};

		int			CheckName(const wchar_t* name) const;
		Var*		GetVariable(const wchar_t* name) const;
		void		AddVariable(const wchar_t* name, const wchar_t* desc, VariableInt* variable);
		void		AddVariable(const wchar_t* name, const wchar_t* desc, VariableFloat* variable);
		void		AddVariable(const wchar_t* name, const wchar_t* desc, VariableString* variable);
		void		RemoveVariable(const wchar_t* name);

		std::map<std::wstring, Var> variables;			
	};

	class VariableBase 
	{
	public:
		virtual ~VariableBase() { }

		static int				Init(MyConsole* console);
		static int				Shutdown();

	protected:
		virtual void			AddVariable() = 0;
		virtual void			RemoveVariable() = 0;
		virtual void			ClearVariable() = 0;

		VariableBase*			next;
		static VariableBase*	root;
		static MyConsole*		console;
	};

	template <class Type>
	class Variable : public VariableBase 
	{
	public:
		Variable(const wchar_t* name, const wchar_t* desc, Type value) : name(name), desc(desc), value(value) 
		{
			next = root; 
			root = this;
		}

		Type&					operator=(const Type& v) { Set(v); return value; }
		operator const Type&	() const { return value; }

		virtual void			Set(Type v) { value = v; }
		virtual const Type&		Get() const { return value; }
		virtual Type&			Get() { return value; }

		virtual int				IsClamped() const { return 0; }

	protected:
		virtual void AddVariable()
		{
			console->AddVariable(name, desc, this);
		}
		
		virtual void RemoveVariable()
		{
			console->RemoveVariable(name);
		}
		
		virtual void ClearVariable()
		{
			value.~Type();
		}

		const wchar_t*	name;	
		const wchar_t*	desc;	
		Type			value;	
	};

	template <class Type>
	class VariableClamp : public Variable<Type> 
	{
	public:
		VariableClamp(const wchar_t* name, const wchar_t* desc, Type value, Type vmin, Type vmax) : Variable<Type>(name, desc, value), varmin(vmin), varmax(vmax) { }

		Type&					operator=(const Type &v) { Set(v); return Variable<Type>::value; }
		operator const Type&	() const { return Variable<Type>::value; }

		virtual void			Set(Type v) { Variable<Type>::value = Clamp(v, varmin, varmax); }
		virtual const Type&		Get() const { return Variable<Type>::value; }
		virtual Type&			Get() { return Variable<Type>::value; }

		virtual int				IsClamped() const { return 1; }

		const Type&				GetMin() const { return varmin; }
		const Type&				GetMax() const { return varmax; }

	private:
		Type varmin;	
		Type varmax;	
	};

}

