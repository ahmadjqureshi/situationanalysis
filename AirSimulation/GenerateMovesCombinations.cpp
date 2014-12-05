
#include "GenerateMovesCombinations.h"
#include "Node.h"


AllPossibleMoves* GenerateCombinations::generate()
{
	std::vector<CombinationTree*> trees;
	std::vector<CombinationTree*> leafs;
	std::vector<CombinationTree*> expandedNodes;

	int j = 0;

	for (int i = 0; i < m_numberOfMoves ; i++)
	{
		CombinationTree*  tree = new CombinationTree(NULL, (PossibleActions)i, 0);
		std::stack<CombinationTree*> nodesTobeExpanded;
		
		trees.push_back( tree);
		
		nodesTobeExpanded.push( trees[i]);
		
		//Iterate until stack is not empty
		while ( !nodesTobeExpanded.empty() )
		{
				CombinationTree* currentParent = nodesTobeExpanded.top();

				nodesTobeExpanded.pop();
				
				for (int k = 0 ; k < m_numberOfMoves ; k++)
				{
					CombinationTree* temp = new CombinationTree( currentParent, (PossibleActions)k, currentParent->m_level + 1);
					currentParent->AddChild( temp );
					
					if ( temp->m_level == ( m_numberOfPlayers -1) )
					{
						//This is the leaf node do not expand it
						leafs.push_back(temp);
					}
					else
					{
						//This is not leaf node do expand it
						nodesTobeExpanded.push( temp);
						//Add it for memory release purpose
						expandedNodes.push_back( temp);
					}
				}
		}
	}

	AllPossibleMoves *moves = new AllPossibleMoves();

	for ( std::vector<CombinationTree*>::iterator it = leafs.begin() ; it != leafs.end() ; it++ )
	{
		CombinationTree *node = *it;
		std::vector< int>  vect;

		while ( node  != NULL)
		{
			vect.push_back(node->getAction());
			node = node->getParent();
		}

		moves->push_back( vect );
	}

	for ( int i = 0 ; i < expandedNodes.size() ; i++ )
	{
		delete expandedNodes[i];
		expandedNodes[i] = NULL;
	}

	for ( int i = 0 ; i < leafs.size() ; i++ )
	{
		delete leafs[i];
		leafs[i] = NULL;
	}

	for (int i = 0 ; i < trees.size() ; i++ )
	{
		delete trees[i];
		trees[i] = NULL;
	}

	leafs.clear();
	expandedNodes.clear();

	return moves;
}

