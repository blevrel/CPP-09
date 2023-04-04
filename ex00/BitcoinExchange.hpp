#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP
#include <iostream>
#include <fstream>
#include <map>
#include <ctime>
#include <cstdlib>
#include <sstream>

class BitcoinExchange
{

public:

	BitcoinExchange(void);
	BitcoinExchange(std::string filename);
	BitcoinExchange(const BitcoinExchange & other);
	~BitcoinExchange(void);
	BitcoinExchange & operator=(const BitcoinExchange & other);

const std::map<std::string, float>& getDatabase(void) const;

private:

	std::map<std::string, float>		_database;
	bool								_parse_database(std::string filename);
	void								_parse_input_file(std::string filename);

};

#endif
