#pragma once

#ifndef C3DMODEL_H_FBX
#define C3DMODEL_H_FBX

#include <string>
using namespace std;

#include "C3DModel.h"

class C3DModel_Fbx : public C3DModel
{
private:
	int m_currentVertex, m_currentNormal, m_currentUV, m_currentFace,m_currentUV_Index;	// Aux counters used when loading an object from file



	//bool readFbxFile(const char * const filename, bool countOnly);		// Read object from file 
	bool parseFbxLine(const char * const filename, bool countOnly);							// Parse line


	bool readMtllib(std::string mtlLibFilename);

protected:
	void reset();														// Cleanup any allocated memory
	bool loadFromFile(const char * const filename);
public:

	C3DModel_Fbx();
	~C3DModel_Fbx();

};



#endif
