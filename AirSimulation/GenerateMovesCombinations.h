
#ifndef __GENERATEMOVESCOMBINATIONS_H__
#define __GENERATEMOVESCOMBINATIONS_H__

#include "CommonDefs.h"
#include <vector>
#include <stack>
#include "smartpointer.h"

class CombinationTree;

typedef std::vector<CombinationTree*> CombinationChildren;


class CombinationTree
{
public:
	CombinationTree()
	{
	}

	CombinationTree(CombinationTree* parent, int action, int level)
	{
		m_parent = parent;
		m_action = action;
		m_level = level;
	}

	CombinationTree* getParent() { return m_parent;}

	int getAction() { return m_action; }

	void AddChild(CombinationTree* child) { m_children.push_back( child); }

	CombinationChildren m_children;
	int m_level;

private:
	CombinationTree *m_parent;
	int m_action;
	
};

class GenerateCombinations
{
public:
	GenerateCombinations( ) : m_numberOfPlayers(0), m_numberOfMoves(0)
	{
	}

	GenerateCombinations( int numberOfPlayers, int numberOfMoves)
	{
		m_numberOfPlayers = numberOfPlayers;
		m_numberOfMoves = numberOfMoves;
	}

	std::vector<std::vector< int>>* generate();

private:
	int m_numberOfPlayers, 
		m_numberOfMoves;

};

#endif
