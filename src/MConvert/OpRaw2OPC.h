//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Internal Note: Most parts are taken from "meinedat\projects\MConvert\raw2opc\main.cpp"
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Newton.h"

inline NewtonWorld * NewtonCreate(void * a, void * b)
{
	return NewtonCreate();
}

inline NewtonBody * NewtonCreateBody (const NewtonWorld* newtonWorld, const NewtonCollision* collision)
{
	float matrix[16];
	matrix[0] = 1;
	matrix[1] = 0;
	matrix[2] = 0;
	matrix[3] = 0;
	matrix[4] = 0;
	matrix[5] = 1;
	matrix[6] = 0;
	matrix[7] = 0;
	matrix[8] = 0;
	matrix[9] = 0;
	matrix[10] = 1;
	matrix[11] = 0;
	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	matrix[15] = 1;
	return NewtonCreateBody(newtonWorld, collision, matrix);
}



//const float mintriarea=0.012f;
const float mintriarea=-10.0f;		// disables the functiontionality to throw away small triangles

class OpRaw2OPC
{
public:
	inline OpRaw2OPC()
	{
	}
	~OpRaw2OPC()
	{
	}

private:

	static float *vert;
	static unsigned int vertn;
	static unsigned int *index;
	static unsigned int indexn;
	static unsigned int curindex;
	static unsigned int i;

	//static void NCollisionIterator (const NewtonBody* body, int vertexCount, const float* FaceArray, int faceId)
	static void NCollisionIterator (void* userData, int vertexCount, const dFloat* FaceArray, int faceId)
	{
		float x[9];
		int k;
		for (k=0;k<vertexCount-2;k++)
		{
			// assume trianglefan, actually GL_POLYGON gets used in the NewtonDemo, which is like a
			// trianglefan and additionally requires the polygon to be convex, so our assumption is
			// true (it prolly also holds that the polygon lies in one plane, but we also don't need that info)
			x[0]=FaceArray[0];
			x[1]=FaceArray[1];
			x[2]=FaceArray[2];
			x[3]=FaceArray[(k+1)*3];
			x[4]=FaceArray[(k+1)*3+1];
			x[5]=FaceArray[(k+1)*3+2];
			x[6]=FaceArray[(k+2)*3];
			x[7]=FaceArray[(k+2)*3+1];
			x[8]=FaceArray[(k+2)*3+2];


			// calc area of the triangle
			float u[3],v[3],w[3],wlen;
			u[0]=x[3]-x[0];
			u[1]=x[4]-x[1];
			u[2]=x[5]-x[2];
			v[0]=x[6]-x[0];
			v[1]=x[7]-x[1];
			v[2]=x[8]-x[2];

			w[0]=u[1]*v[2]-u[2]*v[1];
			w[1]=u[2]*v[0]-u[0]*v[2];
			w[2]=u[0]*v[1]-u[1]*v[0];
			wlen=w[0]*w[0]+w[1]*w[1]+w[2]*w[2];
			if (wlen<mintriarea*mintriarea)
			{
				printf(".");		
				continue;
			}


			curindex=0xffffffff;
			for (i=0;i<vertn;i++)
			{
				if ((vert[i*3]==x[0]) &&
					(vert[i*3+1]==x[1]) &&
					(vert[i*3+2]==x[2]))
				{
					curindex=i;
					break;
				}
			}
			if (curindex==0xffffffff)
			{
				curindex=vertn;
				vert[vertn*3]=x[0];
				vert[vertn*3+1]=x[1];
				vert[vertn*3+2]=x[2];
				vertn++;
			}
			index[indexn]=curindex;
			indexn++;

			curindex=0xffffffff;
			for (i=0;i<indexn;i++)
			{
				if ((vert[i*3]==x[3]) &&
					(vert[i*3+1]==x[4]) &&
					(vert[i*3+2]==x[5]))
				{
					curindex=i;
					break;
				}
			}
			if (curindex==0xffffffff)
			{
				curindex=vertn;
				vert[vertn*3]=x[3];
				vert[vertn*3+1]=x[4];
				vert[vertn*3+2]=x[5];
				vertn++;
			}
			index[indexn]=curindex;
			indexn++;

			curindex=0xffffffff;
			for (i=0;i<indexn;i++)
			{
				if ((vert[i*3]==x[6]) &&
					(vert[i*3+1]==x[7]) &&
					(vert[i*3+2]==x[8]))
				{
					curindex=i;
					break;
				}
			}
			if (curindex==0xffffffff)
			{
				curindex=vertn;
				vert[vertn*3]=x[6];
				vert[vertn*3+1]=x[7];
				vert[vertn*3+2]=x[8];
				vertn++;
			}
			index[indexn]=curindex;
			indexn++;

			if (indexn%1000==0)
				printf("%u ",indexn);
		}
	}

	inline int NewtonConvert(TCHAR * infile, TCHAR * outfile)
	{
		vert=new float[100000000];
		vertn=0;
		index=new unsigned int[20000000];
		indexn=0;
		unsigned int trin;

		FILE *f;

		NewtonWorld *nWorld=NewtonCreate(0,0);
		NewtonCollision *col=NewtonCreateTreeCollision(nWorld,0);
		NewtonTreeCollisionBeginBuild(col);
		
		printf("loading raw data\n");
		_tfopen_s(&f, infile, _T("rt"));
		float x[9];
		int n=0;
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

			NewtonTreeCollisionAddFace(col,3,x,12,0);
			n++;
		}
		fclose(f);

		printf("building NewtonTreeCollision from %u faces\n",n);
		NewtonTreeCollisionEndBuild(col,1);
		NewtonBody *body=NewtonCreateBody(nWorld,col);

		printf("converting NewtonTreeCollision to opc\n");
		//NewtonBodyForEachPolygonDo(body,&NCollisionIterator);
		float matrix[16] = {1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1};
		NewtonCollisionForEachPolygonDo(col, matrix, NCollisionIterator, 0);

		trin=indexn/3;

		printf("created %u triangles\n",trin);

		printf("\nwriting opc file\n");
		_tfopen_s(&f, outfile, _T("wb"));
		fwrite(&vertn,1,4,f);
		fwrite(vert,1,12*vertn,f);
		fwrite(&trin,1,4,f);
		fwrite(index,1,12*trin,f);
		fclose(f);

		return 0;
	}


public:
	inline int Do(TCHAR * infile, TCHAR * outfile)
	{
		int res;
		//res = DirectConvert();
		res = NewtonConvert(infile, outfile);
		
		return res;
	}
};
