#include "BitcoinExchange.hpp"

static bool	check_valid_date_format(std::string date, int trigger);
static bool	check_database_value(std::string date);
static bool	check_input_file_value(std::string value);
static int	get_nb_of_days_in_month(int month);
static bool	empty_line(std::string line);

BitcoinExchange::BitcoinExchange(void)
{
	std::cerr << "Default constructor is useless because the class needs arguments to work" << std::endl;
}

BitcoinExchange::BitcoinExchange(std::string filename)
{
	if (this->_parse_database("data.csv") == true)
		this->_parse_input_file(filename);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange & other)
{
	this->_database = other.getDatabase();
}

BitcoinExchange::~BitcoinExchange(void)
{
}

BitcoinExchange & BitcoinExchange::operator=(const BitcoinExchange & other)
{
	this->_database = other.getDatabase();
	return (*this);
}

const std::map<std::string, float>&	BitcoinExchange::getDatabase(void) const
{
	return (this->_database);
}

bool	BitcoinExchange::_parse_database(std::string filename)
{
	int				i = 0;
	size_t			start;
	size_t			end;
	float			price;
	std::string		line;
	std::string		date;
	std::string		price_str;
	std::ifstream	read_fd;
	std::pair<std::map<std::string, float>::iterator, bool> check;

	read_fd.open(filename.c_str(), std::fstream::in); //open file
	if (read_fd.fail() == true)
	{
		std::cerr << "Error: could not open database file" << std::endl;
		return (false);
	}
	for ( ; std::getline(read_fd, line); ) //get line in file
	{
		if (empty_line(line)) //skip empty lines
			continue ;
		else if (i == 0 && line != "date,exchange_rate") //check for precise first line of file
		{
			std::cerr << "Missing \"date,exchange_rate\" at the beginning of database" << std::endl;
			return (false);
		}
		else if (i == 0 && line == "date,exchange_rate") //accept correct first line
		{
			++i;
			continue ;
		}
		start = 0;
		end = line.find(',', start); //split date and value
		date = line.substr(start, end);
		start = end + 1;
		end = line.find('\n', start);
		price_str = line.substr(start, end);
		if (check_valid_date_format(date, 0) == false)
			return (false);
		if (check_database_value(price_str) == false)
			return (false);
		if (!(std::stringstream(price_str) >> price))
		{
			std::cerr << "Float overflow in database value" << std::endl;
			return (false);
		}
		check = this->_database.insert(std::pair<std::string, float>(date, price));
		if (!check.second)
		{
			std::cerr << "Duplicate value in database" << std::endl;
			return (false);
		}
		++i;
	}
	if (i == 0 && line == "")
	{
		std::cerr << "Empty database file" << std::endl;
		return (false);
	}
	if (i == 1)
	{
		std::cerr << "Database needs atleast one element as a reference" << std::endl;
		return (false);
	}
	return (true);
}

void	BitcoinExchange::_parse_input_file(std::string filename)
{
	int				i = 0;
	size_t			start;
	size_t			end;
	float			value;
	std::string		line;
	std::string		date;
	std::string		value_str;
	std::ifstream	read_fd;
	std::map<std::string, float>::const_iterator	it;

	read_fd.open(filename.c_str(), std::fstream::in); //open file
	if (read_fd.fail() == true)
	{
		std::cerr << "Error: could not open input file" << std::endl;
		return ;
	}
	for ( ; std::getline(read_fd, line); ) //get line in file
	{
		if (empty_line(line)) //skip empty lines
			continue ;
		else if (i == 0 && line != "date | value") //check for precise first line of file
		{
			std::cerr << "Missing \"date | value\" at the beginning of input file" << std::endl;
			return ;
		}
		else if (i == 0 && line == "date | value") //accept correct first line
		{
			++i;
			continue ;
		}
		++i;
		start = 0;
		end = line.find(" | ", start); //split date and value
		date = line.substr(start, end);
		start = end + 3;
		end = line.find('\n', start);
		value_str = line.substr(start, end);
		if (check_valid_date_format(date, 1) == false)
			continue ;
		if (check_input_file_value(value_str) == false)
			continue ;
		if (!(std::stringstream(value_str) >> value))
		{
			std::cerr << "Error : Float overflow" << std::endl;
			continue ;
		}
		it = getDatabase().lower_bound(date);
		if (it == getDatabase().begin() && it->first != date)
		{
			std::cerr << "Error : No data before " << date << std::endl;
			continue ;
		}
		if (it->first != date)
			--it;
		std::cout << date + " ==> " << it->second * value << std::endl;
	}
	if (i == 0 && line == "")
		std::cerr << "Empty input file" << std::endl;
	if (i == 1)
		std::cerr << "Input file needs atleast one element" << std::endl;
}

static bool	check_valid_date_format(std::string date, int trigger)
{
	struct tm 	check_format;
	int			year;
	int			month;
	int			day;
	bool		is_leap_year;
	size_t		start = 0;
	size_t		end = date.find('-', start);

	if (strptime(date.c_str(), "%Y-%m-%d", &check_format) == NULL)
	{
		if (trigger == 1)
			std::cerr << date << " ==> Invalid date format" << std::endl;
		else
			std::cerr << "Invalid date format" << std::endl;
		return (false);
	}
	year = atoi(date.substr(start, end).c_str());
	is_leap_year = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	start = end + 1;
	end = date.find('-', start);
	month = atoi(date.substr(start, end).c_str());
	start = end + 1;
	end = date.size();
	day = atoi(date.substr(start, end).c_str());
	if ((year < 2009) || (year == 2009 && month == 1 && day < 2) //check date isnt before the creation of bitcoin
		|| (month == 2 && day == 29 && !is_leap_year) //check leap years
		|| (day > get_nb_of_days_in_month(month))) //check correct number of days in month
	{
		if (trigger == 1)
			std::cerr << date << " ==> Invalid date" << std::endl;
		else
			std::cerr << "Invalid date" << std::endl;
		return (false);
	}
	return (true);
}

static bool	check_input_file_value(std::string value)
{
	int	count = 0;
	int	nb_test = atoi(value.c_str());

	for (size_t i = 0; i < value.size(); ++i)
	{
		if (isdigit(value[i]) == 0 && value[i] != '.')
			count = 2;
		if ((i == 0 && value[i] == '.') || (i == value.size() - 1 && value[i] == '.'))
			count = 2;
		if (value[i] == '.')
			count++;
	}
	if (nb_test > 1000 || nb_test < 0)
		count = 2;
	if (count > 1)
	{
		std::cerr << "Error : " << value << " ==> Bad input" << std::endl;
		return (false);
	}
	return (true);
}

static bool	check_database_value(std::string value)
{
	int	count = 0;

	for (size_t i = 0; i < value.size(); ++i)
	{
		if (isdigit(value[i]) == 0 && value[i] != '.')
			count = 2;
		if ((i == 0 && value[i] == '.') || (i == value.size() - 1 && value[i] == '.'))
			count = 2;
		if (value[i] == '.')
			count++;
	}
	if (count > 1)
	{
		std::cerr << "Invalid value in database" << std::endl;
		return (false);
	}
	return (true);
}

static bool	empty_line(std::string line)
{
	if (line.find_first_not_of(" \r\v\f\n\t") == std::string::npos)
		return (true);
	return (false);
}

static int	get_nb_of_days_in_month(int month)
{
	switch (month)
	{
		case 1:
			return (31);
		case 2:
			return (29);
		case 3:
			return (31);
		case 4:
			return (30);
		case 5:
			return (31);
		case 6:
			return (30);
		case 7:
			return (31);
		case 8:
			return (31);
		case 9:
			return (30);
		case 10:
			return (31);
		case 11:
			return (30);
		case 12:
			return (31);
		default:
			return (0);
	}
}
