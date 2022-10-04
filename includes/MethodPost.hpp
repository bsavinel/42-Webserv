#ifndef METHODPOST_HPP
# define METHODPOST_HPP

#include "HttpManager.hpp"

class MethodPost: public HttpManager
{
	public:
		MethodPost();
		~MethodPost();

		void	applyMethod();
	private:
};

#endif
