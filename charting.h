#pragma once
#include <vector>
struct panel
{

};
struct chart
{
	std::vector<panel> panels;
	
};

class charting
{
public:

	charting()
	{
	}

	virtual ~charting()
	{
	}
};

