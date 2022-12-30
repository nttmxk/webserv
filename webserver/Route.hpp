/***************************************************/
/* CODED BY JIN H. BANG ===========================*/
/***************************************************/

#ifndef ROUTE_HPP
#define ROUTE_HPP

#include "Define.hpp"
#include "cppLibrary.hpp"

class Route
{
	private:
		std::string _requestUrl;
	public:
		Route();
		~Route();
		bool urlValidator(std::string _requestUrl);

};

Route::urlValidator(std::string _requestUrl)
{

}


#endif
