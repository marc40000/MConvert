//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Internal Note: Most parts are taken from "meinedat\projects\MConvert\OgreMeshExtractor\Application.cpp"
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Ogre.h"
#include "OgreQuake3Level.h"
#include "OgreBspLevel.h"
#include "OgreBspResourceManager.h"

using namespace Ogre;

class OpBSP2Raw
{
public:
	inline OpBSP2Raw()
	{
	}
	~OpBSP2Raw()
	{
	}

private:
	Root *mRoot;
	Camera* mCamera;
	SceneManager* mSceneMgr;
	RenderWindow* mWindow;
	SceneNode *mModelNode;
	Entity *mModel;

	String mQuakePk3;
	String mQuakeLevel;
	BspLevel * level;


	inline int InitOgre(TCHAR * infile)
	{
		char infilepatha[1024];
		wcstombs(infilepatha, infile, 1024);
		// get the path only
		char * p = strrchr(infilepatha, '\\');
		if (p != 0)
		{
			*p = 0;
		}
		else
		{
			sprintf(infilepatha, ".");
		}

#ifdef _DEBUG
		mRoot = new Root("plugins_d.cfg");
#else
		mRoot = new Root("plugins.cfg");
#endif

		// Load resource paths from config file
		ConfigFile cf;
		cf.load("resources.cfg");

		// Go through all settings in the file
		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
		
		Ogre::String secName, typeName, archName;
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator iter;
			for (iter = settings->begin(); iter != settings->end(); ++iter)
			{
				typeName = iter->first;
				archName = iter->second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
			}
		}
		// add the dir of the mesh to the resourcemanager
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(infilepatha, "FileSystem", "General");



		mRoot->restoreConfig();
		mRoot->initialise(false);

		mWindow = mRoot->createRenderWindow("MConvert",640,480,false,0);

		//cf.load("quake3settings.cfg");
		//mQuakePk3 = cf.getSetting("Pak0Location");
		//mQuakeLevel = cf.getSetting("Map");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(infilepatha, "FileSystem", "General");


		mSceneMgr = mRoot->createSceneManager(Ogre::ST_INTERIOR);

		// add the dir of the mesh to the resourcemanager
		//ResourceManager::addCommonArchiveEx(mQuakePk3, "Zip");
		//mSceneMgr = mRoot->getSceneManager(ST_INTERIOR);

		mCamera = mSceneMgr->createCamera("PlayerCam");
		mCamera->setPosition(Vector3(-20,10,20));
		mCamera->lookAt(Vector3(0,2,0));
		mCamera->setNearClipDistance(0.1);
		mCamera->setFarClipDistance( 200 );

		// Create one viewport, entire window
		Viewport* vp = mWindow->addViewport(mCamera);
		vp->setBackgroundColour(ColourValue(0,0,0));

		// Alter the camera aspect ratio to match the viewport
		mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));


		// Set default mipmap level (NB some APIs ignore this)
		//TextureManager::getSingleton().setDefaultNumMipMaps(5);

		mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

		// Fog
		// NB it's VERY important to set this before calling setWorldGeometry 
		// because the vertex program picked will be different
		ColourValue fadeColour(0.93, 0.86, 0.76);
		mSceneMgr->setFog( FOG_LINEAR, fadeColour, .001, 500, 2000);
		mWindow->getViewport(0)->setBackgroundColour(fadeColour);

		return 0;
	}

	inline int LoadBSP(TCHAR * filename)
	{
		char filenamea[1024];
		wcstombs(filenamea, filename, 1024);

		level = BspResourceManager::getSingleton().load(mQuakeLevel);
		
		return 0;
	}

	inline int CreateRawFile(TCHAR * filename)
	{
		MeshPtr tree_mesh = mModel->getMesh();

	/*	int n=mesh->getNumSubMeshes();
		int i,j;
		for (i=0;i<n;i++)
		{
			SubMesh *submesh=mesh->getSubMesh(i);
			VertexData *vd;
			if (submesh->useSharedVertices)
				vd=mesh->sharedVertexData;
			else
				vd=submesh.vertexData;
			IndexData *id=submesh->indexdata;
			
			if (IT_16BIT==id->indexBuffer->getType())
			{
				indexsize=2;
			}
			else
			if (IT_32BIT==id->indexBuffer->getType())
			{
				indexsize=4;
			}
			else
			{
				// unkown index type
				int a=3;
			}

			unsigned char *p=new unsigned char[id->indexCount*indexsize];
			id->indexBuffer->lock();
			id->indexBuffer->readData(id->indexStart*indexsize,id->indexCount*indexsize,p);
			id->indexBuffer->unlock();
			
			fprintf(f,"%u\t",id->indexCount);
			for (j=0;j<id->indexCount;j++)
			{
				unsigned int curindex;
				if (indexsize==2)
					curindex=*((unsigned short int*)(p+j*indexsize));
				else
					curindex=*((unsigned int*)(p+j*indexsize));
				
				VertexElement *ve=cd->vertexDeclaration->getElement(curindex);
				ve->getOffset();
				HardwareVertexBufferSharedPtr hvbs=vd->vertexBufferBinding->getBuffer(0);
				HardwareVertexBuffer *hvb=hvbs.getPointer();

			}

			delete [] p;
		}*/

		SceneNode *node=mModelNode;

		bool added_shared = false;
		size_t _vertex_count = 0,_index_count = 0;
		size_t current_offset = _vertex_count,shared_offset = _vertex_count,next_offset = _vertex_count,index_offset = _index_count;
		size_t prev_vert = _vertex_count,prev_ind = _index_count;

		Quaternion orient = node->getOrientation();
		Vector3 position = node->getPosition();
		Vector3 scale = node->getScale();

		// Calculate how many vertices and indices we're going to need
		for(int i = 0;i < tree_mesh->getNumSubMeshes();i++)
		{
			SubMesh* submesh = tree_mesh->getSubMesh(i);

			// We only need to add the shared vertices once
			if(submesh->useSharedVertices)
			{
				if(!added_shared)
				{
					VertexData* vertex_data = tree_mesh->sharedVertexData;
					_vertex_count += vertex_data->vertexCount;
					added_shared = true;
				}
			}
			else
			{
				VertexData* vertex_data = submesh->vertexData;
				_vertex_count += vertex_data->vertexCount;
			}

			// Add the indices
			IndexData* index_data = submesh->indexData;
			_index_count += index_data->indexCount;
		}

		// (re)allocate space for the vertices and indices
		Vector3* _vertices = new Vector3[_vertex_count];

		int* _indices = new int[_index_count];

		added_shared = false;

		// Run through the submeshes again, adding the data into the arrays
		for(int i = 0;i < tree_mesh->getNumSubMeshes();i++)
		{
			SubMesh* submesh = tree_mesh->getSubMesh(i);

			Ogre::VertexData* vertex_data = submesh->useSharedVertices ? tree_mesh->sharedVertexData : submesh->vertexData;
			if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
			{
				if(submesh->useSharedVertices)
				{
					added_shared = true;
					shared_offset = current_offset;
				}

				// Get the positional buffer element
				const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);			
				Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
				unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
				Ogre::Real* pReal;

				for(size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
				{
					posElem->baseVertexPointerToElement(vertex, &pReal);

					Vector3 pt;

					pt.x = (*pReal++);
					pt.y = (*pReal++);
					pt.z = (*pReal++);

					pt = (orient * (pt * scale)) + position;

					_vertices[current_offset + j].x = pt.x;
					_vertices[current_offset + j].y = pt.y;
					_vertices[current_offset + j].z = pt.z;
				}
				vbuf->unlock();

				next_offset += vertex_data->vertexCount;
			}

			Ogre::IndexData* index_data = submesh->indexData;

			size_t numTris = index_data->indexCount / 3;
			unsigned short* pShort;
			unsigned int* pInt;
			Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
			bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
			if (use32bitindexes) pInt = static_cast<unsigned int*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			else pShort = static_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			for(size_t k = 0; k < numTris; ++k)
			{
				size_t offset = (submesh->useSharedVertices)?shared_offset:current_offset;

				unsigned int vindex = use32bitindexes? *pInt++ : *pShort++;
				_indices[index_offset + 0] = vindex + offset;
				vindex = use32bitindexes? *pInt++ : *pShort++;
				_indices[index_offset + 1] = vindex + offset;
				vindex = use32bitindexes? *pInt++ : *pShort++;
				_indices[index_offset + 2] = vindex + offset;

				index_offset += 3;
			}
			ibuf->unlock();
			current_offset = next_offset;
		}

		FILE *f;
		_tfopen_s(&f, filename, _T("wt"));
		if (f == 0)
		{
			return 1;
		}
		for(size_t offs = 0;offs < _index_count;offs += 3)
		{
			std::vector<Vector3> verts;

	/*		fprintf(f,"%f %f %f\t  %f %f %f\t  %f %f %f\n",
				_vertices[_indices[offs + 0]].x,
				_vertices[_indices[offs + 0]].y,
				_vertices[_indices[offs + 0]].z,
				_vertices[_indices[offs + 1]].x,
				_vertices[_indices[offs + 1]].y,
				_vertices[_indices[offs + 1]].z,
				_vertices[_indices[offs + 2]].x,
				_vertices[_indices[offs + 2]].y,
				_vertices[_indices[offs + 2]].z
				);*/
			fprintf(f,"%.16f %.16f %.16f\t  %.16f %.16f %.16f\t  %.16f %.16f %.16f\n",
				_vertices[_indices[offs + 0]].x,
				-_vertices[_indices[offs + 0]].z,
				_vertices[_indices[offs + 0]].y,
				_vertices[_indices[offs + 1]].x,
				-_vertices[_indices[offs + 1]].z,
				_vertices[_indices[offs + 1]].y,
				_vertices[_indices[offs + 2]].x,
				-_vertices[_indices[offs + 2]].z,
				_vertices[_indices[offs + 2]].y
				);
		}
		fclose(f);

		delete[] _vertices;
		delete[] _indices;

		return 0;
	}

public:
	inline int Do(TCHAR * infile, TCHAR * outfile)
	{
		_tprintf(_T("initializing Ogre\n"));
		InitOgre(infile);
		_tprintf(_T("loading model %s\n"), infile);
		//LoadMesh(infile);
		LoadBsp(infile);

		_tprintf(_T("creating raw rile %s\n"), outfile);
		CreateRawFile(outfile);
		//CreateRawFileFromBsp(filenameout);

		_tprintf(_T("done.\n"));
		
		return 0;
	}
};
