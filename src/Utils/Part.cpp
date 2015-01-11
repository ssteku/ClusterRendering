#include "Part.hpp"

using namespace std;

Part::Part(Context cont, unsigned int partNr) :
		context(cont), partNumber(partNr)
{

}

unsigned int Part::getPartNumber() const
{
	return partNumber;
}

const Context &Part::getContext()
{
	return context;
}


