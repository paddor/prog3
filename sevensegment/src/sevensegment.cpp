#include <ostream>
#include <iostream>
#include <vector>
#include <string>
#include "sevensegment.h"


const std::vector< std::vector<std::string> > digits {
	{ " - ", "| |", "   ", "| |", " - " }, // 0
	{ "   ", "  |", "   ", "  |", "   " }, // 1
	{ " - ", "  |", " - ", "|  ", " - " }, // 2
	{ " - ", "  |", " - ", "  |", " - " }, // 3
	{ "   ", "| |", " - ", "  |", "   " }, // 4
	{ " - ", "|  ", " - ", "  |", " - " }, // 5
	{ " - ", "|  ", " - ", "| |", " - " }, // 6
	{ " - ", "  |", "   ", "  |", "   " }, // 7
	{ " - ", "| |", " - ", "| |", " - " }, // 8
	{ " - ", "| |", " - ", "  |", " - " }  // 9
};
void printLargeDigit(unsigned i, std::ostream &out) {
	std::vector< std::string> digit = digits.at(i);
	for_each(digit.begin(), digit.end(), [&](std::string line) {
		out << line << '\n';
		out << line << '\n';
	});
}
