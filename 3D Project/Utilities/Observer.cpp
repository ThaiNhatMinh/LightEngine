#include "pch.h"
#include "Observer.h"

void Subject::RegisterObserver(Observer*pObserver)
{
	Observers.push_back(pObserver);
}

void Subject::NotifyObserver()
{
	for (auto& el : Observers) el->Notify();
}
