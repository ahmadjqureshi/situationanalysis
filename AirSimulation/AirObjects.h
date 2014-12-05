
#ifndef _AIR_PLANE_H_
#define _AIR_PLANE_H_

#include "smartpointer.h"
#include "stdafx.h"
#include "CommonDefs.h"
#include <vector>

using namespace Gdiplus;

#define MAX_MISSILES 2

class Aircraft;
class Plane;

struct RelationToAircraftInRange
{
	Aircraft *aCraft;
	float opponentBearingAngle;
	float opponentOffAngle;
	float distance;
	float myBearingAngle;
	float myOffAngle;
	float velocityRatio;
	bool  doIhaveHigherVelocity;
	float distanceRatio;
	bool  towardsEachOther;
};

/*class FlyingObject
{
public:
	FlyingObject()
	{
	}

	FlyingObject( const FlyingObject& obj) {}

	//virtual ~FlyingObject() { }

	//****************
	//Helper functions
	//****************
	PPoint getCurrentPosition() { return m_position; }
	int getHeight() { return m_height; }
	HeadingDirection getHeadingDirection() { return m_headingDirection; }
	int getElevationAngle() { return m_elevationAngle; }
	void setPlaneReference( Plane* plane) { m_myPlane = plane; }

	//****************
	//Possible actions
	//****************
	bool moveRight() ;
	bool moveLeft() ;
	bool moveStraight() ;
	int getId() { return m_id; }
	void setId(int id) { m_id = id; }

protected:
	
	int m_speed ; // m/s
	PPoint m_position;
	HeadingDirection m_headingDirection;
	int m_height; // meters
	int m_elevationAngle;
	Plane *m_myPlane;
	int m_id;
};*/

class Missile //: public FlyingObject
{
public:

	Missile() { }

	Missile(const Missile& missile);

	Missile(PPoint initialPosition, 
			int speed, 
			HeadingDirection headingDirection,
			Aircraft* owner );
	
	//~Missile() { }

	Missile* operator=(const Missile& ac);

	void changeOwner(Aircraft *newOwner) { m_owner = newOwner ; }
	Aircraft* getOwner() { return m_owner; }

	//****************
	//Helper functions
	//****************
	PPoint getCurrentPosition() { return m_position; }
	int getHeight() { return m_height; }
	HeadingDirection getHeadingDirection() { return m_headingDirection; }
	int getElevationAngle() { return m_elevationAngle; }
	void setPlaneReference( Plane* plane) { m_myPlane = plane; }

	//****************
	//Possible actions
	//****************
	bool moveRight() ;
	bool moveLeft() ;
	bool moveStraight() ;
	int getId() { return m_id; }
	void setId(int id) { m_id = id; }

private:
	Aircraft *m_owner;
	int m_speed;
	HeadingDirection m_headingDirection;
	PPoint m_previousPosition;

	PPoint m_position;
	int m_height; // meters
	int m_elevationAngle;
	Plane *m_myPlane;
	int m_id;
};

class Aircraft //: public FlyingObject
{
public:

	Aircraft() : m_numberOfMissilesLeft(2), 
				 BVR(100), 
				 WVR(5), 
				 pen(NULL), 
				 image(NULL), 
				 m_elevationAngle(0) {  }

	Aircraft( Aircraft* ac);

	Aircraft(PPoint initialPosition, 
			 int m_speed, 
			 HeadingDirection headingDirection,
			 Team team, 
			 int height, 
			 Role role
			 );
	~Aircraft(void);

	Aircraft* Init( Aircraft* ac,
			 bool initHValue=false);

	Aircraft* Init(PPoint initialPosition, 
			 int m_speed, 
			 HeadingDirection headingDirection,
			 Team team, 
			 int height, 
			 Role role
			 );

	//****************
	//Calculate Range
	//****************
	bool isInBVR( PPoint targetPoint);
	bool isInWVR( PPoint targetPoint);
	
	//****************
	//Possible actions
	//****************
	bool fireOrShoot(HeadingDirection direction, int speed, PPoint target);
	
	//****************
	//Helper functions
	//****************
	void drawAirplane( Graphics *graphics, bool drawBVR, bool drawWVR );
	Team MyTeam() { return m_team; }
	void addRelation(RelationToAircraftInRange relation) { m_relations.push_back(relation);  }
	std::vector<RelationToAircraftInRange>* getRelations() { return &m_relations; }
	int getSpeed() { return m_speed; }
	int getBVR() { return BVR; }
	int getWVR() { return WVR; }
	Role getRole() { return m_role; }

	Aircraft& operator=(const Aircraft& ac);

	//****************
	//getters, setters and  Helper functions
	//****************
	PPoint getCurrentPosition() { return m_position; }
	int getHeight() { return m_height; }
	HeadingDirection getHeadingDirection() { return m_headingDirection; }
	int getElevationAngle() { return m_elevationAngle; }
	void setPlaneReference( Plane* plane) { m_myPlane = plane; }
	bool setSpeed(int speed);

	void setDefenceHeuristicValue(float val) { m_hDefenceValue = val ; }
	float getDefenceHeuristicValue() { return m_hDefenceValue; }

	void setAttackHeuristicValue(float val) { m_hAttackValue = val ; }
	float getAttackHeuristicValue() { return m_hAttackValue; }

	int getId() { return m_id; }
	void setId(int id) { m_id = id; }

	Missile* getMissile() { return m_missile; }
	void     setMissile(Missile* missile) { m_missile = missile; }

	float getHeadingAngle() { return m_headingAngle; }

	//****************
	//Possible actions
	//****************
	bool moveRight() ;
	bool moveLeft() ;
	bool moveStraight() ;
	
	void DestroyResources();

private:
	//Private methods
	bool PointInRange(PPoint currentPosition, PPoint pointTobeTested, int radius);

	//property variables
	int  m_range; //45 degrees in heading direction not used in circular range, may be in future in triangular or like range.
	Team m_team;
	Role m_role;
	Missile* m_missile;
	int m_numberOfMissilesLeft;
	int BVR ;
	int WVR ;
	Pen *pen;
	Gdiplus::Image *image;
	int m_axisAngle;
	std::vector<RelationToAircraftInRange> m_relations;

	int m_speed ; // m/s
	PPoint m_position;
	HeadingDirection m_headingDirection;
	float m_headingAngle;
	int m_height; // meters
	int m_elevationAngle;
public:
	Plane *m_myPlane;
private:
	int m_id;
	float m_hDefenceValue;
	float m_hAttackValue;
};

/*
	Aircraft operations
*/

class AircraftOperations
{
public:
	static bool moveLeft( Aircraft* object)
	{
		object->moveLeft();

		return true;
	}

	static bool moveRight( Aircraft* object)
	{
		object->moveRight();

		return true;
	}

	static bool moveStraight( Aircraft* object)
	{
		object->moveStraight();

		return true;
	}

	static bool moveFire( Aircraft* object)
	{
		PPoint p( 0, 0);

		bool fireSuccesful = object->fireOrShoot( object->getHeadingDirection(), 1000, p); 

		return fireSuccesful;
	}
	
	static bool moveDoNothing( Aircraft* object)
	{
		return true;
	}

	static bool moveIncreaseSpeed( Aircraft* object)
	{
		int speed = object->getSpeed();
		bool success = false;

		speed = speed + HIGHERINONESTEP;

		success = object->setSpeed( speed );

		return success;
	}

	static bool moveDecreaseSpeed( Aircraft* object)
	{
		int speed = object->getSpeed();
		bool success = false;

		speed = speed - HIGHERINONESTEP;

		success = object->setSpeed( speed );

		return success;
	}

	static bool moveTurnMissileToRight( Aircraft* object)
	{
		if ( object->getMissile() == NULL)
		{
			return false;
		}

		object->getMissile()->moveRight();

		return true;
	}

	static bool moveMissileToLeft( Aircraft* object)
	{
		if ( object->getMissile() == NULL)
		{
			return false;
		}

		object->getMissile()->moveLeft();

		return true;
	}

	static bool moveMissileToStraight( Aircraft* object)
	{
		if ( object->getMissile() == NULL)
		{
			return false;
		}

		object->getMissile()->moveStraight();

		return true;
	}

};

typedef bool (*AircraftOperationFunction)(Aircraft* object);

AircraftOperationFunction ACOperationFunctions[3] = {  AircraftOperations::moveLeft,
													   AircraftOperations::moveRight,
													   AircraftOperations::moveStraight
													};

const int operationFunctionCount = 3;

AircraftOperationFunction ACOptionalFunctions[3] = {   //AircraftOperations::moveFire,
													   AircraftOperations::moveDoNothing,
													   AircraftOperations::moveIncreaseSpeed,
													   AircraftOperations::moveDecreaseSpeed
													   //AircraftOperations::moveTurnMissileToRight,
													   //AircraftOperations::moveMissileToStraight,
													   //AircraftOperations::moveMissileToLeft 
												 };



#endif
