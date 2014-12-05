

#ifndef DECISIONHELPER_H
#define DECISIONHELPER_H

#define FIRE 0		// Some times denoted by F in code or comments.
#define NOT_FIRE 1  // Some times denoted by N in code or comments.

#define LONG_DISTANCE 0
#define MEDIUM_DISTANCE 1
#define SHORT_DISTANCE 2

#define TOWARDS_EACHOTHER 0
#define I_AM_BEHIND_OPPONENT 1
#define OPPONENT_BEHIND_ME 2
#define PARALLEL_TO_EACHOTHER 3
#define OTHER_POSITION 4
#define OPPOSITE_TO_EACHOTHER 5
/*
Definition of probabilities in defensive mode
*/


//Depending upon distance will any aircraft will change its state to F or N

//									  LD		MD		SD
const 
float D_x_willChangeState[2][3] = {
								    { 0.1,	0.5,	0.4},  //F
									{ 0.5,	0.3,	0.2}   //N
								  };
//Depending upon position will any aircraft will change its state to F or N
//position is denoted by Z as we do not want to confuse it with P (Probability)

//									  TE	 BO		 OB		PE		UP		OE
const  
float Z_x_willChangeState[2][6]	= {
									{ 0.2,	 0.3,	 0.1,	0.2,	0.1,	0.1}, //F
									{ 0.05,   0.05,    0.2,	0.1,	0.4,	0.2}  //N
								  };

/*
Definition of probabilities in Attacking mode
*/
const 
float xAtAnyTimeAtt[2] = {	
							0.1, // Probablity will fire 
							0.9  // Probability will not fire
						 };

//Probability x will remain in same State, x means transition state
//										//Now/Present
//										F     N  
const 
float xWillRemainInStateAtt[2][2] = { 
										{0.1, 0.9 }, //F		Future
										{0.5, 0.5 }  //N		Future
									};
//Depending upon distance will any aircraft will change its state to F or N

//										  LD		MD		SD
const 
float D_x_willChangeStateAtt[2][3] = {
										{ 0.1,		0.5,	0.4},  //F
										{ 0.6,		0.2,	0.2}   //N
									  };
//Depending upon position will any aircraft will change its state to F or N
//position is denoted by Z as we do not want to confuse it with P (Probability)

//										  TE	 BO		 OB		PE	 	UP		OE
const 
float Z_x_willChangeStateAtt[2][6]	= {
										{ 0.3,	 0.3,	 0.1,	0.1,	0.1,	0.1}, //F
										{ 0.05,	0.05,    0.3,   0.3,    0.25,	0.05}  //N
									  };


class DecisionHelper
{
public:
	static float ProbabilityOfNextStateDefensiveMode(	 int nextState /*FIRE or NOT_FIRE*/,
														 int presentState /*FIRE or NOT_FIRE*/,
														 int distance,
														 int position,
														 float distanceRatio,
														 float opponentsBearingAngleRatio)
	{
		// In variable name x means fire or not fire
		float xAtAnyTime[2] ;

		xAtAnyTime[0] = 0.9;
		xAtAnyTime[1] = 0.1;

		//Probability x will remain in same State
		//
		float xWillRemainInState[2][2] ;
		/*= { 
										   {0.1, 0.9 },
										   {0.5, 0.5 }
										 };*/
		xWillRemainInState[0][0] = 0.9;
		xWillRemainInState[0][1] = 0.1;
		xWillRemainInState[1][0] = 0.5;
		xWillRemainInState[1][1] = 0.5;



		float result = 0;
		float transitionProb =  xWillRemainInState[nextState][presentState] ;
		int presentStateComplement ;

		if (presentState == FIRE)
			presentStateComplement = NOT_FIRE;
		else
			presentStateComplement = FIRE;

		/*float numerirator = ( D_x_willChangeState[presentState][distance] *
										  Z_x_willChangeState[presentState][position] *
										  xAtAnyTime[presentState]);

		float denominator = ( D_x_willChangeState[presentState][distance] *
										   Z_x_willChangeState[presentState][position] *
										   xAtAnyTime[presentState]) +
										 ( D_x_willChangeState[presentStateComplement][distance] *
										   Z_x_willChangeState[presentStateComplement][position] *
										   xAtAnyTime[presentStateComplement])
										;

		float currentStateProbability =  numerirator/ denominator;*/

		

		float probWillFire = (1 - distanceRatio ) ;

		float probWillNotFire = distanceRatio ;

		float angleProbWillFire = ( 1 - opponentsBearingAngleRatio ) ;

		float angleProbWillNotFire = opponentsBearingAngleRatio ;

		/*float probWillFire = (1 - distanceRatio) * D_x_willChangeState[FIRE][distance] ;

		float probWillNotFire = (distanceRatio) * D_x_willChangeState[NOT_FIRE][distance] ;

		float angleProbWillFire = (1 - opponentsBearingAngleRatio) * Z_x_willChangeState[FIRE][position] ;

		float angleProbWillNotFire = (opponentsBearingAngleRatio) * Z_x_willChangeState[NOT_FIRE][position] ;*/
		
		float numerirator = ( probWillNotFire *
							  angleProbWillNotFire *
							  xAtAnyTime[presentState]);

		float denominator = ( probWillNotFire *
							  angleProbWillNotFire *
							  xAtAnyTime[presentState]) +
	    					 ( probWillFire *
							   angleProbWillFire *
							   xAtAnyTime[presentStateComplement]);

		float currentStateProbability =  numerirator/ denominator;

		result = transitionProb * currentStateProbability;

		if ( distanceRatio < 0.1 || distanceRatio > 0.85 )
			result = 0;
		

		return result;
	}
	
	static float ProbabilityOfNextStateAttackingMode(	 int nextState /*FIRE or NOT_FIRE*/,
														 int presentState /*FIRE or NOT_FIRE*/,
														 int distance,
														 int position,
														 float distanceRatio,
														 float bearingAngleRatio)
	{
		float result = 0.00001f;
		float realDistance = distanceRatio * 100000;
		float transitionProb =  xWillRemainInStateAtt[nextState][presentState] ;
		int presentStateComplement ;

		if (presentState == FIRE)
			presentStateComplement = NOT_FIRE;
		else
			presentStateComplement = FIRE;

		/*float currentStateProbability = ( D_x_willChangeStateAtt[presentState][distance] *
										  Z_x_willChangeStateAtt[presentState][position] *
										  xAtAnyTimeAtt[presentState]) /
										(
										( D_x_willChangeStateAtt[presentState][distance] *
										   Z_x_willChangeStateAtt[presentState][position] *
										   xAtAnyTimeAtt[presentState]) +
										( D_x_willChangeStateAtt[presentStateComplement][distance] *
										   Z_x_willChangeStateAtt[presentStateComplement][position] *
										   xAtAnyTimeAtt[presentStateComplement])
										);*/

		
		float probWillFire = 1 - distanceRatio;

		float probWillNotFire = distanceRatio;
		
		float angleProbWillFire = 1 - bearingAngleRatio;

		float angleProbWillNotFire = bearingAngleRatio;

		/*float probWillFire = (1 - distanceRatio) * D_x_willChangeStateAtt[FIRE][distance] ;

		float probWillNotFire = (distanceRatio) * D_x_willChangeStateAtt[NOT_FIRE][distance] ;

		float angleProbWillFire = (1 - bearingAngleRatio) * Z_x_willChangeStateAtt[FIRE][position] ;

		float angleProbWillNotFire = (bearingAngleRatio) * Z_x_willChangeStateAtt[NOT_FIRE][position] ;*/

		float currentStateProbability = ( probWillNotFire *
										  angleProbWillNotFire *
										  xAtAnyTimeAtt[presentState]) /
										(
										( probWillFire *
										  angleProbWillFire  *
										   xAtAnyTimeAtt[presentStateComplement]) +
										( probWillNotFire *
										   angleProbWillNotFire  *
										   xAtAnyTimeAtt[presentState])
										);
										
		result = transitionProb * currentStateProbability;

		if ( distanceRatio < 0.1 || distanceRatio > 0.85 )
			result = 0;


		return result;
	}
};

#endif
