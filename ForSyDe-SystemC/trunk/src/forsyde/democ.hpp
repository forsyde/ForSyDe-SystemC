/**********************************************************************
    * de.hpp -- The discrete-event model of computation               *
    *                                                                 *
    * Authors: Mikkel Koefoed Jakobsen (mkoe@imm.dtu.dk)              *
    *                                                                 *
    * Purpose: Providing promitive element required for modeling      *
    *          discrete-evnent systems in ForSyDe-SystemC             *
    *                                                                 *
    * Usage:   This file is included automatically                    *
    *                                                                 *
    * License: BSD3                                                   *
    *******************************************************************/

#ifndef DEMOC_HPP
#define DEMOC_HPP


#include <vector>

using namespace sc_core;

namespace ForSyDe {
namespace DiscreteEvent {


template <class T> class Event;
template <class T> class Signal;

template <class T>
std::string toString(const Event<T> & e) {
	std::stringstream s;
	s << "(" << e.time << "," << e.value << ")";
	return s.str();
}

template <class T>
std::string toString(const std::vector<Event<T> > & es) {
	std::stringstream s;
	typename std::vector<Event<T> >::iterator i;
	if (es.empty()) {
		s << "[]";
	} else {
		s << "[" << toString<T>(es.front());
		i = ++(es.begin());
		for (; i != es.end(); ++i) {
			s << "," << toString<T>(*i);
		}
		s << "]";
	}
	return s.str();
}

template <class T>
std::string toString(Signal<T> & signal) {
	return toString<T>(signal.events);
}

} /* namespace DiscreteEvent */
} /* namespace ForSyDe */


#include <deque>

namespace ForSyDe {
namespace DiscreteEvent {

template <typename O>
struct tuple {
	tuple(const sc_core::sc_time & time, const O value) : time(time), value(value) {}

	sc_core::sc_time time;
	O value;
};

template <class O1>
class Out1 : public sc_core::sc_module {
public:
	sc_core::sc_out<O1> o1;
	
	Out1(const sc_core::sc_module_name & name, const sc_core::sc_time & delay1) : sc_core::sc_module(name) , delay1(delay1) {
		SC_METHOD(delay1_method);
		dont_initialize();
		sensitive << action1;
	}

protected:
	void delayOutput(const O1 & o1) {
		this->f1.push_back(tuple<O1>(sc_core::sc_time_stamp() + this->delay1,o1));
		this->next_event();
	}

private:
	std::deque<tuple<O1> > f1;
	sc_core::sc_event action1;
	sc_core::sc_time delay1;
	
	SC_HAS_PROCESS(Out1);
	
	void delay1_method() {
		if (!f1.empty()) {
			o1 = f1.front().value;
			f1.pop_front();
			this->next_event();
		}
	}

	void next_event() {
		if (!f1.empty()) {
			const sc_core::sc_time t1 = f1.front().time;
			this->action1.notify(t1 - sc_core::sc_time_stamp());
		}
	}
};

template <class O1,class O2>
class Out2 : public sc_core::sc_module {
public:
	sc_core::sc_out<O1> o1;
	sc_core::sc_out<O2> o2;
	
	Out2(const sc_core::sc_module_name & name, const sc_core::sc_time & delay1, const sc_core::sc_time & delay2) : sc_core::sc_module(name) , delay1(delay1) , delay2(delay2) {
		SC_METHOD(delay1_method);
		dont_initialize();
		sensitive << action1;
		SC_METHOD(delay2_method);
		dont_initialize();
		sensitive << action2;
	}
	
protected:
	void delayOutput(const O1 & o1, const O2 & o2) {
		this->f1.push_back(tuple<O1>(sc_core::sc_time_stamp() + this->delay1,o1));
		this->next_event1();
		this->f2.push_back(tuple<O2>(sc_core::sc_time_stamp() + this->delay2,o2));
		this->next_event2();
	}
	
private:
	std::deque<tuple<O1> > f1;
	std::deque<tuple<O2> > f2;
	sc_core::sc_event action1, action2;
	sc_core::sc_time delay1, delay2;
	
	SC_HAS_PROCESS(Out2);
	
	void delay1_method() {
		if (!f1.empty()) {
			o1 = f1.front().value;
			f1.pop_front();
			this->next_event1();
		}
	}
	
	void delay2_method() {
		if (!f2.empty()) {
			o2 = f2.front().value;
			f2.pop_front();
			this->next_event2();
		}
	}

	void next_event1() {
		if (!f1.empty()) {
			const sc_core::sc_time t1 = f1.front().time;
			this->action1.notify(t1 - sc_core::sc_time_stamp());
		}
	}

	void next_event2() {
		if (!f2.empty()) {
			const sc_core::sc_time t2 = f2.front().time;
			this->action2.notify(t2 - sc_core::sc_time_stamp());
		}
	}
};

template <class O1,class O2, class O3>
class Out3 : public sc_core::sc_module {
public:
	sc_core::sc_out<O1> o1;
	sc_core::sc_out<O2> o2;
	sc_core::sc_out<O3> o3;
	
	Out3(const sc_core::sc_module_name & name, const sc_core::sc_time & delay1, const sc_core::sc_time & delay2, const sc_core::sc_time & delay3) : sc_core::sc_module(name) , delay1(delay1) , delay2(delay2) , delay3(delay3) {
		SC_METHOD(delay1_method);
		dont_initialize();
		sensitive << action1;
		SC_METHOD(delay2_method);
		dont_initialize();
		sensitive << action2;
		SC_METHOD(delay3_method);
		dont_initialize();
		sensitive << action3;
	}
	
protected:
	void delayOutput(const O1 & o1, const O2 & o2, const O3 & o3) {
		this->f1.push_back(tuple<O1>(sc_core::sc_time_stamp() + this->delay1,o1));
		this->next_event1();
		this->f2.push_back(tuple<O2>(sc_core::sc_time_stamp() + this->delay2,o2));
		this->next_event2();
		this->f3.push_back(tuple<O3>(sc_core::sc_time_stamp() + this->delay3,o3));
		this->next_event3();
	}
	
private:
	std::deque<tuple<O1> > f1;
	std::deque<tuple<O2> > f2;
	std::deque<tuple<O3> > f3;
	sc_core::sc_event action1, action2, action3;
	sc_core::sc_time delay1, delay2, delay3;
	
	SC_HAS_PROCESS(Out3);
	
	void delay1_method() {
		if (!f1.empty()) {
			o1 = f1.front().value;
			f1.pop_front();
			this->next_event1();
		}
	}
	
	void delay2_method() {
		if (!f2.empty()) {
			o2 = f2.front().value;
			f2.pop_front();
			this->next_event2();
		}
	}

	void delay3_method() {
		if (!f3.empty()) {
			o3 = f3.front().value;
			f3.pop_front();
			this->next_event3();
		}
	}

	void next_event1() {
		if (!f1.empty()) {
			const sc_core::sc_time t1 = f1.front().time;
			this->action1.notify(t1 - sc_core::sc_time_stamp());
		}
	}

	void next_event2() {
		if (!f2.empty()) {
			const sc_core::sc_time t2 = f2.front().time;
			this->action2.notify(t2 - sc_core::sc_time_stamp());
		}
	}

	void next_event3() {
		if (!f3.empty()) {
			const sc_core::sc_time t3 = f3.front().time;
			this->action3.notify(t3 - sc_core::sc_time_stamp());
		}
	}
};

template <class I1,class O1>
class Map : public Out1<O1> {
public:
	sc_core::sc_in<I1> i1;
	
	Map(const sc_core::sc_module_name & name, const sc_core::sc_time & delay) : Out1<O1>::Out1(name,delay) {
		SC_METHOD(method);
		sc_core::sc_module::dont_initialize();
		sc_core::sc_module::sensitive << i1 << start;
		this->start.notify(SC_ZERO_TIME);
	}
	
private:
	sc_core::sc_event start;
	
	SC_HAS_PROCESS(Map);

	void method() {
		this->delayOutput(this->func(i1));
	}

	virtual O1 func(const I1) const = 0;
};

template <class I1,class I2,class O1>
class Zip : public Out1<O1> {
public:
	sc_core::sc_in<I1> i1;
	sc_core::sc_in<I2> i2;
	
	Zip(const sc_core::sc_module_name & name, const sc_core::sc_time & delay) : Out1<O1>::Out1(name,delay) {
		SC_METHOD(method);
		sc_core::sc_module::dont_initialize();
		sc_core::sc_module::sensitive << i1 << i2 << start;
		this->start.notify(SC_ZERO_TIME);
	}
	
private:
	sc_core::sc_event start;
	
	SC_HAS_PROCESS(Zip);
	
	void method() {
		this->delayOutput(this->func(i1,i2));
	}

	virtual O1 func(const I1, const I2) const = 0;
};

template <class I1,class I2,class I3,class O1>
class Zip3 : public Out1<O1> {
public:
	sc_core::sc_in<I1> i1;
	sc_core::sc_in<I2> i2;
	sc_core::sc_in<I3> i3;
	
	Zip3(const sc_core::sc_module_name & name, const sc_core::sc_time & delay) : Out1<O1>::Out1(name,delay) {
		SC_METHOD(method);
		sc_core::sc_module::dont_initialize();
		sc_core::sc_module::sensitive << i1 << i2 << i3 << start;
		this->start.notify(SC_ZERO_TIME);
	}
	
private:
	sc_core::sc_event start;
	
	SC_HAS_PROCESS(Zip3);
	
	void method() {
		this->delayOutput(this->func(i1,i2,i3));
	}

	virtual O1 func(const I1, const I2, const I3) const = 0;
};

template <class I1,class I2,class I3,class I4,class O1>
class Zip4 : public Out1<O1> {
public:
	sc_core::sc_in<I1> i1;
	sc_core::sc_in<I2> i2;
	sc_core::sc_in<I3> i3;
	sc_core::sc_in<I4> i4;
	
	Zip4(const sc_core::sc_module_name & name, const sc_core::sc_time & delay) : Out1<O1>::Out1(name,delay) {
		SC_METHOD(method);
		sc_core::sc_module::dont_initialize();
		sc_core::sc_module::sensitive << i1 << i2 << i3 << i4 << start;
		this->start.notify(SC_ZERO_TIME);
	}
	
private:
	sc_core::sc_event start;
	
	SC_HAS_PROCESS(Zip4);
	
	void method() {
		this->delayOutput(this->func(i1,i2,i3,i4));
	}

	virtual O1 func(const I1, const I2, const I3, const I4) const = 0;
};

template <class T1>
class FanOut : public Out2<T1,T1> {
	sc_core::sc_in<T1> i1;
	
	FanOut(const sc_core::sc_module_name & name) : Out2<T1,T1>::Out2(name,SC_ZERO_TIME,SC_ZERO_TIME) {
		SC_METHOD(method);
		sc_core::sc_module::dont_initialize();
		sc_core::sc_module::sensitive << i1 << start;
		this->start.notify(SC_ZERO_TIME);
	}
	
private:
	sc_core::sc_event start;
	
	SC_HAS_PROCESS(FanOut);
	
	void method() {
		T1 tmp = i1;
		this->delayOutput(tmp,tmp);
	}
};

template <class T1>
class FanOut3 : public Out3<T1,T1,T1> {
	sc_core::sc_in<T1> i1;
	
	FanOut3(const sc_core::sc_module_name & name) : Out3<T1,T1,T1>::Out3(name,SC_ZERO_TIME,SC_ZERO_TIME,SC_ZERO_TIME) {
		SC_METHOD(method);
		sc_core::sc_module::dont_initialize();
		sc_core::sc_module::sensitive << i1 << start;
		this->start.notify(SC_ZERO_TIME);
	}
	
private:
	sc_core::sc_event start;
	
	SC_HAS_PROCESS(FanOut3);
	
	void method() {
		T1 tmp = i1;
		this->delayOutput(tmp,tmp,tmp);
	}
};

} /* namespace DiscreteEvent */
} /* namespace ForSyDe */


#include <deque>
#include <vector>

namespace ForSyDe {
namespace DiscreteEvent {

template <class T>
class Event {
public:
	Event() : time(sc_core::sc_time(0,SC_SEC)) , value() {};
	Event(const sc_core::sc_time & time, const T & value) : time(time), value(value) {}
	
	sc_core::sc_time time;
	T value;
};

template <class T>
class Signal {
	friend std::string toString<T>(Signal<T> &);
public:
	typedef std::vector<Event<T> > EventList_t;
	
	Signal() : events(), now(0,SC_SEC), last(), index(0) {}
	
	Signal(const EventList_t & events) : events(events), now(0,SC_SEC), last(), index(0) {}
	
	void append(const sc_core::sc_time & time, const T & value) {
		this->events.push_back(Event<T>(time,value));
	}
	
	const Event<T> next() {
		if (this->index != this->events.size()) {
			Event<T> head = this->events[this->index];
			++this->index;
			Event<T> result(head.time - now,head.value);
			now = head.time;
			last = head;
			return result;
		} else {
			return last;
		}
	}
	
	bool has_next() const {
		return (this->index != this->events.size());
	}
private:
	EventList_t events;
	sc_core::sc_time now;
	Event<T> last;
	typename EventList_t::size_type index;
};

} /* namespace DiscreteEvent */
} /* namespace ForSyDe */



namespace ForSyDe {
namespace DiscreteEvent {

template <class T>
class Driver : public sc_core::sc_module {
public:
	sc_core::sc_out<T> out;
	
	Driver(const sc_core::sc_module_name & name, const Signal<T> & signal) : sc_core::sc_module(name), signal(signal) {
		SC_THREAD(driver);
	}
private:
	SC_HAS_PROCESS(Driver);
	
	Signal<T> signal;
	
	void driver() {
		while (this->signal.has_next()) {
			Event<T> event = this->signal.next();
			wait(event.time);
			out = event.value;
		}
	}
};

} /* namespace DiscreteEvent */
} /* namespace ForSyDe */

#endif /* DEMOC_H */

