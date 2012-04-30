#pragma once

#include "OpMesh2Raw.h"
#include "OpRaw2OPC.h"

class OpMesh2OPC
{
public:
	inline OpMesh2OPC()
	{
	}
	~OpMesh2OPC()
	{
	}

public:
	inline int Do(std::string infile, std::string outfile)
	{
		OpMesh2Raw op0;
		int res = op0.Do(infile, "temp.raw");

		OpRaw2OPC op1;
		res |= op1.Do("temp.raw", outfile);
		
		return res;
	}
};
