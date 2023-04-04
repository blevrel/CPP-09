#include "PmergeMe.hpp"

bool	str_is_pos_numeric(std::string str)
{
	std::string::iterator it = str.begin();
	for(; it != str.end(); ++it)
	{
		if (isdigit(*it) == 0)
			return (false);
	}
	return (true);
}

bool	check_duplicates(std::string *nbs, int size)
{
	int	j;

	for (int i = 0; i < size; ++i)
	{
		j = 0;
		for (; j < size; ++j)
		{
			if (i != j && nbs[i] == nbs[j])
				return (false);
		}
	}
	return (true);
}

bool	check_params(std::string *nbs, int size)
{
	int	ovf_test;

	for (int i = 0; i < size; ++i)
	{
		if (nbs[i].size() < 1)
			return (false);
		if (str_is_pos_numeric(nbs[i]) == false)
			return (false);
		if (check_duplicates(nbs, size) == false)
			return (false);
		if (!(std::stringstream(nbs[i]) >> ovf_test))
			return (false);
	}
	return (true);
}

int	main(int argc, char **argv)
{
	clock_t start_time;
	clock_t end_time;
	double elapsed_time;

	if (argc < 2)
	{
		std::cout << "[ERROR] : Usage : ./PmergeMe <numbers>,[...]" << std::endl;
		return (-1);
	}
	std::string *nbs = new std::string[argc - 1];
	for (int i = 0; i < argc - 1; ++i)
		nbs[i] = argv[i + 1];
	if (check_params(nbs, argc - 1) == false)
	{
		std::cout << "[ERROR] : Invalid args" << std::endl;
		delete [] nbs;
		return (-1);
	}
	std::cout << "Before:	";
	for (int i = 0; i < argc - 1; ++i)
	{
		std::cout << nbs[i];
		if (i + 1 == argc - 1)
			std::cout << std::endl;
		else
			std::cout << " ";
	}
	start_time = clock();
	PmergeMe<std::vector<int> >	to_sort_vec(nbs, argc - 1);
	end_time = clock();
	std::cout << "After:	";
	for (int i = 0; i < argc - 1; ++i)
	{
		std::cout << to_sort_vec.getElem()[i];
		if (i + 1 == argc - 1)
			std::cout << std::endl;
		else
			std::cout << " ";
	}
	elapsed_time = end_time - start_time;
	std::cout << "Time to process a range of " << argc - 1 << " elements with std::vector : " << elapsed_time / 1000 << " ms" << std::endl;
	start_time = clock();
	PmergeMe<std::deque<int> >	to_sort_deq(nbs, argc - 1);
	end_time = clock();
	elapsed_time = end_time - start_time;
	std::cout << "Time to process a range of " << argc - 1 << " elements with std::deque : " << elapsed_time / 1000 << " ms" << std::endl;
	delete [] nbs;
	return (0);
}
