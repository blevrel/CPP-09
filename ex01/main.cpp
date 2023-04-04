#include <iostream>
#include <cctype>
#include "RPN.hpp"

bool	is_token(char c)
{
	if (c == '+' || c == '-' || c == '/' || c == '*')
		return (true);
	return (false);
}

int	count_nb_args(std::string args)
{
	size_t	start = 0;
	size_t	end = args.find(' ', start);
	int		count = 0;

	if (isspace(args[0]) == 0)
		count++;
	while (end != std::string::npos)
	{
		if (args[end + 1] != '\0' && isspace(args[end + 1]) == 0)
			count++;
		start = end + 1;
		end = args.find(' ', start);
	}
	return (count);
}

std::string	*split_args(std::string args)
{
	if (args[0] == '\0')
	{
		std::cout << "Invalid arg" << std::endl;
		return (NULL);
	}

	int			count = count_nb_args(args);
	size_t		start = args.find_first_not_of(" \t\n\v\f\r", 0);
	size_t		end = args.find(' ', start);
	int			i = 0;
	std::string	*splitted_args = new std::string[count + 1];
	//pas oublier de delete

	if (!splitted_args)
	{
		std::cout << "[ERROR] : New failed" << std::endl;
		return (NULL);
	}
	while (end != std::string::npos)
	{
		splitted_args[i] = args.substr(start, end - start);
		i++;
		start = args.find_first_not_of(" \t\n\v\f\r", end + 1);
		end = args.find(' ', start);
	}
	if (i != count && isspace(args[start + 1]) == 0)
		splitted_args[i] = args.substr(start, end);
	return (splitted_args);
}

bool	check_args(std::string *args)
{
	for (int i = 0; args[i] != ""; ++i)
	{
		if (args[i].size() != 1 || (isdigit(args[i][0]) == 0 && is_token(args[i][0]) == false))
		{
			std::cout << "[ERROR] : Invalid arg" << std::endl;
			return (false);
		}
	}
	return (true);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "[ERROR] : Usage: <\"operations\">" << std::endl;
		return (-1);
	}
	std::string	args = argv[1];
	std::string	*splitted_args = split_args(args);
	if (!splitted_args)
		return (-1);
	if (check_args(splitted_args) == false)
	{
		delete [] splitted_args;
		return (-1);
	}
	RPN	rpn(splitted_args);
	delete [] splitted_args;
	return (0);
}
