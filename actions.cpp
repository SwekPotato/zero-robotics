/**
 * Move to the first laser position, turn to get laser,
 * move the the second laser position, turn to get laser,
 * return if completed
*/
bool getLaser1() {
	if (otherState[1] > myState[1] && myState[1] > 0 && (myState[0] > 0) == (otherState[0] > 0)) ohCrap = true;
	
	if (moveTo(laserCoords)) { 
		if (!game.haveItem(0, isSPH1 ? 0 : 1)) // 0, 0 for blue
			api.setAttRateTarget(turnSpeed);
	}
	
	//DEBUG(("has laser? %i", game.haveItem(0, isSPH1 ? 0 : 1)));
	
	return api.getTime() > 40 || game.haveItem(0, isSPH1 ? 1 : 0) || ohCrap;

				// their y is greater then ours
				// our y is greater then 0
}

bool getLaser2() {
	laserCoords[0] = isSPH1 ? 0.5f : -0.5f;
	turnSpeed[2] = 0.5f;
	
	if (!moveTo(laserCoords)) {
		api.setAttRateTarget(zeroArray);
	} else {
		api.setAttRateTarget(turnSpeed);
	}
		
	DEBUG(("h2=%i;", game.haveItem(0, isSPH1 ? 0 : 1)));

	return api.getTime() > 85 || game.haveItem(0, isSPH1 ? 0 : 1);
}

/*
 * Moves the satellite to a provided target location, with
 * thrusters firing at full power the enitre trip. Stops the
 * satellite at the target.
*/

void stopAtFastest(float target[3]) {
    if(areWeThereYet) {
        api.setPositionTarget(target);
        return;
    }
        
    if(firstTime) {
        for(int i = 0; i < 3; i++)  {
            initPos[i] = currPos[i];
            halfPos[i] = (target[i] - currPos[i])/2 + currPos[i];
        }
            

        mathVecSubtract(targetVector, target, initPos, 3);
        mathVecSubtract(halfwayVector, halfPos, initPos, 3);
    
        for(int i = 0; i < 3; i++)
            forces[i] = targetVector[i];
        
        mathVecNormalize(forces, 3);
        
        firstTime = false;
        
    } else {
        mathVecSubtract(travelledVector, currPos, initPos, 3);
        if(mathVecMagnitude(travelledVector, 3) >= mathVecMagnitude(halfwayVector, 3)) {
            if(fabsf(mathVecMagnitude(travelledVector, 3) - mathVecMagnitude(targetVector, 3)) < 0.1f) {
                api.setPositionTarget(target);
                firstTime = true;
                areWeThereYet = true;
                
                DEBUG(("%f", (float)api.getTime()));
            } else {
                for(int i = 0; i < 3; i++)
                    forces[i] = targetVector[i] * -1;
                
                mathVecNormalize(forces, 3);
            }
        }
    }
    api.setForces(forces);
    
}

/*
 * Moves the satellite to a provided target location, with
 * thrusters firing only the percentage of the trip specified
 * by the power variable. Stops the satellite at the target.
 * Power should be a decimal between 0 and 1, with 1 being 
 * equivalent to calling stopAtFastest() and 0 resulting in
 * no movement.
*/
void stopAtVariable {
    if(areWeThereYet) {
        api.setPositionTarget(target);
        return;
    }
    
    if(firstTime) {
        for(int i = 0; i < 3; i++) {
            initPos[i] = currPos[i];
            halfPos[i] = .5 * power * (target[i] - currPos[i]) + currPos[i];
        }
            

        mathVecSubtract(targetVector, target, initPos, 3);  
        mathVecSubtract(halfwayVector, halfPos, initPos, 3);
    
        for(int i = 0; i < 3; i++)
            forces[i] = targetVector[i];
        
        mathVecNormalize(forces, 3);
        
        firstTime = false;
        
    } else {
        mathVecSubtract(travelledVector, currPos, initPos, 3);
        if(mathVecMagnitude(travelledVector, 3) >= mathVecMagnitude(halfwayVector, 3)) {
            if(mathVecMagnitude(travelledVector, 3) <= mathVecMagnitude(targetVector, 3) - mathVecMagnitude(halfwayVector, 3)) {
                for(int i = 0; i < 3; i++)
                    forces[i] = 0;
            }
            
            else if(fabsf(mathVecMagnitude(travelledVector, 3) - mathVecMagnitude(targetVector, 3)) < 0.1f) {
                api.setPositionTarget(target);
                firstTime = true;
                areWeThereYet = true;
                
                DEBUG(("%f", (float)api.getTime()));
            }
            
            else {
                for(int i = 0; i < 3; i++)
                    forces[i] = targetVector[i] * -1;
                
                mathVecNormalize(forces, 3);
            }
        }
    }
    api.setForces(forces);
    
}

/*
 * Moves the satellite towards the provided target, with
 * thrusters firing at full power the entire trip. Makes
 * no attempt to stop at the target.
*/

void moveTowardFastest(float target[3]) {
    if(areWeThereYet)
        return;
        
    if(firstTime) {
        for(int i = 0; i < 3; i++)
            initPos[i] = currPos[i];
            
        mathVecSubtract(targetVector, target, initPos, 3); 
    
        for(int i = 0; i < 3; i++)
            forces[i] = targetVector[i];
        
        mathVecNormalize(forces, 3);
        
        firstTime = false;
        
    } else {
        mathVecSubtract(travelledVector, currPos, initPos, 3);
        if(fabsf(mathVecMagnitude(travelledVector, 3) - mathVecMagnitude(targetVector, 3)) < 0.1f) {
            firstTime = true;
            areWeThereYet = true;
            
        }
    }
    api.setForces(forces);
    
}

/*
 * Moves the satellite towards the provided target, with
 * thrusters firing only the percentage of the time 
 * specified by the power variable. Makes no attempt to
 * stop at the target. Power should be a decimal between
 * 0 and 1, with 1 being equivalent to calling
 * moveTowardFastest() and 0 resulting in no movement.
*/

void moveTowardVariable(float target[], float power) {
    if(areWeThereYet)
        return;
    
    if(firstTime) {
        for(int i = 0; i < 3; i++) {
            initPos[i] = currPos[i];
            halfPos[i] = power * (target[i] - currPos[i]) + currPos[i];
        }

        mathVecSubtract(targetVector, target, initPos, 3);  
        mathVecSubtract(halfwayVector, halfPos, initPos, 3);
    
        for(int i = 0; i < 3; i++)
            forces[i] = targetVector[i];
        
        mathVecNormalize(forces, 3);
        
        firstTime = false;
        
    } else {
        mathVecSubtract(travelledVector, currPos, initPos, 3);
        if(mathVecMagnitude(travelledVector, 3) >= mathVecMagnitude(halfwayVector, 3)) {
            for(int i = 0; i < 3; i++)
                forces[i] = 0;
        }
    }
    api.setForces(forces);
    
}

/**
 * If we have the laser:
 *  Return if we've moved to the best lasering position and turned to face the comet
 * If we don't have the laser:
 *  Move to the optimal gravity position and move with the comet
 *  Return false always - no shooting
 */
bool moveToCometPosition() {
	lookAtComet(laserHeading);
		
	return moveTo(optimalShootingPos);
}

bool runWithComet() {
	api.setVelocityTarget(joggingSpeed);
	return game.laserShotsRemaining() <= 0;
}
