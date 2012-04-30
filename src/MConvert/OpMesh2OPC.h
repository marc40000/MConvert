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
	inline int Do(TCHAR * infile, TCHAR * outfile)
	{
		OpMesh2Raw op0;
		int res = op0.Do(infile, _T("temp.raw"));

		OpRaw2OPC op1;
		res |= op1.Do(_T("temp.raw"), outfile);

		_tunlink(_T("temp.raw"));
		
		return res;
	}
};
