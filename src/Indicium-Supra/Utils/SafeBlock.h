#pragma once
#include <type_traits>
#include <functional>
#include <memory>

template<class Executer, typename ...T, typename Ret = std::result_of<Executer(T...)>::type>
Ret safeExecute(Executer executer, T&&... args)
{
	try 
	{
		return executer(args...);
	}
	catch (...)
	{
		return Ret();
	}
}

template<class Executer, typename ...T>
bool safeExecuteWithValidation(Executer executer, T&&... args)
{
	try
	{
		executer(args...);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

template<typename T, typename ...Args>
std::shared_ptr<T> safeMakeShared(Args&&... p)
{
	try 
	{
		return std::make_shared<T>(p...);
	}
	catch (...)
	{
		return nullptr;
	}
}