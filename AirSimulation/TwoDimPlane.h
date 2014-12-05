
#ifndef _TWODIMPLANE_H_
#define _TWODIMPLANE_H_

#include <vector>
#include "smartpointer.h"
#include "AirObjects.h"

struct PositionOfObject
{
	int id;
	union
	{
		Aircraft *pObject;
		Missile  *pObjectMissile;
	};
	FlyingObjectType type;
};

class Node;

class Plane
{
public:
	
	Plane() : m_maxX(500000), m_maxY(500000), m_id(0) { }

	Plane( Plane* plane);

	Plane(	int maxX,
			int maxY)
	{
		m_maxX = maxX;
		m_maxY = maxY;
		m_id = 0;
		m_myNode = NULL;
	}
	
	Plane* Init( Plane* plane);

	Plane* Init(	int maxX,
					int maxY)
	{
		m_maxX = maxX;
		m_maxY = maxY;
		m_id = 0;
		m_myNode = NULL;
		return this;
	}

	//operations
	bool moveLeft( Aircraft& pObject);
	bool moveRight( Aircraft& pObject);
	bool moveStraight( Aircraft& pObject);
	void drawPlane(Graphics *graphics, bool drawBVR, bool drawWVR );

	//Add or remove objects in the plane
	bool AddObjectOnPlane( Aircraft* pObject, FlyingObjectType type);
	bool AddObjectOnPlane( Missile*  pObject, FlyingObjectType type);
	bool RemoveObjectFromPlane(int id);

	std::vector<PositionOfObject>* getObjectsInPlane() { return &m_objectsInPlane; }
	std::vector<PositionOfObject>& getMissilesInPlane() { return m_missilesInPlane; }

	//Plane& operator=(Plane& plane);

	~Plane() {
		for (int l = 0 ; l < m_objectsInPlane.size() ; l++)
		{
			if ( m_objectsInPlane[l].pObject != NULL )
				delete m_objectsInPlane[l].pObject;
		}
		//m_objectsInPlane.clear();
	}

	void DestroyResources()
	{
		m_objectsInPlane.clear();
		m_objectsInPlane.resize(0);
	}

	//Limitation points
	PPoint getMinPoint();
	PPoint getMaxPoint();

	Node *m_myNode;

private:
	int m_maxX, m_maxY, m_id;
	
	std::vector<PositionOfObject> m_objectsInPlane;
	std::vector<PositionOfObject> m_missilesInPlane;
};

class CalculateACRelation
{
public:
	CalculateACRelation(Plane *plane, Aircraft *aircraft)
	{
		m_plane = plane;
		m_aircraft = aircraft;
	}

	void execute();

	float heuristicValue();

private:
	Plane *m_plane;
	Aircraft *m_aircraft;
};

#endif

