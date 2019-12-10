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

void CQuadtreeNode::FirstDivide(int Lim, AABB_2D ParentBounds, CHexGrid C, int Fil, int Col, int SubActual, int MaxDiv)
{
	int TriCont = 0;
	for (int i = 0; i < Fil; i++)
	{
		for (int j = 0; j < Col; j++)
		{
			TriCont += C.HEXA[i][j].getTriangleCount();
		}
	}
	if ((Fil*Col)==1 || TriCont < Lim || SubActual >= MaxDiv)
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
				CVector3	Cor[6];
				for (int k = 1; k < 7; k++)
				{
					Cor[k] = cell.calePoint(cell.Position, k, C.CellSZ, C.Ponty);
				}
				//obtener las esquinas de la celda implementar
				if (ChildBound[0].Points(Cor))
				{
					ChildCells[0].push_back(&cell);
				}
				if (ChildBound[1].Points(Cor))
				{
					ChildCells[1].push_back(&cell);
				}
				if (ChildBound[2].Points(Cor))
				{
					ChildCells[2].push_back(&cell);
				}
				if (ChildBound[3].Points(Cor))
				{
					ChildCells[3].push_back(&cell);
				}
			}
			
		}

		h1->SubDivide(Lim, ChildBound[0], ChildCells[0], SubActual++, MaxDiv);
		h2->SubDivide(Lim, ChildBound[1], ChildCells[1], SubActual++, MaxDiv);
		h3->SubDivide(Lim, ChildBound[2], ChildCells[2], SubActual++, MaxDiv);
		h4->SubDivide(Lim, ChildBound[3], ChildCells[3], SubActual++, MaxDiv);
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

void CQuadtreeNode::SubDivide(int Limit, AABB_2D ParentBounds, std::vector<CHexGridCell*> Cell, int SubActual, int MaxDiv)
{
	int TriCont = 0;
	for (int i = 0; i < Cell.size(); i++)
	{
		TriCont += Cell[i]->getTriangleCount();
	}
	if (Cell.size() == 1 || TriCont < Limit || SubActual >= MaxDiv)
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

		for (int i = 0; i < Cell.size(); i++)
		{
			CHexGridCell  cell = *Cell.at(i);
			CVector3	Cor[6];
			for (int k = 1; k < 7; k++)
			{
				Corners[k] = cell.calePoint(cell.Position, k, 1.5, true);
			}
			//obtener las esquinas de la celda implementar
			if (ChildBound[0].Points(Cor))
			{
				ChildCells[0].push_back(&cell);
			}
			if (ChildBound[1].Points(Cor))
			{
				ChildCells[1].push_back(&cell);
			}
			if (ChildBound[2].Points(Cor))
			{
				ChildCells[2].push_back(&cell);
			}
			if (ChildBound[3].Points(Cor))
			{
				ChildCells[3].push_back(&cell);
			}
		}

		h1->SubDivide(Limit, ChildBound[0], ChildCells[0], SubActual++, MaxDiv);
		h2->SubDivide(Limit, ChildBound[1], ChildCells[1], SubActual++, MaxDiv);
		h3->SubDivide(Limit, ChildBound[2], ChildCells[2], SubActual++, MaxDiv);
		h4->SubDivide(Limit, ChildBound[3], ChildCells[3], SubActual++, MaxDiv);
	}
}

void CQuadtreeNode::SetLimits(AABB_2D Limits)
{
	bounds = Limits;

	
	
	Vertices[0] = bounds.corners[0].getX();
	Vertices[1] = bounds.corners[0].getY();
	Vertices[2] = bounds.corners[0].getZ();

	Vertices[3] = bounds.corners[1].getX();
	Vertices[4] = bounds.corners[1].getY();
	Vertices[5] = bounds.corners[1].getZ();
	
	Vertices[6] = bounds.corners[2].getX();
	Vertices[7] = bounds.corners[2].getY();
	Vertices[8] = bounds.corners[2].getZ();
	
	Vertices[9] = bounds.corners[3].getX();
	Vertices[10] = bounds.corners[3].getY();
	Vertices[11] = bounds.corners[3].getZ();

	Indices[0] = 0;
	Indices[1] = 1;
	Indices[2] = 2;

	Indices[3] = 1;
	Indices[4] = 3;
	Indices[5] = 2;

}

//Im A PICE OF SHIT
