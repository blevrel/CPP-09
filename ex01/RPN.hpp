#ifndef RPN_HPP
#define RPN_HPP
#include <stack>
#include <cstdlib>
#include <iostream>

class RPN
{

public:

	RPN(void);
	RPN(std::string *args);
	RPN(const RPN & other);
	~RPN(void);
	RPN & operator=(const RPN & other);

	std::stack<int>	getStack(void) const;

private:

	std::stack<int>	_stack;
	bool			_perform_op(std::string sign);
	void			_perform_rpn(std::string *args);

};

#endif
