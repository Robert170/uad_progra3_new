#include "../stdafx.h"

#include <iostream>
using namespace std;
#include "../Include/CQuadtreeNode.h"


CQuadtreeNode::CQuadtreeNode()
{
	
}

CQuadtreeNode::~CQuadtreeNode()
{
}

void CQuadtreeNode::Subdivide()
{
}

void CQuadtreeNode::Render(COpenGLRenderer*Render, unsigned int ShaderID)
{
	float Col[3] = { 0,0.3f,0 };
	MathHelper::Matrix4 Matriz;
	CVector3 Position = { 0,0,0 };
	Matriz = MathHelper::SimpleModelMatrixRotationTranslation(0.0f, Position);
	Render->renderObject(&ShaderID, &VertexArrayObjetc, 0, 2, Col, &Matriz, COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES);
	if (h1 != nullptr && h2 != nullptr && h3 != nullptr && h4 != nullptr)
	{
		h1->Render(Render, ShaderID);
		h2->Render(Render, ShaderID);
		h3->Render(Render, ShaderID);
		h4->Render(Render, ShaderID);
	}
	
	/*if (Cam->isAABBVisible(bounds))
	{
		if (h1 != nullptr)
		{
			h1->Render(Cam, m_Result);
			h2->Render(Cam, m_Result);
			h3->Render(Cam, m_Result);
			h4->Render(Cam, m_Result);
		}
		else
		{
			for (int i = 0; i < m_Data.size(); i++)
			{
				m_Result->push_back(m_Data[i]);
			}
		}
	}*/
}


void CQuadtreeNode::LimitSetCorner()
{
}

void CQuadtreeNode::LimitGetCorner()
{
}

void CQuadtreeNode::SetBound(AABB_2D Datos)
{
	bounds = Datos;
}

void CQuadtreeNode::FirstDivide(int Lim, AABB_2D ParentBounds, CHexGrid C, int Fil, int Col)
{
	int TriCont = 0;
	for (int i = 0; i < Fil; i++)
	{
		for (int j = 0; j < Col; j++)
		{
			TriCont += C.HEXA[i][j].getTriangleCount();
		}
	}
	if ((Fil*Col)==1 || TriCont < Lim)
	{
		//Copiar elementos de Cells a m_Data
		D = C.HEXA;
		h1 = h2 = h3 = h4 = nullptr;
	}
	else
	{
		CVector3 Corners[4];
		AABB_2D ChildBound[4];
		for (int i = 0; i < 4; i++)
		{
			Corners[i] = ParentBounds.Getcorners(i);
		}
		Corners[1].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[3].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[2].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		Corners[3].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		ChildBound[0].Setcorners(Corners); /////falta implementarlo otras 3 veces

		for (int i = 0; i < 4; i++)
		{
			Corners[i] = ParentBounds.Getcorners(i);
		}
		Corners[0].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[2].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[3].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		Corners[2].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		ChildBound[1].Setcorners(Corners);

		for (int i = 0; i < 4; i++)
		{
			Corners[i] = ParentBounds.Getcorners(i);
		}
		Corners[3].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[1].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[0].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		Corners[1].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		ChildBound[2].Setcorners(Corners);

		for (int i = 0; i < 4; i++)
		{
			Corners[i] = ParentBounds.Getcorners(i);
		}
		Corners[2].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[0].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[1].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		Corners[0].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		ChildBound[3].Setcorners(Corners);

		h1 = new CQuadtreeNode();
		h1->SetLimits(ChildBound[0]);
		h2 = new CQuadtreeNode();
		h2->SetLimits(ChildBound[1]);
		h3 = new CQuadtreeNode();
		h3->SetLimits(ChildBound[2]);
		h4 = new CQuadtreeNode();
		h4->SetLimits(ChildBound[3]);

		std::vector<CHexGridCell*>ChildCells[4];

		for (int i = 0; i < Fil; i++)
		{
			for (int j = 0; j < Col; j++)
			{
				CHexGridCell  cell = C.HEXA[i][j];
				CVector3	Corners[4];
				for (int k = 1; k < 7; k++)
				{
					cell.calePoint(cell.Position, k, C.CellSZ, C.Ponty);
				}
				//obtener las esquinas de la celda implementar
				if (ChildBound[0].Points(Corners))
				{
					ChildCells[0].push_back(&cell);
				}
				if (ChildBound[1].Points(Corners))
				{
					ChildCells[1].push_back(&cell);
				}
				if (ChildBound[2].Points(Corners))
				{
					ChildCells[2].push_back(&cell);
				}
				if (ChildBound[3].Points(Corners))
				{
					ChildCells[3].push_back(&cell);
				}
			}
			
		}

		h1->SubDivide(Lim, ChildBound[0], &ChildCells[0]);
		h2->SubDivide(Lim, ChildBound[1], &ChildCells[1]);
		h3->SubDivide(Lim, ChildBound[2], &ChildCells[2]);
		h4->SubDivide(Lim, ChildBound[3], &ChildCells[3]);
	}
}

bool CQuadtreeNode::Carga(COpenGLRenderer * Render, unsigned int ShaderID)
{
	bool Si_o_no = Render->allocateGraphicsMemoryForObject(&ShaderID, &VertexArrayObjetc, Vertices, 4, Normales, 2, UV, 4, Indices, 2, Indices_Normales, 2, Indices, 2);
	if (!Si_o_no)
	{
		Render->freeGraphicsMemoryForObject(&VertexArrayObjetc);
	}
	return Si_o_no;
}

void CQuadtreeNode::SubDivide(int Limit, AABB_2D ParentBounds, std::vector<CHexGridCell*>* Cell)
{
	int TriCont = 0;
	for (int i = 0; i < Cell->size(); i++)
	{
		//TriCont += Cell[i].getTriangleCont();
	}
	if (Cell->size() == 1 || TriCont < Limit)
	{
		//Copiar elementos de Cells a m_Data
		m_Data = Cell;
		h1 = h2 = h3 = h4 = nullptr;
	}
	else
	{
		CVector3 Corners[4];
		AABB_2D ChildBound[4];
		for (int i = 0; i < 4; i++)
		{
			Corners[i] = ParentBounds.Getcorners(i);
		}
		Corners[1].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[3].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[2].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		Corners[3].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		ChildBound[0].Setcorners(Corners); /////falta implementarlo otras 3 veces

		for (int i = 0; i < 4; i++)
		{
			Corners[i] = ParentBounds.Getcorners(i);
		}
		Corners[0].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[2].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[3].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		Corners[2].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		ChildBound[1].Setcorners(Corners); 

		for (int i = 0; i < 4; i++)
		{
			Corners[i] = ParentBounds.Getcorners(i);
		}
		Corners[3].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[1].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[0].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		Corners[1].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		ChildBound[2].Setcorners(Corners); 

		for (int i = 0; i < 4; i++)
		{
			Corners[i] = ParentBounds.Getcorners(i);
		}
		Corners[2].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[0].X = (Corners[1].getX() / 2) + (Corners[0].getX() / 2);
		Corners[1].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		Corners[0].Z = (Corners[1].getZ() / 2) + (Corners[0].getZ() / 2);
		ChildBound[3].Setcorners(Corners); 

		h1 = new CQuadtreeNode();
		h1->SetLimits(ChildBound[0]);
		h2 = new CQuadtreeNode();
		h2->SetLimits(ChildBound[1]);
		h3 = new CQuadtreeNode();
		h3->SetLimits(ChildBound[2]);
		h4 = new CQuadtreeNode();
		h4->SetLimits(ChildBound[3]);

		std::vector<CHexGridCell*>ChildCells[4];

		for (int i = 0; i < Cell->size(); i++)
		{
			CHexGridCell  cell = *Cell->at(i);
			CVector3	Corners[4];
			//obtener las esquinas de la celda implementar
			if (ChildBound[0].Points(Corners))
			{
				ChildCells[0].push_back(&cell);
			}
			if (ChildBound[1].Points(Corners))
			{
				ChildCells[1].push_back(&cell);
			}
			if (ChildBound[2].Points(Corners))
			{
				ChildCells[2].push_back(&cell);
			}
			if (ChildBound[3].Points(Corners))
			{
				ChildCells[3].push_back(&cell);
			}
		}

		h1->SubDivide(Limit, ChildBound[0], &ChildCells[0]);
		h2->SubDivide(Limit, ChildBound[1], &ChildCells[1]);
		h3->SubDivide(Limit, ChildBound[2], &ChildCells[2]);
		h4->SubDivide(Limit, ChildBound[3], &ChildCells[3]);
	}
}

void CQuadtreeNode::SetLimits(AABB_2D Limits)
{
	bounds = Limits;

	for (int i = 0; i < 12; i++)
	{
		Vertices[i] = bounds.corners[i].getX();
		Vertices[i] = bounds.corners[i].getY();
		Vertices[i] = bounds.corners[i].getZ();
	}

	
	Indices[0] = 0;
	Indices[1] = 1;
	Indices[2] = 2;

	Indices[3] = 1;
	Indices[4] = 3;
	Indices[5] = 2;

}

