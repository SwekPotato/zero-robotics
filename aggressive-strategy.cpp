float target;
float myPos[3], desiredVel[3];
float velocityScalar[3];
bool    firstTime;
bool    areWeThereYet;
float   currPos[3];
float ourState[12];
float othersState[12]; 
float ourMemoryPackPos[3];
float opponentMemoryPackPos[3];
int opponentMemoryPack; 
int ourMemoryPack; 
float alpha; 
float   myVel[3];
float   myState[12];
float   forces[3];              //the forces to be applied to this call of the function
float   initPos[3];             //the position of the sphere in the first call of the fn
float   halfPos[3];             //the position halfway between initpos and the destination
float   travelledVector[3];     //the vector from initpos to the sphere this call of the fn
float   halfwayVector[3];       //the vector from initpos to halfpos
float   targetVector[3];        //the vector from initpos to the destination
float   zeroVelocity[3];

void init(){
	firstTime = true;
	areWeThereYet = false;
	
    for(int i = 0; i < 12; i++) {
        if(i < 3) {
            zeroVelocity[i] = 0.0f;
            halfPos[i] = 0.0f;
            currPos[i] = 0.0f;
        }
        myState[i] = 0.0f;
    }
    	alpha = 0.343834f; //radians, degrees = 19.70029402 degrees
	api.getMyZRState(ourState); 
	api.getOtherZRState(othersState); 

	if (ourState[1] == 0.6f) {
		ourMemoryPack = 1;
		opponentMemoryPack = 0; 
	} else {
		ourMemoryPack = 0;
		opponentMemoryPack = 1; 
	}
	
	ourMemoryPackPos[0] = opponentMemoryPackPos[0] = -0.5f; 
	ourMemoryPackPos[1] = opponentMemoryPackPos[1] = 0.6f; 
	ourMemoryPackPos[2] = opponentMemoryPackPos[2] = 0.0f; 
	
	if (ourMemoryPack == 1) {
		opponentMemoryPackPos[1] *= -1;
	} else {
		ourMemoryPackPos[1] *= -1; 
	}
}

void loop(){
    
	//This function is called once per second.  Use it to control the satellite.

    //Get other Memory pack if other team not trying too
    game.getPOILoc(float pos[3], int id)


    //Take 2  or 3 pics of POI
    game.takePic(float zrState[12], float poiPos[3])

	//Upload pics while moving to get enemy memory
	game.uploadPic()

	//Go grab our own memory pack 

	//Flare detection
	if (game.getNextFlare() < 15) {
		// Movement code to shadow
	}

}
