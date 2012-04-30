// MConvert.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "OpMesh2Raw.h"
//#include "OpBSP2Raw.h"
#include "OpRaw2OPC.h"
#include "OpRaw2NTC.h"

// combined ops
#include "OpMesh2OPC.h"
#include "OpMesh2NTC.h"


void PrintHelp()
{
	_tprintf(_T("Usage:\n"));
	_tprintf(_T("    MConvert {op} {infile} {outfile}\n"));
	_tprintf(_T("Parameters:\n"));
	_tprintf(_T("    op      : mesh2raw, raw2opc, raw2ntc, mesh2opc, mesh2ntc\n"));
	_tprintf(_T("    infile  : input filename\n"));
	_tprintf(_T("    outfile : output filename\n"));
}

const unsigned int MConvertOpMesh2Raw = 0;
const unsigned int MConvertOpBSP2Raw = 1;
const unsigned int MConvertOpRaw2OPC = 2;
const unsigned int MConvertOpRaw2NTC = 3;

const unsigned int MConvertOpMesh2OPC = 10;
const unsigned int MConvertOpMesh2NTC = 11;

int Go(int argc, _TCHAR* argv[])
{
	_tprintf(_T("MConvert v1   (c) 2009 M. Rochel\n"));

	if ((argc != 3) && (argc != 4))
	{
		PrintHelp();
	}
	else
	{
		TCHAR * ops;
		TCHAR * infile;
		TCHAR * outfile;

		TCHAR temp[1024];

		ops = argv[1];
		infile = argv[2];
		if (argc == 4)
		{
			outfile = argv[3];
		}
		else
		{
			outfile = temp;
			_tcscpy_s(temp, 1024, infile);
			unsigned int n = _tcslen(infile) - 1;
			bool ok = false;
			while (n > 0)
			{
				if (temp[n] == _T('.'))
				{
					temp[n] = 0;
					ok = true;
					break;
				}
				--n;
			}
			if (!ok)
			{
				_tprintf(_T("Error: Input filename does not contain a '.'\n"));
				return 1;
			}
		}

		_tprintf(_T("Performing:\n"));
		_tprintf(_T("    op      : %s\n"), ops);
		_tprintf(_T("    infile  : %s\n"), infile);
		_tprintf(_T("    outfile : %s\n"), outfile);

		int op;
		if (_tcscmp(ops, _T("mesh2raw")) == 0)
		{
			op = MConvertOpMesh2Raw;
		}
		else
		if (_tcscmp(ops, _T("bsp2raw")) == 0)
		{
			op = MConvertOpBSP2Raw;
		}
		else
		if (_tcscmp(ops, _T("raw2opc")) == 0)
		{
			op = MConvertOpRaw2OPC;
		}
		else
		if (_tcscmp(ops, _T("raw2ntc")) == 0)
		{
			op = MConvertOpRaw2NTC;
		}
		else
		if (_tcscmp(ops, _T("mesh2opc")) == 0)
		{
			op = MConvertOpMesh2OPC;
		}
		else
		if (_tcscmp(ops, _T("mesh2ntc")) == 0)
		{
			op = MConvertOpMesh2NTC;
		}
		else
		{
			op = -1;
			_tprintf(_T("illegal op\n"));
		}

		if (op != -1)
		{
			int res;
			switch (op)
			{
			case MConvertOpMesh2Raw:
				{
					OpMesh2Raw op;
					res = op.Do(infile, outfile);
					break;
				}
			case MConvertOpBSP2Raw:
				{
					//OpBSP2Raw op;
					//res = op.Do(infile, outfile);
					res = 0;
					break;
				}
			case MConvertOpRaw2OPC:
				{
					OpRaw2OPC op;
					res = op.Do(infile, outfile);
					break;
				}
			case MConvertOpRaw2NTC:
				{
					OpRaw2NTC op;
					res = op.Do(infile, outfile);
					break;
				}
			case MConvertOpMesh2OPC:
				{
					OpMesh2OPC op;
					res = op.Do(infile, outfile);
					break;
				}
			case MConvertOpMesh2NTC:
				{
					OpMesh2NTC op;
					_tcscat(outfile, _T(".ntc"));
					res = op.Do(infile, outfile);
					break;
				}
			default:
				{
					// should never happen
					res = -1;
				}
			}

			if (res == 0)
			{
				_tprintf(_T("The op returned %i => success!\n"), res);
			}
			else
			{
				_tprintf(_T("The op returned %i => failure!\n"), res);
			}
		}
	}

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int ret = Go(argc, argv);

	return ret;
}

