#include "stdafx.h"
#include "AirObjects.h"
#include "TwoDimPlane.h"

//******************************************
//Missile
//******************************************

Missile::Missile(PPoint initialPosition, 
			int speed, 
			HeadingDirection headingDirection,
			Aircraft *owner )
{
	PPoint newPosition;

	m_position = initialPosition;
	m_speed = speed;
	m_headingDirection = headingDirection;
	m_owner = owner;
	m_previousPosition = m_position;

	directionfuncPtrs[ m_headingDirection]( m_position, newPosition, m_speed);
}

Missile::Missile(const Missile& missile)
{
	this->m_elevationAngle = missile.m_elevationAngle;
	this->m_headingDirection= missile.m_headingDirection;
	this->m_height = missile.m_height;
	this->m_id = missile.m_id;
	this->m_position = missile.m_position;
	this->m_speed = missile.m_speed;
	this->m_previousPosition = missile.m_previousPosition;
	this->m_owner = NULL;
}

Missile* Missile::operator=(const Missile& ac)
{
	Missile* newObj = new Missile( ac );
	return newObj;
}

bool Missile::moveRight()
{
	PPoint newPoint;

	if ( m_headingDirection == NorthWest )
	{
		m_headingDirection = North;
	}
	else
	{
		m_headingDirection = (HeadingDirection)(m_headingDirection + 1) ;
	}

	m_previousPosition = m_position;

	directionfuncPtrs[m_headingDirection]( m_position, newPoint, m_speed);

	m_position = newPoint;

	return true;
}

bool Missile::moveLeft()
{
	PPoint newPoint;

	if ( m_headingDirection ==  North)
	{
		m_headingDirection = NorthWest;
	}
	else
	{
		m_headingDirection = (HeadingDirection)(m_headingDirection - 1) ;
	}

	m_previousPosition = m_position;

	directionfuncPtrs[m_headingDirection]( m_position, newPoint, m_speed);

	m_position = newPoint;

	return true;
}

bool Missile::moveStraight()
{
	PPoint newPoint;

	m_previousPosition = m_position;

	directionfuncPtrs[m_headingDirection]( m_position, newPoint, m_speed);

	m_position = newPoint;

	return true;
}


//******************************************
//Aircraft
//******************************************
Aircraft::Aircraft(	PPoint initialPosition, 
					int speed,
					HeadingDirection headingDirection, 
					Team team,
					int height,
					Role role) : m_numberOfMissilesLeft(2), 
								 BVR(100000), 
								 WVR(5000), 
								 pen(NULL), 
								 image(NULL), 
								 m_missile(NULL),
								 m_axisAngle(0),
								 m_hDefenceValue(1),
								 m_hAttackValue(1)
{
	m_position = initialPosition;
	m_speed = speed;
	m_headingDirection = headingDirection;
	m_team = team;
	m_height = height;
	m_elevationAngle = 0;
	m_role = role;
	m_headingAngle = actHeadingAngle[m_headingDirection].startAngle;

}

Aircraft::Aircraft( Aircraft* ac)
{
	this->BVR = ac->BVR;
	this->m_axisAngle = ac->m_axisAngle;
	this->m_elevationAngle = ac->m_elevationAngle;
	this->m_headingDirection= ac->m_headingDirection;
	this->m_height = ac->m_height;
	this->m_id = ac->m_id;
	this->m_numberOfMissilesLeft = ac->m_numberOfMissilesLeft;
	this->m_position = ac->m_position;
	this->m_range = ac->m_range;
	this->m_role = ac->m_role;
	this->m_speed = ac->m_speed;
	this->m_team = ac->m_team;
	this->WVR = ac->WVR;
	this->pen = NULL;
	this->image = NULL;
	this->m_headingAngle = ac->m_headingAngle;

	if ( ac->m_missile == NULL )
	{
		this->m_missile = NULL;
	}
}

Aircraft* Aircraft::Init(	PPoint initialPosition, 
							int speed,
							HeadingDirection headingDirection, 
							Team team,
							int height,
							Role role) 
{
	m_numberOfMissilesLeft=2;
	BVR = 100000; 
	WVR = 5000;
	pen = NULL; 
	image = NULL; 
	m_missile = NULL;
	m_axisAngle = 0;
	m_hDefenceValue = 1;
	m_hAttackValue = 1;

	m_position = initialPosition;
	m_speed = speed;
	m_headingDirection = headingDirection;
	m_team = team;
	m_height = height;
	m_elevationAngle = 0;
	m_role = role;
	m_headingAngle = actHeadingAngle[m_headingDirection].startAngle;
	
	return this;
}

Aircraft* Aircraft::Init( Aircraft* ac, bool initHValue)
{
	this->BVR = ac->BVR;
	this->m_axisAngle = ac->m_axisAngle;
	this->m_elevationAngle = ac->m_elevationAngle;
	this->m_headingDirection= ac->m_headingDirection;
	this->m_height = ac->m_height;
	this->m_id = ac->m_id;
	this->m_numberOfMissilesLeft = ac->m_numberOfMissilesLeft;
	this->m_position = ac->m_position;
	this->m_range = ac->m_range;
	this->m_role = ac->m_role;
	this->m_speed = ac->m_speed;
	this->m_team = ac->m_team;
	this->WVR = ac->WVR;
	this->pen = NULL;
	this->image = NULL;
	this->m_headingAngle = ac->m_headingAngle;
	if ( initHValue)
	{
		this->m_hDefenceValue = 0;
		this->m_hAttackValue  = 0;
	}
	else
	{
		this->m_hDefenceValue = ac->m_hDefenceValue;
		this->m_hAttackValue = ac->m_hAttackValue;
	}

	if ( ac->m_missile == NULL )
	{
		this->m_missile = NULL;
	}

	return this;
}

Aircraft::~Aircraft(void)
{
	if ( image != NULL)
		delete image;

	if ( pen != NULL)
		delete pen;

	m_relations.clear();

	image = NULL;
}

void Aircraft::DestroyResources()
{
	if ( image != NULL)
		delete image;

	if ( pen != NULL)
		delete pen;

	m_relations.clear();
	m_relations.resize(0);

	image = NULL;
}

/*Aircraft& Aircraft::operator=(const Aircraft& ac)
{
	Aircraft* newObj = new Aircraft( ac);

	return *newObj;
}*/

bool Aircraft::fireOrShoot(HeadingDirection direction, int speed, PPoint target)
{
	if ( m_numberOfMissilesLeft <= 0 )
	{
		return false;
	}

	if (m_missile != NULL)
	{
		return false;
	}

	m_numberOfMissilesLeft--;

	PPoint missileInitialPosition;

	directionfuncPtrs[direction]( m_position, missileInitialPosition, speed);

	Aircraft *owner = this;

	Missile* tmpMissile = new Missile( missileInitialPosition, 
										speed, 
										direction,
										owner);

	m_missile = tmpMissile;
	
	m_myPlane->AddObjectOnPlane( tmpMissile, FlyingObjectType::TypeMissile);

	return true;
}

bool Aircraft::setSpeed(int speed)
{
	if ( speed > MAXSPEEDAIRCRAFT) 
	{
		return false;
	}
	else if ( speed < MINSPEEDAIRCRAFT)
	{
		return false;
	}
	m_speed = speed;

	return true;
}

//To be deleted
/*void Aircraft::calcRange()
{
	//Calculate all three point for range current position is known
	//leftBVR and right BVR is unknown

	int gx2 = 0, gy2 = BVR;

	//calculate leftBVR 
	leftBVR.x =  gx2 * cos( headingAngleRange[m_headingDirection].startAngle *PI /180) - gy2 *sin( headingAngleRange[m_headingDirection].startAngle *PI /180) + m_position.x;

	leftBVR.y =  gx2 * sin( headingAngleRange[m_headingDirection].startAngle *PI /180) + gy2 *cos( headingAngleRange[m_headingDirection].startAngle *PI /180) + m_position.y;

	//calculate rightBVR
	rightBVR.x = gx2 * cos( headingAngleRange[m_headingDirection].endAngle *PI /180) - gy2 *sin( headingAngleRange[m_headingDirection].endAngle *PI /180) + m_position.x;

	rightBVR.y = gx2 * sin( headingAngleRange[m_headingDirection].endAngle *PI /180) + gy2 *cos( headingAngleRange[m_headingDirection].endAngle *PI /180) + m_position.y;
}*/



bool Aircraft::isInBVR( PPoint targetPoint)
{
	bool isInRange = PointInRange(  m_position, targetPoint, BVR );
	
	return isInRange;
}

bool Aircraft::isInWVR( PPoint targetPoint)
{
	bool isInRange = PointInRange( m_position, targetPoint, WVR );

	return isInRange;
}

bool Aircraft::PointInRange(PPoint currentPosition, PPoint pointTobeTested, int radius)
{
	int D =(int) sqrt(pow((double)((currentPosition.x ) 
					- (pointTobeTested.x  ) ), 2) + 
					  pow( (double)( (currentPosition.y ) 
					- (pointTobeTested.y ) ), 2));
	return (D <= radius );
}

void Aircraft::drawAirplane( Graphics *graphics, bool drawBVR, bool drawWVR )
{
	Gdiplus::Matrix matrixIdentity;
	Gdiplus::Matrix matrixRotation;
	int x, y;

	if (this->m_team == Blue && image == NULL)
	{
		image = Gdiplus::Image::FromFile(L"C:\\Images\\blue.bmp");
		pen = new Pen(Color(255, 0, 0, 255));
	}
	else if ( image == NULL)
	{
		image = Gdiplus::Image::FromFile(L"C:\\Images\\red.bmp");
		pen = new Pen(Color(255, 255, 0, 0));
	}

	float headingAngleForPresentation = m_headingAngle;

	headingAngleForPresentation = headingAngleForPresentation + 90;

	if ( headingAngleForPresentation < 0 )
	{
		headingAngleForPresentation = headingAngleForPresentation + 360;
	}
	else if ( headingAngleForPresentation > 360)
	{
		headingAngleForPresentation = headingAngleForPresentation - 360;
	}

	matrixRotation.RotateAt( (float) headingAngleForPresentation, 
							 Gdiplus::PointF( (Gdiplus::REAL) m_position.x / viewScale, 
							(Gdiplus::REAL) m_position.y / viewScale)
							);
	
	graphics->SetTransform( &matrixRotation);
	graphics->DrawImage( image, (INT)(m_position.x / viewScale), 
								(INT)(m_position.y / viewScale));
	graphics->SetTransform( &matrixIdentity);

	if (drawBVR)
	{
		for (float angle = 0 ; angle <= 360 ; angle += 2 )
		{
			x = (int)(0 *cos( angle *PI /180) 
						- (-BVR) * sin( angle *PI /180) );
			y = (int)(0 *sin( angle *PI /180)
						+ (-BVR) * cos( angle *PI /180) );
		
			graphics->DrawLine(pen, (x + m_position.x) / viewScale, 
									(y + m_position.y) / viewScale + 2, 
									(x + m_position.x) / viewScale, 
									(y + m_position.y) / viewScale);
		}
	}

	if (drawWVR)
	{
		for (float angle = 0 ; angle <= 360 ; angle += 10 )
		{
			x = (int)(0 *cos( angle *PI /180) 
					- (-WVR) * sin( angle *PI /180) );
			
			y = (int)(0 *sin( angle *PI /180)
				+ (-WVR) * cos( angle *PI /180) );
		
			graphics->DrawLine(pen, (x + m_position.x) / viewScale, 
									(y + m_position.y) / viewScale + 1, 
									(x + m_position.x) / viewScale, 
									(y + m_position.y) / viewScale);
		}
	}
	
	if (image != NULL)
	{
		delete image;
		image = NULL;
	}

	if (pen != NULL)
	{
		delete pen;
		pen = NULL;
	}
}

bool Aircraft::moveRight()
{
	for (int i = 0 ; i < NUMBEROFSECONDS ; i++)
	{
		PPoint newPoint;

		m_headingAngle = m_headingAngle + 2;

		if ( m_headingAngle > 360)
		{
			m_headingAngle = m_headingAngle - 360;
		}

		NewCordinates::CalculateNextCordinates( m_position, newPoint, m_speed, m_headingAngle );

		m_position = newPoint;
	}

	return true;
}

bool Aircraft::moveLeft()
{

	for (int i = 0 ; i < NUMBEROFSECONDS ; i++)
	{
		PPoint newPoint;

		m_headingAngle = m_headingAngle - 2;

		if ( m_headingAngle < 0)
		{
			m_headingAngle = 360 + m_headingAngle;
		}

		NewCordinates::CalculateNextCordinates( m_position, newPoint, m_speed, m_headingAngle );

		m_position = newPoint;
	}

	return true;
}

bool Aircraft::moveStraight()
{
	for (int i = 0 ; i < NUMBEROFSECONDS ; i++)
	{
		PPoint newPoint;

		NewCordinates::CalculateNextCordinates( m_position, newPoint, m_speed, m_headingAngle );

		m_position = newPoint;
	}
	return true;
}
