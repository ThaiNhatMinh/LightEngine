#pragma once

class Delegate
{
public:
	template<class T,class F,class ...P>
	struct _mem_delegate
	{
		T* m_ObjPtr;
		F(T::*m_Func)(P ...);
		_mem_delegate(T* obj, F(T::*func)(P ...)) :m_ObjPtr(obj), m_Func(func) {}

		F operator()(P...p)
		{
			return (m_ObjPtr->*m_Func)(static_cast<P>(p)...);
		}
	};


public:
	template<class T, class F, class ...P>
	static _mem_delegate<T, F, P...> Make(T* p, F(T::*func)(P ...))
	{
		_mem_delegate<T, F, P ...> t(p, func);
		return t;
	}
};