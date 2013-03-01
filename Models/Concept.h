#pragma once

class ConceptNoCopy
{
public:
	ConceptNoCopy() {};
private:
	ConceptNoCopy(const ConceptNoCopy& rhs);
	ConceptNoCopy& operator= (const ConceptNoCopy &rhs);
};