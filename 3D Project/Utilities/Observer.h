#pragma once

#include <vector>
#include <memory>
class Observer
{
public:
	virtual void Notify() = 0;
};


class Subject
{
public:
	void RegisterObserver(Observer*);
	void NotifyObserver();

private:
	std::vector<Observer*> Observers;
};