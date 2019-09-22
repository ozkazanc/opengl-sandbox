#include "Observer.h"

void Subject::AddObserver(Observer* observer){
	observer->m_Next = m_Head;
	m_Head = observer;
	if (observer->m_Next != nullptr)
		observer->m_Next->m_Prev = observer;
}

void Subject::RemoveObserver(Observer* observer){

	if (observer->m_Prev == nullptr) {
		m_Head = observer->m_Next;
		if (observer->m_Next != nullptr)
			observer->m_Next->m_Prev = nullptr;
	}
	
	else{
		observer->m_Prev->m_Next = observer->m_Next;
		if (observer->m_Next != nullptr)
			observer->m_Next->m_Prev = observer->m_Prev;
	}
	observer->m_Next = nullptr;
	observer->m_Prev = nullptr;
}

void Subject::Notify(int event_) const{
	Observer* current = m_Head;
	while (current != nullptr){
		current->OnNotify(event_);
		current = current->m_Next;
	}
}
void Subject::Notify(float event_, float Yevent, bool scroll) const {
	Observer* current = m_Head;
	while (current != nullptr) {
		current->OnNotify(event_, Yevent, scroll);
		current = current->m_Next;
	}
}