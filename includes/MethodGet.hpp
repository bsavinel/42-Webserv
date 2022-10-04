#ifndef METHODGEST_HPP
# define METHODGEST_HPP

# include "HttpManager.hpp"

class MethodGet: public HttpManager
{
	public:
		MethodGet();
		~MethodGet();

		void	applyMethod();

	private:
		
};

#endif
