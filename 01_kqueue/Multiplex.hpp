/***************************************************/
/* CODED BY JIBANG ================================*/
/***************************************************/

#ifndef MULTIPLEX_HPP
#define MULTIPLEX_HPP

#include "cppLibrary.hpp"
#include "Define.hpp"

/***************************************/
/* Classes managed by Connection class */
/***************************************/

/* Multiplex *///-----------------------
// class Multiplex;

// void
// Multiplex::declareKqueue();
// void
// Multiplex::enrollEventToChangeList(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void* udata);
// int
// Multiplex::senseEvents();
// void
// Multiplex::clearChangeList();
// struct kevent const *
// Multiplex::getEventList() const;

class Multiplex
{
	private:
		int _kq;
		std::vector<struct kevent> _changeList;
		struct kevent _eventList[MAX_NUM_EVENTS];

	public:
		void declareKqueue();
		void enrollEventToChangeList(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void* udata);
		int senseEvents();
		void clearChangeList();
		struct kevent const *getEventList() const;
};

#endif
