float	target, alpha;
float	myPos[3], desiredVel[3], velocityScalar[3];
float	ourMemoryPackPos[3], opponentMemoryPackPos[3];
float   myVel[3], forces[3], initPos[3], halfPos[3];
float   travelledVector[3], halfwayVector[3], targetVector[3];
float   currPos[3];
float	ourState[12], othersState[12]; 
float   zeroVelocity[3];
float	firstPOI[3], secondPOI[3];
int 	opponentMemoryPack, ourMemoryPack; 
bool	firstTime, areWeThereYet;

int 	currentState,
		TURNED_OFF = 0,
		TURNED_ON = 1,
		MOVE_TO_OUR_MEM_PACK = 2,
		MOVE_TO_THEIR_MEM_PACK = 3,
		TAKING_OUR_MEM_PACK = 4,
		TAKING_THEIR_MEM_PACK = 5,
		MOVING_TO_SHADOW = 6,
		MOVING_TO_FIRST_POI = 7,
		MOVING_TO_SECOND_POI = 8,
		TAKING_PIC = 9,
		UPLOAD_PIC = 10,
		OUT_OF_FUEL = 11;

void init(){
	firstTime = true;
	areWeThereYet = false;
	
    for(int i = 0; i < 12; i++) {
        if(i < 3) {
            zeroVelocity[i] = 0.0f;
            halfPos[i] = 0.0f;
            currPos[i] = 0.0f;
        }
        ourState[i] = 0.0f;
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

	if (currentState == TURNED_OFF) {
		game.turnOff();
	}
	
	else if (currentState == TURNED_ON) {
		game.turnOn();
	}

	else if (currentState == MOVE_TO_OUR_MEM_PACK) {
		stopAtFastest([ -0.5, -0.6, 0,0]);
	}

	else if (currentState == MOVE_TO_THEIR_MEM_PACK) {
		stopAtFastest([ -0.5, 0.6, 0,0]);
	}

	else if (currentState == TAKING_THEIR_MEM_PACK) {
		spin();
	}

	else if (currentState == TAKING_OUR_MEM_PACK) {
		spin();
	}

	else if (currentState == MOVING_TO_SHADOW) {
		if (game.getNextFlare() < 15) {
			//TODO: patrick's code
		}
	}

	else if (currentState == MOVING_TO_FIRST_POI) {
		firstPOI = game.getPOILoc(pos[3], 0);
		stopAtFastest(firstPOI[3]);
		currentState = TAKING_PIC;
	}

	else if (currentState == MOVING_TO_SECOND_POI) {
		secondPOI = game.getPOILoc(pos[3], 1);
		stopAtFastest(firstPOI[3]);
		currentState = TAKING_PIC;
	}

	else if (currentState == TAKING_PIC) {

		lookAtPOIFromOuter(); //TODO: write this method

		if (game.alignLine(0)) {
			game.takePic(0);
		} 

		lookAtPOIFromInner(); ////TODO: write this method

	}

	if (currentState == UPLOAD_PIC) {
		if (game.getMemorySize() ==  game.getMemoryFilled()) {
			game.uploadPic(); //SPEHERE now disabled for 3secs
		}
	}

	if (currentState == OUT_OF_FUEL) {
		//TODO: move to shadow
		game.turnOff();
		currentState = TURNED_OFF;
	}
}