#ifndef METHODDELETE_HPP
# define METHODDELETE_HPP

#include "HttpManager.hpp"

class MethodDelete: public HttpManager
{
	public:
		MethodDelete();
		~MethodDelete();

		void	applyMethod();

	private:
};

#endif