/**
 * Gets the distance between two points in 2D space.
 * @param point1 the first point
 * @param point2 the second point
 */
float dist2D(float pt1[], float pt2[]) {
	return sqrtf(powf(pt1[0] - pt1[1],2) + powf(pt2[0] - pt2[1],2)); 
}

/**
 * Gets the distance between two points in 3D space.
 * @param point1 the first point
 * @param point2 the second point
 */
float dist(float point1[], float point2[]) {
	return sqrtf(
		(point2[0] - point1[0]) * (point2[0] - point1[0]) +
		(point2[1] - point1[1]) * (point2[1] - point1[1]) +
		(point2[2] - point1[2]) * (point2[2] - point1[2])
	);
}

/**
 * Rotates the satellite to look at a specified point in 3D space.
 * @param target the target point
 * @return if we're looking at the target
 */
bool lookAtAsteroid(float target[]) {	
	Vector subVector;
	mathVecSubtract(subVector, target, myState, 3);
	mathVecNormalize(subVector, 3);
	
	// turn to face target attitude
	api.setAttitudeTarget(subVector);
	
	//TODO: Change to take pic
	// see if we're within the angle necessary to hit the asteroid (angle = atan(0.07/dist))
	float angle = atanf(0.07f / dist(myState, target));
	return dist(subVector, myState + 6) < angle;
}

/**
 * Checks if our opponent is going for their memory pack
 * @param memPackNum
 * @return if they're going for it
 */
bool opponentGoingForMemPack(int memPackNum) {

	//if we have the memory pack, we don't really care what opponet is doing regarding it
	if (game.hasMemoryPack(0, memPackNum)) {
		return false; 
	}

	if (game.hasMemoryPack(1, memPackNum)) {
		DEBUG(("Enemy already has that memory pack!"));
		return false; 
	}

	if (memPackNum == opponentMemoryPack) { 
		float lastPos[3]; 
		
		for (int i = 0; i < 3; i++) {
			lastPos[i] = othersState[i]; 
		}
	
		float lastDist = dist2D(lastPos, opponentMemoryPackPos);
		api.getOtherZRState(othersState); 
		float currPos[3];

		for (int i = 0; i < 3; i++) {
			currPos[i] = othersState[i]; 
		}
	
		float currDist = dist2D(currPos, opponentMemoryPackPos);
		return (currDist - lastDist) > 0.01; //might need to tweak that 
	} else {
		api.getOtherZRState(othersState);
		float currVelVector[3];

		for (int i = 3; i < 6; i++) {
			currVelVector[i] = othersState[i]; 
		}
		
		if (currVelVector[0] == 0 || currVelVector[1] == 0) {
			return false;
		}

	float theta = atanf(currVelVector[0]/currVelVector[1]);
		return abs(theta - alpha) < .1; 
	}
	
	return false; 
}


