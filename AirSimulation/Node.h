
#ifndef _NODE_H_
#define _NODE_H_

#include <vector>
#include <stack>
#include <queue>

#include "CommonDefs.h"
#include "TwoDimPlane.h"
#include "AirObjects.h"


class Node;

typedef std::vector<Node*> Children;
typedef std::vector<std::vector< int>> AllPossibleMoves;

class Node
{
public:
	Node()
	{
	}

	Node ( Node* pParent, Plane* xyPlane, int level);

	Node* Init( Node* pParent, Plane* xyPlane, int level);

	~Node()
	{
		delete m_xyPlane;
	}

	void DestroyResources()
	{
		m_xyPlane = NULL;
		m_childs.clear();
		m_childs.resize(0);
	}

	void expandNode(AllPossibleMoves *moves, AllPossibleMoves *optionalMoves);

	Node *m_parent;
	Children m_childs;
	Plane* m_xyPlane;
	int  m_level;
	float m_collectiveHeuristic;

};

class LookAhead
{
public:
	LookAhead()
	{
	}

	void Run( Node* node, Children* generations, int levels, AllPossibleMoves *moves, AllPossibleMoves *optionalMoves);

	Node* getBestPlane(Children* generations);
};

/*
	Singleton memory allocator/deallocator
*/

class NodeAllocator
{
public:
	static NodeAllocator* Instance();

	Node*		getNode();
	Plane*		getPlane();
	Aircraft*	getAircraft();

	void ReleaseNode(Node* p );
	void ReleasePlane(Plane* p);
	void ReleaseAircraft(Aircraft* p);

private:
	NodeAllocator();
	void AllocateMemory();

	std::queue<Node*> *m_availableNodes;
	std::queue<Plane*> *m_availablePlanes;
	std::queue<Aircraft*> *m_availableAircrafts;
	
	static NodeAllocator* m_instance;
};

#endif
