#include "stdafx.h"
#include "Node.h"
#include "GenerateMovesCombinations.h"

extern int levelsInOneStep;

//************************************
//Definition of Node
//************************************
Node::Node( Node* pParent, Plane* xyPlane, int level)
{
	NodeAllocator* instance = NodeAllocator::Instance();
	m_parent = pParent;
	m_xyPlane = instance->getPlane()->Init(xyPlane);
	m_level = level;
}

Node* Node::Init( Node* pParent, Plane* xyPlane, int level)
{
	NodeAllocator* instance = NodeAllocator::Instance();
	
	m_parent = pParent;
	m_xyPlane = instance->getPlane()->Init(xyPlane);
	m_level = level;
	m_xyPlane->m_myNode = this;
	if ( xyPlane->m_myNode != NULL)
		m_collectiveHeuristic = xyPlane->m_myNode->m_collectiveHeuristic;
	else
		m_collectiveHeuristic = 1.0;

	return this;
}

bool sortfunction (Node* i,Node* j) { return (i->m_collectiveHeuristic > j->m_collectiveHeuristic ); }

void Node::expandNode(AllPossibleMoves *moves, AllPossibleMoves *optionalMoves)
{
	//Expand it in as many possible combinations and create children of current node.
	NodeAllocator* instance = NodeAllocator::Instance();
	
	for ( unsigned int i = 0; i < moves->size() ; i++ )
	{
		Node *node = instance->getNode()->Init( this, m_xyPlane, m_level + 1);
		
		bool allMovesSame = true;

		//Make copy of objects in plane too

		std::vector<PositionOfObject>* childObjects = node->m_xyPlane->getObjectsInPlane();

		for ( unsigned int j = 0; j < (*moves)[i].size() ; j++ )
		{
			if ((*childObjects)[j].type == FlyingObjectType::TypeAircraft)
			{
				ACOperationFunctions[(*moves)[i][j]]( (*childObjects)[j].pObject);
			}
		}

		/*allMovesSame = true;

		for ( unsigned int n = 0 ; n < node->m_xyPlane->getObjectsInPlane()->size() ; n++)
		{
			CalculateACRelation objRelations( node->m_xyPlane, (*node->m_xyPlane->getObjectsInPlane())[n].pObject );
			objRelations.execute();

			node->m_collectiveHeuristic = node->m_collectiveHeuristic * (*node->m_xyPlane->getObjectsInPlane())[n].pObject->getDefenceHeuristicValue();
		}*/
		
		for (unsigned int l = 0 ; l < optionalMoves->size() ; l++)
		{
			Node *opNode = instance->getNode()->Init(this, node->m_xyPlane, m_level + 1);

			std::vector<PositionOfObject>* opChildObjects = opNode->m_xyPlane->getObjectsInPlane();
			
			allMovesSame = true;

			for (unsigned int m = 0 ; m < (*optionalMoves)[l].size() ; m++)
			{
				if ((*opChildObjects)[m].type == FlyingObjectType::TypeAircraft)
				{
					ACOptionalFunctions[(*optionalMoves)[l][m]]( (*opChildObjects)[m].pObject);
				}
			}
			
			//Calculate relations before adding the new child
			for ( unsigned int k = 0 ; k < opNode->m_xyPlane->getObjectsInPlane()->size() ; k++ )
			{
				if ((*opNode->m_xyPlane->getObjectsInPlane())[k].type == FlyingObjectType::TypeAircraft)
				{
					CalculateACRelation obj( opNode->m_xyPlane, 
											 (*opNode->m_xyPlane->getObjectsInPlane())[k].pObject );
					obj.execute();

					opNode->m_collectiveHeuristic = opNode->m_collectiveHeuristic * 
													(*opNode->m_xyPlane->getObjectsInPlane())[k].pObject->getDefenceHeuristicValue() *
													(*opNode->m_xyPlane->getObjectsInPlane())[k].pObject->getAttackHeuristicValue();
				}
			}

			this->m_childs.push_back(opNode);
		}
		
		
		//this->m_childs.push_back( node);
		instance->ReleaseNode( node);

	}
	
	std::sort( this->m_childs.begin(), this->m_childs.end(), sortfunction);

	if (m_childs.size() > 1)
	{
		while ( ( m_childs.begin() + 1) != m_childs.end() )
		{
			instance->ReleaseNode( *(m_childs.begin() + 1));
			m_childs.erase( m_childs.begin() + 1);
		}
	}

	/*std::sort( m_childs.begin(), m_childs.end(), sortfunction);
	if (m_childs.size() > 3 )
	{
		while ( ( m_childs.begin() +3 ) != m_childs.end())
		{
			instance->ReleaseNode( *(m_childs.begin() + 3));
			m_childs.erase(m_childs.begin() + 3);
		}
	}
	m_childs.resize(3);*/
}


//**************************************
//Definition of LookAhead
//**************************************

void LookAhead::Run( Node* node, Children* generations, int levels, AllPossibleMoves *moves, AllPossibleMoves *optionalMoves)
{
	std::stack<Node*> nodesToBeExpanded;

	nodesToBeExpanded.push( node);
	wchar_t dbgMessage[64];
	int count = 0;
	int topLevel = node->m_level;

	while ( !nodesToBeExpanded.empty() )
	{
		Node* currentNode = nodesToBeExpanded.top();

		nodesToBeExpanded.pop();

		currentNode->expandNode( moves, optionalMoves);
		for (unsigned int i = 0 ; i < currentNode->m_childs.size() ; i++ )
		{
			generations[ currentNode->m_childs[i]->m_level - topLevel - 1].push_back( currentNode->m_childs[i] );
			count++;
			if ( (currentNode->m_childs[i]->m_level - topLevel ) < (levels ) )
			{
				nodesToBeExpanded.push( currentNode->m_childs[i]);
			}
		}
	}
}


void g(Children* generations)
{
	NodeAllocator* instance = NodeAllocator::Instance();
	std::vector<Aircraft*> pAircraft;
	pAircraft.resize( generations[levelsInOneStep-1][0]->m_xyPlane->getObjectsInPlane()->size() );

	for (int k = 0 ; k < pAircraft.size() ; k++ )
	{
		pAircraft[k] =NULL;
	}

	for ( int i = 0 ; i < generations[levelsInOneStep-1].size() ; i++)
	{
		std::vector<PositionOfObject>* pAircrafts = generations[levelsInOneStep-1][i]->m_xyPlane->getObjectsInPlane();
		for (int j = 0 ; j < pAircrafts->size() ; j++)
		{
			if ( pAircraft[j] == NULL )
			{
				pAircraft[j] = (*pAircrafts)[j].pObject;
			}

			if (pAircraft[j] != NULL)
			{
				if ( pAircraft[j]->getDefenceHeuristicValue() < 
										(*pAircrafts)[j].pObject->getDefenceHeuristicValue() )
				{
					pAircraft[j] = (*pAircrafts)[j].pObject;
				}
			}
		}
	}

	Plane *plane = instance->getPlane()->Init( 500000, 500000);
	for ( int l = 0 ; l < pAircraft.size() ; l++ )
	{
		Aircraft* p = instance->getAircraft();
		int steps = levelsInOneStep;

		if (steps > 1)
		{
			while ( steps != 1)
			{
				pAircraft[l] = (*pAircraft[l]->m_myPlane->m_myNode->m_parent->m_xyPlane->getObjectsInPlane())[l].pObject;
				steps--;
			}
		}

		p->Init( pAircraft[l]);

		plane->AddObjectOnPlane( p, FlyingObjectType::TypeAircraft);
	}

	//Plane *plane = pAircraft[1]->m_myPlane;
	
//	return plane;
}


Node* LookAhead::getBestPlane(Children* generations)
{
	NodeAllocator* instance = NodeAllocator::Instance();
	std::vector<Aircraft*> pAircraft;
	Node* bestNode;
	pAircraft.resize( generations[levelsInOneStep-1][0]->m_xyPlane->getObjectsInPlane()->size() );

	for (int k = 0 ; k < pAircraft.size() ; k++ )
	{
		pAircraft[k] =NULL;
	}

	/*for ( int i = 0 ; i < generations[levelsInOneStep-1].size() ; i++)
	{
		std::vector<PositionOfObject>* pAircrafts = generations[levelsInOneStep-1][i]->m_xyPlane->getObjectsInPlane();
		for (int j = 0 ; j < pAircrafts->size() ; j++)
		{
			if ( pAircraft[j] == NULL )
			{
				pAircraft[j] = (*pAircrafts)[j].pObject;
			}

			if (pAircraft[j] != NULL)
			{
				if ( pAircraft[j]->getHeuristicValue() < 
										(*pAircrafts)[j].pObject->getHeuristicValue() )
				{
					pAircraft[j] = (*pAircrafts)[j].pObject;
				}
			}
	 }*/

	std::vector<Node*> nodes = generations[levelsInOneStep-1];

	if (nodes.size() > 0)
		bestNode = nodes[0];

	for (int j = 1 ; j < nodes.size() ; j++)
	{
		if ( bestNode->m_collectiveHeuristic < 
					nodes[j]->m_collectiveHeuristic )
			{
				bestNode = nodes[j];
			}
	}
	

	//Plane *plane = instance->getPlane()->Init( bestNode->m_xyPlane);
	/*for ( int l = 0 ; l < pAircraft.size() ; l++ )
	{
		Aircraft* p = instance->getAircraft();*/
		int steps = levelsInOneStep;
		
		if (steps > 1 )
		{
			while ( steps != 1)
			{
				//pAircraft[l] = (*pAircraft[l]->m_myPlane->m_myNode->m_parent->m_xyPlane->getObjectsInPlane())[l].pObject;
				bestNode = bestNode->m_parent;
				steps--;
			}
		}

	/*	p->Init( pAircraft[l]);

		plane->AddObjectOnPlane( p, FlyingObjectType::TypeAircraft);
	}*/

	//Plane *plane = pAircraft[1]->m_myPlane;
	//Plane *plane = instance->getPlane()->Init( bestNode->m_xyPlane);

	return bestNode;
}

//**************************************
//NodeAllocator
//**************************************

NodeAllocator* NodeAllocator::m_instance = NULL ;

NodeAllocator::NodeAllocator()
{
	m_availableNodes = new std::queue<Node*>();
	m_availablePlanes = new std::queue<Plane*>();
	m_availableAircrafts = new std::queue<Aircraft*>();
	AllocateMemory();
}

NodeAllocator* NodeAllocator::Instance()
{
	if ( m_instance == NULL)
	{
		m_instance = new NodeAllocator();
	}

	return m_instance;
}

void NodeAllocator::AllocateMemory()
{
	Node*		pN;
	Plane*		pP;
	Aircraft*	pA;

	for (int i = 0 ; i < 1000000 ; i++)
	{
		pN = new Node();
		m_availableNodes->push( pN);

		pP = new Plane();
		m_availablePlanes->push( pP);
	}

	for (int i = 0;i < 5000000;i++)
	{
		pA = new Aircraft();
		m_availableAircrafts->push( pA );
	}
}

Node* NodeAllocator::getNode()
{
	if ( m_availableNodes->size() > 0 )
	{
		Node* p = m_availableNodes->front();
		m_availableNodes->pop();

		return p;
	}

	return NULL;
}

Plane* NodeAllocator::getPlane()
{
	if ( m_availablePlanes->size() > 0 )
	{
		Plane* p = m_availablePlanes->front();
		m_availablePlanes->pop();

		return p;
	}

	return NULL;
}

Aircraft* NodeAllocator::getAircraft()
{
	if ( m_availableAircrafts->size() > 0 )
	{
		Aircraft* p = m_availableAircrafts->front();
		m_availableAircrafts->pop();

		return p;
	}

	return NULL;
}

void NodeAllocator::ReleaseNode(Node* p)
{
	ReleasePlane( p->m_xyPlane);
	
	p->DestroyResources();

	m_availableNodes->push( p );
}

void NodeAllocator::ReleasePlane( Plane* p)
{
	std::vector<PositionOfObject>* objects = p->getObjectsInPlane();

	for (int i = 0; i < objects->size() ; i++)
	{
		ReleaseAircraft( (*objects)[i].pObject );
	}

	p->DestroyResources();

	m_availablePlanes->push( p);
}

void NodeAllocator::ReleaseAircraft( Aircraft* p)
{
	p->DestroyResources();
	
	m_availableAircrafts->push( p);
}
