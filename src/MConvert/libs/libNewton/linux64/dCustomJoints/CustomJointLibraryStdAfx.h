/* Copyright (c) <2009> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/



#ifndef __JOINT_LIBRARY_STDAFX___
#define __JOINT_LIBRARY_STDAFX___

#ifdef _MSC_VER

	// Modify the following defines if you have to target a platform prior to the ones specified below.
	// Refer to MSDN for the latest info on corresponding values for different platforms.
	#ifndef WINVER				// Allow use of features specific to Windows XP or later.
	#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
	#endif

	#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
	#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
	#endif						

	#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
	#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
	#endif

	#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
	#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
	#endif

	#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
	// Windows Header Files:
	#include <windows.h>
	#include <crtdbg.h>

	#if (_MSC_VER >= 1400)
	#	pragma warning (disable: 4996) // for 2005 users declared deprecated
	#endif

	#pragma warning (disable: 4100) //unreferenced formal parameter
	#pragma warning (disable: 4127) //conditional expression is constant
#endif



#ifndef _MSC_VER
	#ifndef _NEWTON_USE_LIB
		#define _NEWTON_USE_LIB
	#endif
#endif

#ifdef _MSC_VER
	#ifdef _NEWTON_STATIC_LIB
		#define JOINTLIBRARY_API
	#else
		#ifdef JOINTLIBRARY_LIB
			#define JOINTLIBRARY_API 
		#else
			#ifdef JOINTLIBRARY_EXPORTS
				#ifdef _WIN32
					#define JOINTLIBRARY_API __declspec(dllexport)
				#else
					#define JOINTLIBRARY_API __attribute__ ((visibility("default")))
				#endif
			#else
				#ifdef _WIN32
					#define JOINTLIBRARY_API __declspec(dllimport)
				#else
					#define JOINTLIBRARY_API
				#endif
			#endif
		#endif
	#endif
#else
	#define JOINTLIBRARY_API
#endif


#include <stdlib.h>
#include <string.h>
#include <Newton.h>
//#include <JointLibrary.h>
#include <NewtonCustomJoint.h>




#endif
// TODO: reference additional headers your program requires here

