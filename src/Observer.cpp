#include "Observer.h"

void Subject::AddObserver(Observer* observer){
	observer->m_Next = m_Head;
	m_Head = observer;
	if (observer->m_Next != nullptr)
		observer->m_Next->m_Prev = observer;
}

void Subject::RemoveObserver(Observer* observer){
	if (observer->m_Prev != nullptr)
		observer->m_Prev->m_Next = observer->m_Next;
	
	if (observer->m_Next != nullptr)
		observer->m_Next->m_Prev = observer->m_Prev;
}

void Subject::Notify(int event_){
	Observer* current = m_Head;
	while (current != nullptr){
		current->OnNotify(event_);
		current = current->m_Next;
	}
}