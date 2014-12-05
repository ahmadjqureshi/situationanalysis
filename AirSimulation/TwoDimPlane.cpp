#include "stdafx.h"
#include "TwoDimPlane.h"
#include <math.h>
#include "Node.h"
#include "DecisionHelper.h"

//**********************
//Plane class definition
//**********************

Plane::Plane(Plane* plane)
{
	NodeAllocator* instance = NodeAllocator::Instance();
	int missilesCount=0;

	this->m_id = plane->m_id;
	this->m_maxX = plane->m_maxX;
	this->m_maxY = plane->m_maxY;
	
	this->m_objectsInPlane.resize( plane->getObjectsInPlane()->size() ); 
	this->m_missilesInPlane.resize( plane->getMissilesInPlane().size() );

	for (int i = 0 ; i < plane->getObjectsInPlane()->size() ; i++)
	{
		this->m_objectsInPlane[i].id = (*plane->getObjectsInPlane())[i].id;
		this->m_objectsInPlane[i].type = (*plane->getObjectsInPlane())[i].type;

		if ( (*plane->getObjectsInPlane())[i].type == FlyingObjectType::TypeAircraft)
		{
			this->m_objectsInPlane[i].pObject = instance->getAircraft()->Init( (*plane->getObjectsInPlane())[i].pObject );
			this->m_objectsInPlane[i].pObject->setPlaneReference( this);
			
			if ( (*plane->getObjectsInPlane())[i].pObject->getMissile() != NULL)
			{
				this->m_missilesInPlane[missilesCount].id = 
					(*plane->getObjectsInPlane())[i].pObject->getMissile()->getId();
				this->m_missilesInPlane[missilesCount].type = FlyingObjectType::TypeMissile;
				this->m_missilesInPlane[missilesCount].pObjectMissile = new Missile( *((*plane->getObjectsInPlane())[i].pObject->getMissile()) );
				this->m_missilesInPlane[missilesCount].pObjectMissile->changeOwner( (*plane->getObjectsInPlane())[i].pObject);
				this->m_missilesInPlane[missilesCount].pObjectMissile->setPlaneReference( this );
				this->m_objectsInPlane[i].pObject->setMissile( 
					this->m_missilesInPlane[missilesCount].pObjectMissile);
				missilesCount++;
			}
		}
	}
}

Plane* Plane::Init(Plane* plane)
{
	NodeAllocator* instance = NodeAllocator::Instance();
	int missilesCount=0;

	this->m_id = plane->m_id;
	this->m_maxX = plane->m_maxX;
	this->m_maxY = plane->m_maxY;
	
	this->m_objectsInPlane.resize( plane->getObjectsInPlane()->size() ); 
	this->m_missilesInPlane.resize( plane->getMissilesInPlane().size() );

	for (int i = 0 ; i < plane->getObjectsInPlane()->size() ; i++)
	{
		this->m_objectsInPlane[i].id = (*plane->getObjectsInPlane())[i].id;
		this->m_objectsInPlane[i].type = (*plane->getObjectsInPlane())[i].type;

		if ( (*plane->getObjectsInPlane())[i].type == FlyingObjectType::TypeAircraft)
		{
			this->m_objectsInPlane[i].pObject = instance->getAircraft()->Init( (*plane->getObjectsInPlane())[i].pObject, false );
			this->m_objectsInPlane[i].pObject->setPlaneReference( this);
			
			if ( (*plane->getObjectsInPlane())[i].pObject->getMissile() != NULL)
			{
				this->m_missilesInPlane[missilesCount].id = 
					(*plane->getObjectsInPlane())[i].pObject->getMissile()->getId();
				this->m_missilesInPlane[missilesCount].type = FlyingObjectType::TypeMissile;
				this->m_missilesInPlane[missilesCount].pObjectMissile = new Missile( *((*plane->getObjectsInPlane())[i].pObject->getMissile()) );
				this->m_missilesInPlane[missilesCount].pObjectMissile->changeOwner( (*plane->getObjectsInPlane())[i].pObject);
				this->m_missilesInPlane[missilesCount].pObjectMissile->setPlaneReference( this );
				this->m_objectsInPlane[i].pObject->setMissile( 
					this->m_missilesInPlane[missilesCount].pObjectMissile);
				missilesCount++;
			}
		}
	}

	return this;
}

/*Plane& Plane::operator=(Plane& plane)
{
	Plane* newPlane = new Plane( plane);
	return *newPlane;
}*/

bool Plane::moveLeft( Aircraft& pObject)
{
	return pObject.moveLeft();
}

bool Plane::moveRight( Aircraft& pObject)
{
	return pObject.moveRight();
}

bool Plane::moveStraight( Aircraft& pObject)
{
	return pObject.moveStraight();
}

bool Plane::AddObjectOnPlane(Aircraft*  pObject, FlyingObjectType type)
{
	PositionOfObject objectInPlane;
	
	objectInPlane.id = m_id;
	objectInPlane.pObject = pObject;
	objectInPlane.type = type;
	objectInPlane.pObject->setId( m_id );
	objectInPlane.pObject->setPlaneReference( this );

	m_id++;

	for (std::vector<PositionOfObject>::iterator i = m_objectsInPlane.begin() ; i != m_objectsInPlane.end() ; i++)
	{
		if ( ( i->pObject->getCurrentPosition().x == pObject->getCurrentPosition().x) &&
			 ( i->pObject->getCurrentPosition().y == pObject->getCurrentPosition().y) )
		{
			return false;
		}
	}

	m_objectsInPlane.push_back(objectInPlane);

	return true;
}

bool Plane::AddObjectOnPlane( Missile*  pObject, FlyingObjectType type)
{
	PositionOfObject objectInPlane;
	
	objectInPlane.id = m_id;
	objectInPlane.pObjectMissile = pObject;
	objectInPlane.type = type;
	objectInPlane.pObject->setId( m_id);
	objectInPlane.pObject->setPlaneReference( this);

	m_id++;

	m_missilesInPlane.push_back(objectInPlane);

	return true;
}

bool Plane::RemoveObjectFromPlane(int id)
{
	for (std::vector<PositionOfObject>::iterator i = m_objectsInPlane.begin() ; i != m_objectsInPlane.end() ; i++)
	{
		if ( i->id == id)
		{
			m_objectsInPlane.erase(i);
			return true;
		}
	}

	return false;
}

PPoint Plane::getMinPoint()
{
	PPoint minPoint(0 , 0);

	return minPoint;
}

PPoint Plane::getMaxPoint()
{
	PPoint maxPoint(m_maxX, m_maxY);

	return maxPoint;
}

void Plane::drawPlane( Graphics *graphics, bool drawBVR, bool drawWVR )
{
	for (std::vector<PositionOfObject>::iterator i = m_objectsInPlane.begin() ; i != m_objectsInPlane.end() ; i++)
	{
		if ( i->type == FlyingObjectType::TypeAircraft)
		{
			((Aircraft*)i->pObject)->drawAirplane( graphics, drawBVR, drawWVR);
		}
	}
	
}


//***************************
//CalculateACRelation
//***************************
void CalculateACRelation::execute()
{
	for ( unsigned int i = 0 ; i < m_plane->getObjectsInPlane()->size() ; i++)
	{
		if ( (*m_plane->getObjectsInPlane())[i].type == FlyingObjectType::TypeAircraft 
			&& ( (*m_plane->getObjectsInPlane())[i].pObject != m_aircraft) )
		{
			if ( m_aircraft->isInBVR( (*m_plane->getObjectsInPlane())[i].pObject->getCurrentPosition()) )
			{
				Aircraft* opponentAC = (*m_plane->getObjectsInPlane())[i].pObject;

				//if ( opponentAC->MyTeam() != m_aircraft->MyTeam())
				//{
					RelationToAircraftInRange relativePosition;

					relativePosition.towardsEachOther = false;

					relativePosition.aCraft = (Aircraft*)(*m_plane->getObjectsInPlane())[i].pObject;

					// calculate distance

					float tempDis = pow( m_aircraft->getCurrentPosition().x - opponentAC->getCurrentPosition().x , 2) +
									pow( m_aircraft->getCurrentPosition().y - opponentAC->getCurrentPosition().y, 2) +
									pow( m_aircraft->getHeight() - opponentAC->getHeight(), 2);

					relativePosition.distance = sqrt( tempDis);
					relativePosition.distance = relativePosition.distance ;

					float tempMyBearingAngle = ( opponentAC->getCurrentPosition().x - m_aircraft->getCurrentPosition().x) 
														* cos( ( m_aircraft->getElevationAngle() * PI) / 180.0f ) * cos( ( m_aircraft->getHeadingAngle() * PI ) / 180.0f )
											 + ( opponentAC->getCurrentPosition().y - m_aircraft->getCurrentPosition().y) 
														* cos( ( m_aircraft->getElevationAngle() * PI) / 180.0f ) * sin( ( m_aircraft->getHeadingAngle() * PI ) / 180.0f )
											 + ( opponentAC->getHeight() - m_aircraft->getHeight()) * sin( m_aircraft->getElevationAngle());
				
					tempMyBearingAngle = tempMyBearingAngle / relativePosition.distance ;

					relativePosition.myBearingAngle = acos( tempMyBearingAngle );
				
					float tempOpponentsBearingAngle = ( m_aircraft->getCurrentPosition().x - opponentAC->getCurrentPosition().x) 
														* cos( ( opponentAC->getElevationAngle() * PI) / 180.0f ) * cos( ( opponentAC->getHeadingAngle() * PI ) / 180.0f )
											 + ( m_aircraft->getCurrentPosition().y - opponentAC->getCurrentPosition().y) 
														* cos( ( opponentAC->getElevationAngle() * PI) / 180.0f ) * sin( ( opponentAC->getHeadingAngle() * PI ) / 180.0f )
											 + ( m_aircraft->getHeight() - opponentAC->getHeight()) * sin( opponentAC->getElevationAngle());

					tempOpponentsBearingAngle = tempOpponentsBearingAngle / relativePosition.distance;

					relativePosition.opponentBearingAngle = acos( tempOpponentsBearingAngle );

					float tempMyOffangle = ( opponentAC->getCurrentPosition().x - m_aircraft->getCurrentPosition().x) * cos( (opponentAC->getElevationAngle() *PI ) /180.0f) 
														* cos( ( opponentAC->getHeadingAngle() * PI ) / 180.0f )
											 + ( opponentAC->getCurrentPosition().y - m_aircraft->getCurrentPosition().y) * cos( (opponentAC->getElevationAngle() * PI ) /180.0f) 
														* sin( ( opponentAC->getHeadingAngle() * PI ) / 180.0f )
											 + ( opponentAC->getHeight() - m_aircraft->getHeight()) * sin( opponentAC->getElevationAngle());

					tempMyOffangle = tempMyOffangle / relativePosition.distance;
				
					relativePosition.myOffAngle = acos( tempMyOffangle );

					float tempOffAngle = ( m_aircraft->getCurrentPosition().x - opponentAC->getCurrentPosition().x) * cos( (m_aircraft->getElevationAngle() *PI ) /180.0) 
														* cos( ( m_aircraft->getHeadingAngle() * PI ) / 180 )
											 + ( m_aircraft->getCurrentPosition().y - opponentAC->getCurrentPosition().y) * cos( (m_aircraft->getElevationAngle() *PI ) /180.0) 
														* sin( ( m_aircraft->getHeadingAngle() * PI ) / 180 )
											 + ( m_aircraft->getHeight() - opponentAC->getHeight()) * sin( m_aircraft->getElevationAngle());

					tempOffAngle = tempOffAngle / relativePosition.distance;

					relativePosition.opponentOffAngle = acos( tempOffAngle);

					if ( m_aircraft->getSpeed() > opponentAC->getSpeed() )
					{
						relativePosition.doIhaveHigherVelocity = true;
						relativePosition.velocityRatio = opponentAC->getSpeed() / m_aircraft->getSpeed();
					}
					else
					{
						relativePosition.doIhaveHigherVelocity = false;
						relativePosition.velocityRatio = m_aircraft->getSpeed() / opponentAC->getSpeed();
					}

					relativePosition.distanceRatio = 
						relativePosition.distance / ( m_aircraft->getBVR() );

					float myOffAngleRatio = relativePosition.myOffAngle/PI;
					float opponentsOffAngleRatio = relativePosition.opponentOffAngle/PI;

					if ( myOffAngleRatio > 0.75 && opponentsOffAngleRatio > 0.75 )
					{
						relativePosition.towardsEachOther = true;
					}

					m_aircraft->addRelation( relativePosition);
				//}
			}
		}
	}

	heuristicValue();

}

float CalculateACRelation::heuristicValue()
{
	float prevhValue = 1.0f;
	std::vector<RelationToAircraftInRange>* relations = m_aircraft->getRelations();
	bool first =true;

	for (unsigned int i = 0 ; i < relations->size() ; i++)
	{
		float hValue = 1.0f;
		float mybearingAngleRatio = (*relations)[i].myBearingAngle / PI ;
		float opponentsbearingAngleRatio = (*relations)[i].opponentBearingAngle / PI ;
		int   distanceVariable = -1, positionVariable = -1;

		/*
			Find suitable position variable
		*/

		if ( mybearingAngleRatio > 0.7 && opponentsbearingAngleRatio < 0.3 ) //Very high Possibility that opponent is behind me
		{
			positionVariable = OPPONENT_BEHIND_ME;
		}
		else if ( opponentsbearingAngleRatio > 0.7 && mybearingAngleRatio < 0.3 ) // I am behind opponent
		{
			positionVariable = I_AM_BEHIND_OPPONENT;
		}
		else if ( (*relations)[i].towardsEachOther ) // towards each other
		{
			positionVariable = TOWARDS_EACHOTHER;
		}
		else if ( ( opponentsbearingAngleRatio > 0.4 && opponentsbearingAngleRatio < 0.6 ) &&
				  ( mybearingAngleRatio > 0.4 && mybearingAngleRatio < 0.6 ) )  // Parallel to each other
		{
			positionVariable = PARALLEL_TO_EACHOTHER;
		}
		else if ( mybearingAngleRatio > 0.6 && opponentsbearingAngleRatio > 0.6)
		{
			positionVariable = OPPOSITE_TO_EACHOTHER;
		}
		else // Other position or unknown position
		{
			positionVariable = OTHER_POSITION;
		}

		/*
			Find suitable distance variable
		*/
		
		if ( (*relations)[i].distanceRatio < 0.9 &&  (*relations)[i].distanceRatio > 0.4) //This is Medium distance
		{
			distanceVariable = MEDIUM_DISTANCE;
		}
		else if ((*relations)[i].distanceRatio > 0.9 )
		{
			distanceVariable = LONG_DISTANCE;
		}
		else
		{
			distanceVariable = SHORT_DISTANCE;
		}
		
		if ( (*relations)[i].aCraft->MyTeam() != m_aircraft->MyTeam())
		{
			float hValueAttack = DecisionHelper::ProbabilityOfNextStateAttackingMode( FIRE,
																					  NOT_FIRE, 
																					  distanceVariable, 
																					  positionVariable, 
																					  (*relations)[i].distanceRatio, 
																					  mybearingAngleRatio  );
			float hValueDefense =  DecisionHelper::ProbabilityOfNextStateDefensiveMode( NOT_FIRE,
																						NOT_FIRE, 
																						distanceVariable, 
																						positionVariable, 
																						(*relations)[i].distanceRatio, 
																						opponentsbearingAngleRatio );

			if (first)
			{
				first = false;
			}
			else
			{
				hValueDefense = hValueDefense * m_aircraft->getDefenceHeuristicValue();
				hValueAttack  =  hValueAttack  * m_aircraft->getAttackHeuristicValue();
			}

			m_aircraft->setDefenceHeuristicValue( hValueDefense );
			m_aircraft->setAttackHeuristicValue( hValueAttack );

			if ( (*relations)[i].distanceRatio < 0.2 || (*relations)[i].distanceRatio > 0.85 )
			{
				hValueDefense = 0; 
				hValueAttack  = 0;
				m_aircraft->setDefenceHeuristicValue(hValueDefense );
				m_aircraft->setAttackHeuristicValue( hValueAttack );
			}
			/*else if ( (*relations)[i].distanceRatio < 0.7 && (*relations)[i].distanceRatio > 0.6)
			{
				hValueDefense = hValueDefense / 2;
				hValueAttack  = hValueAttack  / 2;
			}*/
		}
		else
		{
			if ( (*relations)[i].distanceRatio < 0.2 || (*relations)[i].distanceRatio > 0.9 )
			{
				float hValueDefense = m_aircraft->getDefenceHeuristicValue() * 0.1; 
				float hValueAttack  = m_aircraft->getAttackHeuristicValue() * 0.1;
				m_aircraft->setDefenceHeuristicValue(hValueDefense );
				m_aircraft->setAttackHeuristicValue( hValueAttack );
			}
		}
	}
	
	return 1;
}
