// MConvert.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "OpMesh2Raw.h"
//#include "OpBSP2Raw.h"
#include "OpRaw2OPC.h"
#include "OpRaw2NTC.h"

// combined ops
#include "OpMesh2OPC.h"
#include "OpMesh2NTC.h"


void PrintHelp()
{
	std::cout << "Usage:" << std::endl;
	std::cout << "    MConvert {op} {infile} {outfile}" << std::endl;
	std::cout << "Parameters:" << std::endl;
	std::cout << "    op      : mesh2raw, raw2opc, raw2ntc, mesh2opc, mesh2ntc" << std::endl;
	std::cout << "    infile  : input filename" << std::endl;
	std::cout << "    outfile : output filename" << std::endl;
}

const unsigned int MConvertOpMesh2Raw = 0;
const unsigned int MConvertOpBSP2Raw = 1;
const unsigned int MConvertOpRaw2OPC = 2;
const unsigned int MConvertOpRaw2NTC = 3;

const unsigned int MConvertOpMesh2OPC = 10;
const unsigned int MConvertOpMesh2NTC = 11;


#ifdef _WIN32
int Go(int argc, _TCHAR* argv[])
#else
int Go(int argc, char* argv[])
#endif
{
	std::string ops = argv[1];
	std::string infile = argv[2];
	std::string outfile;
	
	if (argc == 4)
	{
		outfile = argv[3];
	}
	else
	{
		outfile = infile;
		unsigned int n = infile.length() - 1;
		bool ok = false;
		while (n > 0)
		{
			if (infile.at(n) == '.')
			{
				infile.at(n) = 0;
				ok = true;
				break;
			}
			--n;
		}
		if (!ok)
		{
			std::cout << "Error: Input filename does not contain a '.'" << std::endl;
			return 1;
		}
	}

	std::cout << "Performing:" << std::endl;
	std::cout << "    op      :" << ops << std::endl;
	std::cout << "    infile  :" << infile << std::endl;
	std::cout << "    outfile :" << outfile << std::endl;

	int op;
	if (ops == "mesh2raw")
	{
		op = MConvertOpMesh2Raw;
	}
	else
	if (ops == "bsp2raw")
	{
		op = MConvertOpBSP2Raw;
	}
	else
	if (ops == "raw2opc")
	{
		op = MConvertOpRaw2OPC;
	}
	else
	if (ops == "raw2ntc")
	{
		op = MConvertOpRaw2NTC;
	}
	else
	if (ops == "mesh2opc")
	{
		op = MConvertOpMesh2OPC;
	}
	else
	if (ops == "mesh2ntc")
	{
		op = MConvertOpMesh2NTC;
	}
	else
	{
		return -1;
		std::cout << "illegal op" << std::endl;
	}


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
			res = op.Do(infile, outfile + ".ntc");
			break;
		}
	default:
		{
			std::cout << "invalid opcode, INTERNAL BUG, SHOULD NEVER HAPPEN" << std::endl;
			return -1;
		}
	}

	std::cout << "The op returned " << res << " => success!" << std::endl;

	return 0;
}

#ifdef _WIN32
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	std::cout << "MConvert v1   (c) 2009 M. Rochel" << std::endl;
	if ((argc != 3) && (argc != 4))
	{
		PrintHelp();
		return -1;
	}
	int ret = Go(argc, argv);

	return ret;
}

