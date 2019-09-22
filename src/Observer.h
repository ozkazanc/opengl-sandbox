#pragma once

class Observer{
	friend class Subject;

private:
	Observer* m_Next;
	Observer* m_Prev;

public:
	Observer()
		:m_Next(nullptr), m_Prev(nullptr) {}

	virtual ~Observer() {}

	virtual void OnNotify(int event_) = 0;
	virtual void OnNotify(float Xevent, float Yevent, bool scroll) = 0;
};

class Subject{
private:
	Observer* m_Head;

protected:
	void Notify(int event_) const;
	void Notify(float Xevent_, float Yevent, bool scroll) const;

public:
	Subject()
		:m_Head(nullptr) {}
	~Subject() {}

	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);
};
