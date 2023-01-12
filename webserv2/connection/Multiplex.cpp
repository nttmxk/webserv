/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#include "Multiplex.hpp"

void
Multiplex::declareKqueue()
{
	_kq = kqueue();
	if (_kq == -1)
	{
		std::cerr << "kqueue() error : ";
		exit(1); // make it throw later
	}
}

void
Multiplex::enrollEventToChangeList(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void* udata)
{
	struct kevent event;

	// EV_SET(&event, ident, filter, flags, fflags, data, udata); //ident = socketFd
	EV_SET(&event, ident, filter, flags, fflags, data, udata);
	_changeList.push_back(event);
}

time_t
Multiplex::now_plus_n(int n)
{
	time_t later = time(NULL);

	return later + n;
}

void
Multiplex::add_abs_timer(int n)
{
	struct kevent kev;

	EV_SET(&kev, n * 100000, EVFILT_TIMER, EV_ADD, NOTE_SECONDS | NOTE_ABSOLUTE, now_plus_n(n), (void *)n);
	_changeList.push_back(kev);
}


int
Multiplex::senseEvents()
{
	int sensedEvents;

	sensedEvents = kevent(_kq, &_changeList[0], _changeList.size(), _eventList, MAX_NUM_EVENTS, NULL); // SENSE NEW_EVENTS(연결 감지)
	if (sensedEvents == -1)
	{
		std::cerr << "kevent() error :";
		exit(1); // make it throw later
	}
	return (sensedEvents);
}

void
Multiplex::clearChangeList()
{
	_changeList.clear();
}

struct kevent const *
Multiplex::getEventList() const
{
	return (_eventList);
}
