/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#ifndef MULTIPLEX_HPP
#define MULTIPLEX_HPP

#include "../includes/cppLibrary.hpp"
#include "../includes/Define.hpp"
#include <assert.h>

/***************************************/
/* Classes managed by Connection class */
/***************************************/

/* Multiplex *///-----------------------
class Multiplex
{
	private:
		int _kq;
		std::vector<struct kevent> _changeList;
		struct kevent _eventList[MAX_NUM_EVENTS];

	public:
		void declareKqueue();
		void enrollEventToChangeList(uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, intptr_t data, void* udata);
		time_t now_plus_n(int n);
		void add_abs_timer(int n);
		int senseEvents();
		void clearChangeList();
		struct kevent const *getEventList() const;
};

#endif
