#include "RPN.hpp"

RPN::RPN(void)
{
	std::cout << "Default constructor is useless because the class needs arguments to work" << std::endl;
}

RPN::RPN(std::string *args)
{
	this->_perform_rpn(args);
}

RPN::RPN(const RPN & other)
{
	this->_stack = other.getStack();
}

RPN::~RPN(void)
{
}

RPN & RPN::operator=(const RPN & other)
{
	this->_stack = other.getStack();
	return (*this);
}

std::stack<int>	RPN::getStack(void) const
{
	return (this->_stack);
}

bool	RPN::_perform_op(std::string sign)
{
	int	nb_a;
	int	nb_b;

	//faire les operations selon le signe
	nb_a = this->_stack.top();
	this->_stack.pop();
	nb_b = this->_stack.top();
	this->_stack.pop();
	if (sign == "+")
		this->_stack.push(nb_b + nb_a);
	else if (sign == "-")
		this->_stack.push(nb_b - nb_a);
	else if (sign == "/")
	{
		if (nb_a == 0)
		{
			std::cout << "Division by 0" << std::endl;
			return false;
		}
		this->_stack.push(nb_b / nb_a);
	}
	else if (sign == "*")
		this->_stack.push(nb_b * nb_a);
	return (true);
}

void	RPN::_perform_rpn(std::string *args)
{
	int	i = 0;
	int	nb;

	for (; args[i] != ""; ++i)
	{
		if (isdigit(args[i][0]) == 1)
		{
			nb = std::atoi(args[i].c_str());
			this->_stack.push(nb);
		}
		else
		{
			if (this->_stack.size() < 2)
			{
				std::cout << "Invalid format" << std::endl;
				return ;
			}
			if (this->_perform_op(args[i]) == false)
				return ;
		}
	}
	if (this->_stack.size() != 1)
	{
		std::cout << "Invalid format" << std::endl;
		return ;
	}
	std::cout << this->_stack.top() << std::endl;
}
