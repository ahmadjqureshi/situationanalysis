
#ifndef _COMMON_DEFS_
#define _COMMON_DEFS_

#include <math.h>

#define PI 3.14159265

/*
  Based on following matrix we will define direction and direction changes, 
  the turning angle is 45 degrees

  NW N NE
   W * E
  SW S SE
*/
class NewCordinates;

#define MAXSPEEDAIRCRAFT 600 //m/s
#define HIGHERINONESTEP  50  //m/s
#define MINSPEEDAIRCRAFT 200 //m/s

#define NUMBEROFSECONDS 1

enum PossibleActions
{
	MoveLeft = 0,
	MoveRight,
	MoveStraight,
	MaxAction // Not real action just used for count
};

enum AircraftSpecificActions
{
	//Fire = 0,
	DoNothing = 0,
	IncreaseSpeed,
	DecreaseSpeed,
	//TurnMissileToRight,
	//MoveMissileStraight,
	//TurnMissileToLeft,
	MaxOptionalAction
};

enum HeadingDirection
{
	North = 0,
	NorthEast,
	East,
	SouthEast,
	South,
	SouthWest,
	West,
	NorthWest
};

struct HeadingAngle
{
	HeadingDirection direction;
	float			 startAngle;
};

///<summary>
///Heading angle used for image rotation only for presentation
///</summary>
HeadingAngle headingAngle[8] =
{
	{ North, 0.0f},
	{ NorthEast, 45.0f},
	{ East, 90.0f},
	{ SouthEast, 135.0f},
	{ South, 180.0f},
	{ SouthWest, 225.0f},
	{ West, 270.0f},
	{ NorthWest, 315.0f}
};

///<summary>
///Heading angle used for calculations
///</summary>
HeadingAngle actHeadingAngle[8] =
{
	{ North, 270.0f},
	{ NorthEast, 315.0f},
	{ East, 0.0f},
	{ SouthEast, 45.0f},
	{ South, 90.0f},
	{ SouthWest, 135.0f},
	{ West, 180.0f},
	{ NorthWest, 225.0f}
	
};



enum Team
{
	Red = 0,
	Blue
};

enum Role
{
	Attacker = 0,
	Defender
};

struct PPoint
{
	PPoint() { }
	PPoint(int X, int Y) { x = X; y = Y; }

	int x, y;
};

enum FlyingObjectType
{
	TypeAircraft = 0,
	TypeMissile
};



class NewCordinates
{
public:

	///<summary>
	///current point is current position
	///new point will be the new position after move
	///speed is in m/s
	///</summary>
	static void MoveNorth( PPoint currentPoint, PPoint& newPoint, int speed )
	{
		/*newPoint.x = currentPoint.x;
		newPoint.y = currentPoint.y - (int)(speed / (1000/OneKMNumberOfPixels));*/
		//int x=0, y = -(speed / (1000/OneKMNumberOfPixels));
		int x=0, y = -(speed );

		int newX = x *cos( headingAngle[North].startAngle *PI /180) - y *sin( headingAngle[North].startAngle *PI /180);

		int newY = x *sin( headingAngle[North].startAngle *PI /180) + y *cos( headingAngle[North].startAngle *PI /180);

		newPoint.x = currentPoint.x + newX;

		newPoint.y = currentPoint.y + newY;
	}

	///<summary>
	///current point is current position
	///new point will be the new position after move
	///speed is in m/s
	///</summary>
	static void MoveSouth( PPoint currentPoint, PPoint& newPoint, int speed)
	{
		/*newPoint.x = currentPoint.x;
		newPoint.y = currentPoint.y + (int)(speed / (1000/OneKMNumberOfPixels));*/
		//int x=0, y = -(speed / (1000/OneKMNumberOfPixels));
		int x=0, y = -(speed );

		int newX = x *cos( headingAngle[South].startAngle *PI /180) - y *sin( headingAngle[South].startAngle *PI /180);

		int newY = x *sin( headingAngle[South].startAngle *PI /180) + y *cos( headingAngle[South].startAngle *PI /180);

		newPoint.x = currentPoint.x + newX;

		newPoint.y = currentPoint.y + newY;
	}

	///<summary>
	///current point is current position
	///new point will be the new position after move
	///speed is in m/s
	///</summary>
	static void MoveEast( PPoint currentPoint, PPoint& newPoint, int speed)
	{
		/*newPoint.x = currentPoint.x + (int)(speed / (1000/OneKMNumberOfPixels));
		newPoint.y = currentPoint.y ;*/
		//int x=0, y = -(speed / (1000/OneKMNumberOfPixels));
		int x=0, y = -(speed );

		int newX = x *cos( headingAngle[East].startAngle *PI /180) - y *sin( headingAngle[East].startAngle *PI /180);

		int newY = x *sin( headingAngle[East].startAngle *PI /180) + y *cos( headingAngle[East].startAngle *PI /180);

		newPoint.x = currentPoint.x + newX;

		newPoint.y = currentPoint.y + newY;
	}

	///<summary>
	///current point is current position
	///new point will be the new position after move
	///speed is in m/s
	///</summary>
	static void MoveWest( PPoint currentPoint, PPoint& newPoint, int speed)
	{
		/*newPoint.x = currentPoint.x - (int)(speed / (1000/OneKMNumberOfPixels));
		newPoint.y = currentPoint.y ;*/
		//int x=0, y = -(speed / (1000/OneKMNumberOfPixels));
		int x=0, y = -(speed );

		int newX = x *cos( headingAngle[West].startAngle *PI /180) - y *sin( headingAngle[West].startAngle *PI /180);

		int newY = x *sin( headingAngle[West].startAngle *PI /180) + y *cos( headingAngle[West].startAngle *PI /180);

		newPoint.x = currentPoint.x + newX;

		newPoint.y = currentPoint.y + newY;
	}

	///<summary>
	///current point is current position
	///new point will be the new position after move
	///speed is in m/s
	///</summary>
	static void MoveNorthEast( PPoint currentPoint, PPoint& newPoint, int speed)
	{
		/*newPoint.x = currentPoint.x + (int)(speed / (1000/OneKMNumberOfPixels));
		newPoint.y = currentPoint.y -  (int)(speed / (1000/OneKMNumberOfPixels));*/
		//int x=0, y = -(speed / (1000/OneKMNumberOfPixels));
		int x=0, y = -(speed );

		int newX = x *cos( headingAngle[NorthEast].startAngle *PI /180) - y *sin( headingAngle[NorthEast].startAngle *PI /180);

		int newY = x *sin( headingAngle[NorthEast].startAngle *PI /180) + y *cos( headingAngle[NorthEast].startAngle *PI /180);

		newPoint.x = currentPoint.x + newX;

		newPoint.y = currentPoint.y + newY;
	}

	///<summary>
	///current point is current position
	///new point will be the new position after move
	///speed is in m/s
	///</summary>
	static void MoveSouthEast( PPoint currentPoint, PPoint& newPoint, int speed)
	{
		/*newPoint.x = currentPoint.x + (int)(speed / (1000/OneKMNumberOfPixels));
		newPoint.y = currentPoint.y +  (int)(speed / (1000/OneKMNumberOfPixels));*/
		//int x=0, y = -(speed / (1000/OneKMNumberOfPixels));
		int x=0, y = -(speed );

		int newX = x *cos( headingAngle[SouthEast].startAngle *PI /180) - y *sin( headingAngle[SouthEast].startAngle *PI /180);

		int newY = x *sin( headingAngle[SouthEast].startAngle *PI /180) + y *cos( headingAngle[SouthEast].startAngle *PI /180);

		newPoint.x = currentPoint.x + newX;

		newPoint.y = currentPoint.y + newY;
	}

	///<summary>
	///current point is current position
	///new point will be the new position after move
	///speed is in m/s
	///</summary>
	static void MoveSouthWest( PPoint currentPoint, PPoint& newPoint, int speed)
	{
		/*newPoint.x = currentPoint.x - (int)(speed / (1000/OneKMNumberOfPixels));
		newPoint.y = currentPoint.y +  (int)(speed / (1000/OneKMNumberOfPixels));*/
		//int x=0, y = -(speed / (1000/OneKMNumberOfPixels));
		int x=0, y = -(speed );

		int newX = x *cos( headingAngle[SouthWest].startAngle *PI /180) - y *sin( headingAngle[SouthWest].startAngle *PI /180);

		int newY = x *sin( headingAngle[SouthWest].startAngle *PI /180) + y *cos( headingAngle[SouthWest].startAngle *PI /180);

		newPoint.x = currentPoint.x + newX;

		newPoint.y = currentPoint.y + newY;
	}

	///<summary>
	///current point is current position
	///new point will be the new position after move
	///speed is in m/s
	///</summary>
	static void MoveNothWest( PPoint currentPoint, PPoint& newPoint, int speed)
	{
		/*newPoint.x = currentPoint.x - (int)(speed / (1000/OneKMNumberOfPixels));
		newPoint.y = currentPoint.y - (int)(speed / (1000/OneKMNumberOfPixels));*/
		//int x=0, y = -(speed / (1000/OneKMNumberOfPixels));
		int x=0, y = -(speed );

		int newX = x * cos( headingAngle[NorthWest].startAngle *PI /180) - y * sin( headingAngle[NorthWest].startAngle *PI /180);

		int newY = x * sin( headingAngle[NorthWest].startAngle *PI /180) + y * cos( headingAngle[NorthWest].startAngle *PI /180);

		newPoint.x = currentPoint.x + newX;

		newPoint.y = currentPoint.y + newY;
	}

	static void CalculateNextCordinates( PPoint currentPoint, PPoint& newPoint, int speed, float headingAngle )
	{
		int x=speed, y = 0;

		int newX = x * cos( headingAngle *PI /180) - y * sin( headingAngle *PI /180);

		int newY = x * sin( headingAngle *PI /180) + y * cos( headingAngle *PI /180);

		newPoint.x = currentPoint.x + newX;

		newPoint.y = currentPoint.y + newY;
	}

	///<summary>
	///Number pixels that make one Kilometer.
	///</summary>
};

typedef void (*DirectionfuncPtr)( PPoint currentPoint, PPoint& newPoint, int speed) ;

DirectionfuncPtr directionfuncPtrs[8] = {  NewCordinates::MoveNorth, 
										   NewCordinates::MoveNorthEast,
										   NewCordinates::MoveEast,
										   NewCordinates::MoveSouthEast,
										   NewCordinates::MoveSouth,
										   NewCordinates::MoveSouthWest,
										   NewCordinates::MoveWest,
										   NewCordinates::MoveNothWest};
int viewScale = 40 ;

#endif
