#ifndef PMERGEME_HPP
#define PMERGEME_HPP
#include <vector>
#include <deque>
#include <iostream>
#include <cctype>
#include <sstream>
#include <ctime>
#include <cstdlib>

template <typename T>
class PmergeMe
{

public:

	//constructors/destructors
	PmergeMe(void);
	PmergeMe(const std::string *nbs, int size);
	PmergeMe(const PmergeMe & other);
	~PmergeMe(void);

	//operator overload
	PmergeMe & operator=(const PmergeMe & other);

	//getters
	T getElem(void) const;


private:
 
 	//attributes
	T	_elem;

	//methods
	void	_merge_insert_sort(T & elem) const;
 	void	_merge_sort(T & elem_a, T & elem_b, T & elem_merge) const;
 	void	_insert_sort(T & elem) const;

};

template <typename T>
PmergeMe<T>::PmergeMe(void)
{
	this->_elem.push_back(1);
	std::cout << "Default constructor is useless because the program needs a list of numbers to sort" << std::endl;
}

template <typename T>
PmergeMe<T>::~PmergeMe(void)
{
}

template <typename T>
PmergeMe<T>::PmergeMe(const std::string *nbs, int size)
{
	for (int i = 0; i < size; ++i)
		this->_elem.push_back(std::atoi(nbs[i].c_str()));
	this->_merge_insert_sort(this->_elem);
}

template <typename T>
PmergeMe<T>::PmergeMe(const PmergeMe & other)
{
	this->_elem = other.getElem();
}

template <typename T>
PmergeMe<T> & PmergeMe<T>::operator=(const PmergeMe & other)
{
	this->_elem = other.getElem();
	return (*this);
}

template <typename T>
T PmergeMe<T>::getElem(void) const
{
	return (this->_elem);
}

template <typename T>
void	PmergeMe<T>::_merge_insert_sort(T & elem) const
{
	T		elem_a;
	T		elem_b;
	size_t	size = elem.size();
	size_t	i = 0;

	if (elem.size() <= 20)
	{
		this->_insert_sort(elem);
		return ;
	}
	for ( ; i < size / 2 || elem.empty(); ++i)
	{
		elem_a.push_back(elem.front());
		elem.erase(elem.begin());
	}
	for ( ; i < size; ++i)
	{
		elem_b.push_back(elem.front());
		elem.erase(elem.begin());
	}
	this->_merge_insert_sort(elem_a);
	this->_merge_insert_sort(elem_b);
	this->_merge_sort(elem_a, elem_b, elem);
}

template <typename T>
void	PmergeMe<T>::_merge_sort(T & elem_a, T & elem_b, T & elem_merge) const
{
	size_t	size = elem_a.size() + elem_b.size();

	for (size_t i = 0; i < size; ++i)
	{
		if (elem_a.empty())
		{
			for ( ; !elem_b.empty(); )
			{
				elem_merge.push_back(elem_b.front());
				elem_b.erase(elem_b.begin());
			}
			return ;
		}
		if (elem_b.empty())
		{
			for ( ; !elem_a.empty(); )
			{
				elem_merge.push_back(elem_a.front());
				elem_a.erase(elem_a.begin());
			}
			return ;
		}
		if (elem_a.front() > elem_b.front())
		{
			elem_merge.push_back(elem_b.front());
			elem_b.erase(elem_b.begin());
		}
		else
		{
			elem_merge.push_back(elem_a.front());
			elem_a.erase(elem_a.begin());
		}
	}
}

template <typename T>
void	PmergeMe<T>::_insert_sort(T & elem) const
{
	int		tmp;
	size_t	j;

	for (size_t i = 1; i < elem.size(); ++i)
	{
		j = i;
		for ( ; j > 0 && elem[j] < elem[j - 1]; --j)
		{
			tmp = elem[j - 1];
			elem[j - 1] = elem[j];
			elem[j] = tmp;
		}
	}
}

#endif
