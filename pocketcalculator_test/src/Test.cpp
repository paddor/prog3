#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

#include <string>
#include <sstream>
#include "calc.h"
#include "sevensegment.h"
#include "pocketcalculator.h"

namespace calc_tests {
	void it_multiplies() {
		ASSERT_EQUAL(20, calc(4, 5, '*'));
		ASSERT_EQUAL(-30, calc(-6, 5, '*'));
	}
	void it_divides() {
		ASSERT_EQUAL(5, calc(60, 12, '/'));
		ASSERT_EQUAL(-3, calc(9, -3, '/'));
	}
	void it_throws_when_dividing_by_zero() {
		ASSERT_THROWS(calc(1, 0, '/'), std::domain_error);
	}
	void it_adds() {
		ASSERT_EQUAL(5, calc(2, 3, '+'));
		ASSERT_EQUAL(-10, calc(-6, -4, '+'));
	}
	void it_subtracts() {
		ASSERT_EQUAL(17, calc(20, 3, '-'));
		ASSERT_EQUAL(-5, calc(-2, 3, '-'));
	}
	void it_knows_modulo() {
		ASSERT_EQUAL(5, calc(15, 10, '%'));
		ASSERT_EQUAL(-5, calc(-15, 10, '%'));
	}
	void it_throws_when_modulo_zero() {
		ASSERT_THROWS(calc(10, 0, '%'), std::domain_error);
	}
	void it_throws_when_given_invalid_operator() {
		ASSERT_THROWS(calc(1, 1, '^'), std::invalid_argument);
	}

	void it_takes_term_from_istream() {
		std::istringstream term_stream { "1+1" };
		int result = calc(term_stream);
		ASSERT_EQUAL(2, result);
	}
	void it_takes_term_from_string() {
		std::string term { "3-5" };
		int result = calc(term);
		ASSERT_EQUAL(-2, result);
	}
	void it_throws_when_given_invalid_term() {
		std::vector<std::string> invalid_terms {
			"foobar",
			"3+2-",
			"1",
			"8--",
			"*",
			"4%%6",
			"3//7",
		};
		for_each(invalid_terms.begin(), invalid_terms.end(), [](std::string term) {
			ASSERT_THROWS(calc(term), std::exception);
		});
	}
}

namespace sevensegment_tests {
	const std::string large_8 {
		" - \n"
		"| |\n"
		" - \n"
		"| |\n"
		" - \n"
	};
	const std::string large_1 {
		"   \n"
		"  |\n"
		"   \n"
		"  |\n"
		"   \n"
	};
	const std::string large_3_scale2 {
		" -- \n"
		"   |\n"
		"   |\n"
		" -- \n"
		"   |\n"
		"   |\n"
		" -- \n"
	};

	void it_prints_digit() {
		std::ostringstream output {};
		sevensegment::printLargeDigit(8, output);
		ASSERT_EQUAL(large_8, output.str());
		output.str("");
		sevensegment::printLargeDigit(1, output);
		ASSERT_EQUAL(large_1, output.str());
	}
	void it_prints_scaled_digit() {
		std::ostringstream output {};
		sevensegment::printLargeDigit(3, output, 2);
		ASSERT_EQUAL(large_3_scale2, output.str());
	}
	void it_throws_when_digit_out_of_range() {
		std::ostringstream output {};
		ASSERT_THROWS(sevensegment::printLargeDigit(10,output), std::out_of_range);
	}

	const std::string large_number {
		" -     -  -    \n"
		"|  | |  |  |  |\n"
		" -  -  -  -    \n"
		"  |  |  ||    |\n"
		" -     -  -    \n"
	};

	const std::string large_negative_number {
	"    -  - \n"
	"     |  |\n"
	" -  -  - \n"
	"     |  |\n"
	"    -  - \n"
	};

	void it_prints_number() {
		std::ostringstream output {};
		sevensegment::printLargeNumber(54321, output);
		ASSERT_EQUAL(large_number, output.str());
	}

	void it_prints_negative_number() {
		std::ostringstream output {};
		sevensegment::printLargeNumber(-33, output);
		ASSERT_EQUAL(large_negative_number, output.str());
	}

	const std::string large_error {
		" -             \n"
		"|              \n"
		" -  -  -  -  - \n"
		"|  |  |  | ||  \n"
		" -        -    \n"
	};

	void it_prints_error() {
		std::ostringstream output {};
		sevensegment::printLargeError(output);
		ASSERT_EQUAL(large_error, output.str());
	}
}

void runAllTests(int argc, char const *argv[]){
	cute::suite s {};

	// calc tests
	s.push_back(CUTE(calc_tests::it_multiplies));
	s.push_back(CUTE(calc_tests::it_divides));
	s.push_back(CUTE(calc_tests::it_throws_when_dividing_by_zero));
	s.push_back(CUTE(calc_tests::it_adds));
	s.push_back(CUTE(calc_tests::it_subtracts));
	s.push_back(CUTE(calc_tests::it_throws_when_given_invalid_operator));
	s.push_back(CUTE(calc_tests::it_knows_modulo));
	s.push_back(CUTE(calc_tests::it_throws_when_modulo_zero));
	s.push_back(CUTE(calc_tests::it_takes_term_from_istream));
	s.push_back(CUTE(calc_tests::it_throws_when_given_invalid_term));
	s.push_back(CUTE(calc_tests::it_takes_term_from_string));

	// sevensegment tests
	s.push_back(CUTE(sevensegment_tests::it_prints_digit));
	s.push_back(CUTE(sevensegment_tests::it_prints_scaled_digit));
	s.push_back(CUTE(sevensegment_tests::it_throws_when_digit_out_of_range));
	s.push_back(CUTE(sevensegment_tests::it_prints_number));
	s.push_back(CUTE(sevensegment_tests::it_prints_negative_number));
	s.push_back(CUTE(sevensegment_tests::it_prints_error));

	// pocketcalculator tests
	// @todo add pocketcalculator tests

	cute::xml_file_opener xmlfile(argc,argv);
	cute::xml_listener<cute::ide_listener<> >  lis(xmlfile.out);
	cute::makeRunner(lis,argc,argv)(s, "AllTests");
}

int main(int argc, char const *argv[]){
    runAllTests(argc,argv);
    return 0;
}


