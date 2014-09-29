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
void stopAtVariable (float target[3], float power) {
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
bool hasTimeToMoveToShawdowZone() {
    
    int timeToNextSolarFlare = game.getNextFlare(); 
    
    if (timeToNextSolarFlare == -1)
        return false; 
    
    if (game.getMemoryFilled() == 0)
        return false; //should just turn off
    
    //make sure this is up to date
    api.getMyZRState(ourState); 

    float currPos[3] = {ourState[0], ourState[1], ourState[2]};

    /* Shawdow zone positions: 
     * X: 0.0 - 0.64
     * Y: -0.2 - 0.2
     * 
     * Therefore, center at (0.32, 0.0)
     * When going for shawdow, aim there. 
     */ 

     if (currPos[0] >= 0.0f && currPos[1] <= -0.2f) {
        //q3
     } else if (currPos[0] >= 0.0f && currPos[1] >= 0.2f) {
        //q4
     } else {  //easy /Q1 or Q2
        
        float timeToGetToShawdow = 2*ourState[1]/ourState[4]; 

        return timeToGetToShawdow + 3 > timeToNextSolarFlare; //3 second room for error
     }
}

void moveToShadowZone() {

    //update this
    api.getMyZRState(ourState); 

    if (ourState[0] >= 0.0f && ourState[1] <= -0.2f) {
        
        float waypoint[] = {0.0f,-0.4,0.0f}; 

        float waypointArray[1][3];
        waypointArray[0][0] = waypoint[0];
        waypointArray[0][1] = waypoint[1];
        waypointArray[0][2] = waypoint[2];
        
        moveToWaypoints(waypointArray, shadowCenter); 

     } else if (ourState[0] >= 0.0f && ourState[1] >= 0.2f) {

       float waypoint[] = {0.0f,0.4,0.0f}; 

        float waypointArray[1][3];
        waypointArray[0][0] = waypoint[0];
        waypointArray[0][1] = waypoint[1];
        waypointArray[0][2] = waypoint[2];
        
        moveToWaypoints(waypointArray, shadowCenter); 
     } else {  //easy /Q1 or Q2
        moveToFastest(shadowCenter);
     }
}

void moveToWaypoints(float wayPoints[][3], float finalDest[]) {
    //TODO david
}

bool firstTimeSpinningForMemPack; 

//somewhere in init
firstTimeSpinningForMemPack = false; 

/*
 * The satellite’s angular velocity must start at less than 2.3°/s.
 * Rotate the satellite >90° along about the Z axis for 2D. 
 * Do not attempt to rotate faster than 80°/s
 */
void spinForMemoryPack() {
    //SPIN ALONG Z_AXIS

    //update this
    api.getMyZRState(ourState); 

    float currXRotation = ourState[9];
    float currYRotation = ourState[10]; 
    float currZRotation = ourState[11];

    float zTarget; 

    //THIS IS ALL CURRENTLY IN DEGREES, WHICH IS PROBABLY WRONG. 
    if (firstTimeSpinningForMemPack) {

        firstTimeSpinningForMemPack = false; 

        if (abs(currZRotation) >= 2.3) {

            float rVel[3] = {currXRotation, currYRotation, 0};

            api.setAttRateTarget(rVel); 
            return; 
        }
    } else {
        if (abs(currZRotation) >= 80) {
                
                float rVel[3] = {currXRotation, currYRotation, 0};

                api.setAttRateTarget(rVel); 
                return; 
            }
        /* nothing should be wrong at this point, time to start spinning
         * need to spin 90 degrees, restricted at 80 degreees / second
         * meaning this is going to take at least 2 loops, should aim for 40 degrees per second
         */
         if (currZRotation > 0 && abs(currZRotation - 40) > 10)
            return; 

            zTarget = 50 * PI/180; //spin at 50 degrees/second

            float rVel[3] = {currXRotation, currYRotation, zTarget};

            api.setAttRateTarget(rVel);
        }
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