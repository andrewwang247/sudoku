#include "square.h"
#include <exception>
using namespace std;

Square::Square() :
	m_row ( 0 ), m_column ( 0 ), number ( 0 ),
	possible_values ( nullopt ) { }

vector<unsigned char> Square::possible() const {
	// Get reference to the underlying bitset.
	const auto& bs ( possible_values.value() );
	// Make the vector and reserve the right amount of space.
	vector<unsigned char> v;
	v.reserve( bs.count() );
	// Iterate through and push for those that are set.
	for ( unsigned char i = 0; i < bs.size(); ++i )
		if ( bs[i] ) v.push_back(i + 1);
	return v;
}

ostream& operator<<( ostream& os, const Square& sq ) { return os << +sq.number; }