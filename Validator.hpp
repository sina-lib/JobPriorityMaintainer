#include <iostream>

#ifndef _VALIDATOR_HEADER_
#define _VALIDATOR_HEADER_

template <typename T>
class Validator
{
public:
	Validator() = delete;
	explicit Validator(std::istream& isref, std::ostream& osref) : is{isref} , os{osref}  {};
	Validator& operator>> (T& inp)
		{
			while ( (os << ">") && !(is>>inp) )
			{
				is.clear();
				is.ignore( std::numeric_limits<std::streamsize>::max() , '\n');
				os << "--Bad input\n" ;
			}
			is.ignore( std::numeric_limits<std::streamsize>::max() , '\n');
			return *this;
		};
private:
	std::istream& is;
	std::ostream& os;
};

#endif
