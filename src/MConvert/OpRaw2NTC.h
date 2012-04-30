//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Internal Note: Most parts are taken from "meinedat\projects\MConvert\raw2opc\main.cpp"
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Newton.h"
#include "dVector.h"


class OpRaw2NTC
{
public:
	inline OpRaw2NTC()
	{
	}
	~OpRaw2NTC()
	{
	}

private:

	FILE * serf;

	static void NSerializeFunction(void* serializeHandle, const void* buffer, int size)
	{
		_tprintf(_T("NSerialize size=%i\n"), size);
		OpRaw2NTC * op = (OpRaw2NTC *)serializeHandle;
		fwrite(buffer, 1, size, op->serf);
	}

	static void NCollisionIterator (void* userData, int vertexCount, const dFloat* FaceArray, int faceId)
	{
		float * bbiter = (float*)userData;
		float x[3];
		int k;
		for (k = 0; k < vertexCount; ++k)
		{
			x[0] = FaceArray[k * 3];
			x[1] = FaceArray[k * 3 + 1];
			x[2] = FaceArray[k * 3 + 2];

			bbiter[0] = MMin(bbiter[0], x[0]);
			bbiter[1] = MMin(bbiter[1], x[1]);
			bbiter[2] = MMin(bbiter[2], x[2]);
			bbiter[3] = MMax(bbiter[3], x[0]);
			bbiter[4] = MMax(bbiter[4], x[1]);
			bbiter[5] = MMax(bbiter[5], x[2]);
		}
	}


	inline int NewtonConvert(TCHAR * infile, TCHAR * outfile)
	{
		FILE *f;

		NewtonWorld *nWorld=NewtonCreate(0,0);
		NewtonCollision *col=NewtonCreateTreeCollision(nWorld,0);
		NewtonTreeCollisionBeginBuild(col);
		
		printf("loading raw data\n");
		_tfopen_s(&f, infile, _T("rt"));
		float x[9];
		int n=0;
		float bb[6];
		bb[0] = 1e32;
		bb[1] = 1e32;
		bb[2] = 1e32;
		bb[3] = -1e32;
		bb[4] = -1e32;
		bb[5] = -1e32;
		while (!feof(f))
		{
			//fscanf(f,"%f %f %f  %f %f %f  %f %f %f\n",x+0,x+1,x+2, x+3,x+4,x+5, x+6,x+7,x+8);
			//fscanf(f,"%f %f %f  %f %f %f  %f %f %f\n",x+0,x+1,x+2, x+6,x+7,x+8, x+3,x+4,x+5);
			//fscanf(f,"%f %f %f  %f %f %f  %f %f %f\n",x+0,x+2,x+1, x+6,x+8,x+7, x+3,x+5,x+4);
			//fscanf(f,"%f %f %f  %f %f %f  %f %f %f\n",x+0,x+2,x+1, x+3,x+5,x+4, x+6,x+8,x+7);
			//fscanf(f,"%f %f %f  %f %f %f  %f %f %f\n",x+0,x+2,x+1, x+3,x+5,x+4, x+6,x+8,x+7);
			//x[2]*=-1;
			//x[8]*=-1;
			//x[5]*=-1;
			fscanf(f,"%f %f %f  %f %f %f  %f %f %f\n",x+0,x+1,x+2, x+3,x+4,x+5, x+6,x+7,x+8);

			bb[0] = MMin(bb[0], x[0]);
			bb[1] = MMin(bb[1], x[1]);
			bb[2] = MMin(bb[2], x[2]);
			bb[3] = MMax(bb[3], x[0]);
			bb[4] = MMax(bb[4], x[1]);
			bb[5] = MMax(bb[5], x[2]);

			bb[0] = MMin(bb[0], x[3]);
			bb[1] = MMin(bb[1], x[4]);
			bb[2] = MMin(bb[2], x[5]);
			bb[3] = MMax(bb[3], x[3]);
			bb[4] = MMax(bb[4], x[4]);
			bb[5] = MMax(bb[5], x[5]);

			bb[0] = MMin(bb[0], x[6]);
			bb[1] = MMin(bb[1], x[7]);
			bb[2] = MMin(bb[2], x[8]);
			bb[3] = MMax(bb[3], x[6]);
			bb[4] = MMax(bb[4], x[7]);
			bb[5] = MMax(bb[5], x[8]);

			NewtonTreeCollisionAddFace(col,3,x,12,0);
			n++;
		}
		fclose(f);

		_tprintf(_T("RAW READ BB:   %f %f %f   %f %f %f\n"), bb[0], bb[1], bb[2], bb[3], bb[4], bb[5]);
		

		printf("building NewtonTreeCollision from %u faces\n",n);
		NewtonTreeCollisionEndBuild(col,1);

		printf("\nwriting ntc file\n");
		_tfopen_s(&serf, outfile, _T("wb"));
		int mconvertversion = 0;
		fwrite(&mconvertversion, sizeof(int), 1, f);
		_tprintf(_T("MConvertVersion: %i\n"), mconvertversion);
		int newtonversion = NewtonWorldGetVersion();
		_tprintf(_T("NewtonWorldGetVersion: %i\n"), newtonversion);
		fwrite(&newtonversion, sizeof(int), 1, f);
		fwrite(bb, sizeof(float), 6, f);
		NewtonCollisionSerialize(nWorld, col, NSerializeFunction, this);
		fclose(serf);


		//float bb[6];
		for (int i = 0; i < 3; i ++)
		{
			dVector point;
			dVector dir (0.0f, 0.0f, 0.0f, 0.0f);

			// find the most extreme point along this axis and this is maximum Box size on that direction
			dir[i] = 1.0f;
			NewtonCollisionSupportVertex (col, &dir[0], &point[0]);
			bb[3 + i] = point[i];
			//bb[3 + i] = point % dir;

			// find the most extreme point along the opposite axis and this is maximum Box size on that direction
			dir[i] = -1.0f;
			NewtonCollisionSupportVertex (col, &dir[0], &point[0]);
			bb[i] = point[i];
			//bb[i] = -(point % dir);
		}
		_tprintf(_T("NTC BB:   %f %f %f   %f %f %f\n"), bb[0], bb[1], bb[2], bb[3], bb[4], bb[5]);


		float bbiter[6];
		bbiter[0] = 1e32;
		bbiter[1] = 1e32;
		bbiter[2] = 1e32;
		bbiter[3] = -1e32;
		bbiter[4] = -1e32;
		bbiter[5] = -1e32;
		float matrix[16] = {1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1};
		NewtonCollisionForEachPolygonDo(col, matrix, NCollisionIterator, bbiter);
		_tprintf(_T("NTC BBITER:   %f %f %f   %f %f %f\n"), bbiter[0], bbiter[1], bbiter[2], bbiter[3], bbiter[4], bbiter[5]);


		return 0;
	}

public:
	inline int Do(TCHAR * infile, TCHAR * outfile)
	{
		int res;
		res = NewtonConvert(infile, outfile);
		
		return res;
	}
};
