#include "../stdafx.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#include "../Include/CVector3.h"
#include "../Include/C3DModel.h"
#include "../Include/C3DModel_Fbx.h"

C3DModel_Fbx::C3DModel_Fbx():
	C3DModel(),
	m_currentVertex(0),
	m_currentNormal(0),
	m_currentUV(0),
	m_currentFace(0)
{
	cout << "Constructor: C3DModel_Fbx()" << endl;
}

C3DModel_Fbx::~C3DModel_Fbx()
{
	cout << "Destructor: C3DModel_Fbx()" << endl;
	reset();
}

void C3DModel_Fbx::reset()
{
	C3DModel::reset();

	m_currentVertex = 0;
	m_currentNormal = 0;
	m_currentUV = 0;
	m_currentFace = 0;
}

//bool C3DModel_Fbx::readFbxFile(const char * const filename, bool countOnly)
//{
//	ifstream infile;
//	string lineBuffer;
//	bool readFileOk = true;
//	int lineNumber = 0;
//	infile.open(filename);
//
//	while (!infile.eof())
//	{
//		getline(infile, lineBuffer);
//		lineNumber++;
//
//		if (!(this->parseFbxLine(lineBuffer, countOnly, lineNumber)))
//		{
//			readFileOk = false;
//			break;
//		}
//	}
//
//	infile.close();
//
//	return readFileOk;
//}

bool C3DModel_Fbx::parseFbxLine(const char * const filename, bool countOnly)
{
	char* token = nullptr;
	char* nextToken = nullptr;
	char *nextToken2 = nullptr;
	char delimiterToken = '\t';
	char nextdelimiterToken = '*';
	bool read_file = true;
	int number;
	int line_number = 0;

	float Number2;

	int Index = 0;
	//int NumFaceNormal;

	ifstream in_file;
	string line_buffer;

	//Abrimos el archivo
	in_file.open(filename);

	while (!in_file.eof()) //minentras no se acabe el arcivo seguimos leyendo
	{
		getline(in_file, line_buffer); //le damos a line_buffer el valor de la linea actual

		line_number++; //aumentamos la linea

		token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);//se le da un valor a token y nextTokend

		if (token != NULL)
		{
			//*Vertices*//
			if (0 == strcmp(token, "Vertices:"))
			{
				line_buffer = nextToken;
				token = strtok_s((char *)line_buffer.c_str(), "*", &nextToken);
				line_buffer = token;
				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);
				//aqui token te da la cantidad de datos de los vertices
				//aqui se transforma token en int para darle tamaño al arreglo
				istringstream iss(token);
				iss >> number;

				//Numero total de vertices
				m_currentVertex = number / 3;

				//se crea un array del numero total de vertices
				m_verticesRaw = new float[number];
				getline(in_file, line_buffer);
				line_number++;

				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);
				if (0 == strcmp(token, "a:"))
				{
					while (token != NULL)
					{
						token = strtok_s(nextToken, ",", &nextToken2);
						
						//agregar el dato a la lista
						if (Index < number)
						{
							istringstream iss2(token);
							iss2 >> Number2;

							m_verticesRaw[Index++] = Number2;
						}
						else
						{
							token = NULL;
						}
						if (*nextToken2 == NULL || *nextToken2 == ' ' || *nextToken2 == '\n' || *nextToken2 == '\0')
						{
							getline(in_file, line_buffer);
							line_number++;
							if (line_buffer == "} ")
							{
								break;
							}
							nextToken2 = (char *)line_buffer.c_str();
						}
						//aqui va la lista de lo que se va ha guardar
						nextToken = nextToken2;
					}
					//se resetea el index
					Index = 0;
				}
			}

			//*Faces*//
			else if (0 == strcmp(token, "PolygonVertexIndex:"))
			{
				line_buffer = nextToken;
				token = strtok_s((char *)line_buffer.c_str(), "*", &nextToken);
				line_buffer = token;
				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);
				//aqui token te da la cantidad de datos de los vertices
				//aqui se transforma token en int para darle tamaño al arreglo
				istringstream iss(token);
				iss >> number;

				m_currentFace = number / 3;
				m_vertexIndices = new unsigned short[number]; //se crea array con el numero total de caras
				getline(in_file, line_buffer);
				line_number++;

				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);
				if (0 == strcmp(token, "a:"))
				{
					while (token != NULL)
					{
						token = strtok_s(nextToken, ",", &nextToken2);

						//agregar el dato a la lista

						if (Index < number)
						{
							istringstream iss2(token);
							iss2 >> Number2;

							if (Number2 < 0)
							{
								Number2 = (Number2*-1) - 1;
							}
							m_vertexIndices[Index++] = Number2;
						}
						else
						{
							token = NULL;
						}
						if (*nextToken2 == NULL || *nextToken2 == ' ' || *nextToken2 == '\n' || *nextToken2 == '\0')
						{
							getline(in_file, line_buffer);
							line_number++;
							if (line_buffer == "} ")
							{
								break;
							}
							nextToken2 = (char *)line_buffer.c_str();
						}
						//aqui va la lista de lo que se va ha guardar
						nextToken = nextToken2;
					}
					//se resetea el index
					Index = 0;
				}
			}

			//*Normal*//
			else if (0 == strcmp(token, "Normals:"))
			{
				line_buffer = nextToken;
				token = strtok_s((char *)line_buffer.c_str(), "*", &nextToken);
				line_buffer = token;
				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);
				//aqui token te da la cantidad de datos de los vertices
				//aqui se transforma token en int para darle tamaño al arreglo
				istringstream iss(token);
				iss >> number;

				m_currentNormal = (number / 3) / 3;

				m_normalsRaw = new float[number];
				getline(in_file, line_buffer);
				line_number++;

				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);
				if (0 == strcmp(token, "a:"))
				{
					while (token != NULL)
					{
						token = strtok_s(nextToken, ",", &nextToken2);

						//agregar el dato a la lista

						if (Index < number)
						{
							istringstream iss2(token);
							iss2 >> Number2;

							m_normalsRaw[Index++] = Number2;
						}
						else
						{
							token = NULL;
						}
						if (*nextToken2 == NULL || *nextToken2 == ' ' || *nextToken2 == '\n' || *nextToken2 == '\0')
						{
							getline(in_file, line_buffer);
							line_number++;
							if (line_buffer == "} ")
							{
								break;
							}
							nextToken2 = (char *)line_buffer.c_str();
						}
						//aqui va la lista de lo que se va ha guardar
						nextToken = nextToken2;
					}
					//se resetea el index
					Index = 0;
				}
			}

			//*UV*//
			else if (0 == strcmp(token, "UV:"))
			{
				line_buffer = nextToken;
				token = strtok_s((char *)line_buffer.c_str(), "*", &nextToken);
				line_buffer = token;
				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);
				//aqui token te da la cantidad de datos de los vertices
				//aqui se transforma token en int para darle tamaño al arreglo
				istringstream iss(token);
				iss >> number;

				m_currentUV = number / 2;

				m_uvCoordsRaw = new float[number];
				getline(in_file, line_buffer);
				line_number++;

				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);
				if (0 == strcmp(token, "a:"))
				{
					while (token != NULL)
					{
						token = strtok_s(nextToken, ",", &nextToken2);

						//agregar el dato a la lista

						if (Index < number)
						{
							istringstream iss2(token);
							iss2 >> Number2;

							m_uvCoordsRaw[Index++] = Number2;
						}
						else
						{
							token = NULL;
						}
						if (*nextToken2 == NULL || *nextToken2 == ' ' || *nextToken2 == '\n' || *nextToken2 == '\0')
						{
							getline(in_file, line_buffer);
							line_number++;
							if (line_buffer == "} ")
							{
								break;
							}
							nextToken2 = (char *)line_buffer.c_str();
						}
						//aqui va la lista de lo que se va ha guardar
						nextToken = nextToken2;
					}
					//se resetea el index
					Index = 0;
				}
			}
			//*FaceUV*//
			else if (0 == strcmp(token, "UVIndex:"))
			{
				line_buffer = nextToken;

				token = strtok_s((char *)line_buffer.c_str(), "*", &nextToken);
				line_buffer = token;
				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);
				//aqui token te da la cantidad de datos de los vertices


				//aqui se transforma token en int para darle tamaño al arreglo
				istringstream iss(token);
				iss >> number;

				m_currentUV_Index = number / 3;

				m_UVindices = new unsigned short[number];
				getline(in_file, line_buffer);
				line_number++;

				token = strtok_s((char *)line_buffer.c_str(), " ", &nextToken);
				if (0 == strcmp(token, "a:"))
				{
					while (token != NULL)
					{
						token = strtok_s(nextToken, ",", &nextToken2);

						//agregar el dato a la lista

						if (Index < number)
						{
							istringstream iss2(token);
							iss2 >> Number2;

							m_UVindices[Index++] = Number2;
						}
						else
						{
							token = NULL;
						}
						if (*nextToken2 == NULL || *nextToken2 == ' ' || *nextToken2 == '\n' || *nextToken2 == '\0')
						{
							getline(in_file, line_buffer);
							line_number++;
							if (line_buffer == "} ")
							{
								break;
							}
							nextToken2 = (char *)line_buffer.c_str();
						}
						//aqui va la lista de lo que se va ha guardar
						nextToken = nextToken2;
					}
					//se resetea el index
					Index = 0;
				}
			}
		}

	}

	in_file.close();
	return true;
}

bool C3DModel_Fbx::readMtllib(std::string mtlLibFilename)
{
	return false;
}



bool C3DModel_Fbx::loadFromFile(const char * const filename)
{
	bool readFileOk = false;

	// Free any previous resources
	reset();

	// First pass is to count the number of vertices, normals, UVs, faces
	readFileOk = parseFbxLine(filename, true);

	m_numVertices = m_currentVertex;
	m_numNormals = m_currentNormal;
	m_numUVCoords = m_currentUV;
	m_numFaces = m_currentFace;

	// Display count
	cout << "Finished reading 3D model" << endl;
	cout << "Vertices: " << m_numVertices << endl;
	cout << "Normals: " << m_numNormals << endl;
	cout << "UVCoords: " << m_numUVCoords << endl;
	cout << "Faces: " << m_numFaces << endl;

	if (readFileOk)
	{
		// Check for MAX number of faces
		if (m_numVertices >= 65535 || m_numNormals >= 65535 || m_numUVCoords >= 65535)
		{
			cout << "Error: object cannot have more than 65535 vertices" << endl;
			cout << "Object attempted to load has: " << m_numVertices << " vertices" << endl;
			return false;
		}

		// If model was read without normals or UVCoords, we'll set a default value for them
		// i.e.:
		//   0,0 for UV coords
		//   face normal for normal
		if (m_numNormals == 0)
		{
			m_modelHasNormals = false;
			m_numNormals = m_numVertices;
		}
		else
		{
			m_modelHasNormals = true;
		}

		if (m_numUVCoords == 0)
		{
			m_numUVCoords = m_numVertices;
			m_modelHasUVs = false;
		}
		else
		{
			m_modelHasUVs = true;
		}


		// Zero-out indices arrays
		//memset(m_vertexIndices, 0, sizeof(unsigned short) * m_numFaces * 3);
		////memset(m_normalIndices, 0, sizeof(unsigned short) * m_numFaces * 3);
		//memset(m_UVindices, 0, sizeof(unsigned short) * m_numFaces * 3);
		//memset(m_verticesRaw, 0, sizeof(float) * m_numVertices * 3);
		//memset(m_normalsRaw, 0, sizeof(float) * m_numNormals * 3);
		//memset(m_uvCoordsRaw, 0, sizeof(float) * m_numUVCoords * 2);

		// Second pass is to read the data
		//readFileOk = parseFbxLine(filename, false);

		if (readFileOk)
		{
			m_modelGeometryLoaded = true;

			if (!m_modelHasNormals)
			{
				computeFaceNormals();
			}
		}
	}
	else
	{
		cout << "Error ocurred while reading 3D model." << endl;
	}

	return readFileOk;
}


