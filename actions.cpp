/*
 * Moves the satellite to a provided target location, with
 * thrusters firing at full power the enitre trip. Stops the
 * satellite at the target.
*/
void stopAtFastest(float target[]) {
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
void stopAtVariable (float target[], float power) {
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

void moveTowardFastest(float target[]) {
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

void moveToShadowZone() {

    //update this
    api.getMyZRState(ourState); 

    if (ourState[0] >= 0.0f && ourState[1] <= -0.2f) {
        
        float waypoint[3];
        waypoint[0] = 0.0f;
        waypoint[1] = -0.4f;
        waypoint[2] = 0.0f;  

        moveToWaypoint(waypoint, shadowCenter); 

     } else if (ourState[0] >= 0.0f && ourState[1] >= 0.2f) {

       float waypoint[3];
       waypoint[0] = 0.0f;
       waypoint[1] = 0.4f; 
       waypoint[2] = 0.0f; 
        
       moveToWaypoint(waypoint, shadowCenter); 
     } else {  //easy /Q1 or Q2
        stopAtFastest(shadowCenter);
     }
}

void moveToWaypoint(float wayPoint[], float finalDest[]) {
    //TODO david
}

/*
 * The satellite’s angular velocity must start at less than 2.3°/s.
 * Rotate the satellite >90° along about the Z axis for 2D. 
 * Do not attempt to rotate faster than 80°/s
 */
void spinForMemoryPack(float memoryPackPos[]) {

    api.getMyZRState(ourState); 
    float zRVel = ourState[11]; 

    float currPos[3];
    currPos[0] = ourState[0];
    currPos[1] = ourState[1];
    currPos[2] = ourState[2];
  
    float distToMemPack = dist(memoryPackPos, currPos);

        if (zRVel < 1.4f && zRVel > 0.04f && distToMemPack < 0.05f) {
            float rVel[3];
            rVel[0] = 0.0f;
            rVel[1] = 0.0f;
            rVel[2] = 1.0f; //in radians
            api.setAttRateTarget(rVel); 
        }
        else {    
            api.setPositionTarget(memoryPackPos); 
            float rVel[3];
            rVel[0] = 0.0f;
            rVel[1] = 0.0f;
            rVel[2] = 0.0f;
            api.setAttRateTarget(rVel);  
        }
}

void lookAtPOIFromZone(int zoneID) {

    //update this each time this gets called in loop
    api.getMyZRState(ourState); 

    float innerPos, outerPos; 

    innerPos = ourState[1] > 0 ? 0.26f : -0.26f; 
    outerPos = ourState[1] > 0 ? 0.425f : -0.425f; 

    float xTargetPos; 
    xTargetPos = zoneID == INNER_ZONE_ID ? innerPos : outerPos; 

    float lookingPosition[3]; 

    lookingPosition[0] =  xTargetPos; 

    //---no need to change these---//
    lookingPosition[1] = ourState[1]; 
    lookingPosition[2] = ourState[2]; 

    api.setPositionTarget(lookingPosition); 

    float noRotation[3];
    noRotation[0] = 0.0f;
    noRotation[1] = 0.0f; 
    noRotation[2] = 0.0f; 

    api.setAttRateTarget(noRotation); 

}

