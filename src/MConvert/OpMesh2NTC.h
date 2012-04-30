#pragma once

#include "OpMesh2Raw.h"
#include "OpRaw2NTC.h"

class OpMesh2NTC
{
public:
	inline OpMesh2NTC()
	{
	}
	~OpMesh2NTC()
	{
	}

public:
	inline int Do(std::string infile, std::string outfile)
	{
		OpMesh2Raw op0;
		int res = op0.Do(infile, "temp.raw");

		OpRaw2NTC op1;
		res |= op1.Do("temp.raw", outfile);
		
		return res;
	}
};
