void moveToWaypoint(float target[3], float waypoint[3]) {
    if(areWeThereYet)   {
        api.setPositionTarget(target);
        return;
    }

    if(firstTime) {
        for(int i = 0; i < 3; i++) {
            initPos[i] = currPos[i];
            prevVel[i] = currVel[i];
            initVel[i] = currVel[i];
        }

        mathVecSubtract(travelledVector, currPos, initPos, 3);
        mathVecSubtract(initToWp, waypoint, initPos, 3);
        mathVecSubtract(wpToTar, target, waypoint, 3);

        for(int i = 0; i < 3; i++) {
            forces[i] = initToWp[i];
            dirParTar[i] = wpToTar[i];

        }

        mathVecNormalize(forces, 3);
        mathVecNormalize(dirParTar, 3);

        for(int i = 0; i < 3; i++) {
            currForce[i] = forces[i];
            switchPerpVector[i] = 3 * wpToTar[i] / 4;
        }

        mathVecAdd(switchPerpPos, waypoint, switchPerpVector, 3);

        currSpeedParTar = mathVecInner(currVel, wpToTar, 3)/mathVecMagnitude(wpToTar, 3);
        currSpeedPerpTar = mathVecMagnitude(xProdCurrVelWpToTar, 3)/mathVecMagnitude(wpToTar, 3);
        // mathVecSubtract(currSpeedPerpTar, currVel, currSpeedParTar, 3);


        for(int i = 0; i < 3; i++)
            predictedVelWp[i] = sqrtf(powf(initVel[i], 2) + 2 * MAX_ACCEL_ONE_AXIS * mathVecMagnitude(initToWp, 3));

        mathVecCross(xProdPredVelWpToTar, predictedVelWp, wpToTar);

        predictedSpeedWpPar = mathVecInner(predictedVelWp, wpToTar, 3) / mathVecMagnitude(wpToTar, 3);
        predictedSpeedWpPerp = mathVecMagnitude(xProdPredVelWpToTar, 3) / mathVecMagnitude(wpToTar, 3);

        maxSpeedParTar = sqrtf(2 * MAX_ACCEL_ONE_AXIS * mathVecMagnitude(wpToTar, 3));

        if(predictedSpeedWpPar > maxSpeedParTar)
            maxSpeedPerpTar = maxSpeedParTar;

        else
            maxSpeedPerpTar = predictedSpeedWpPar;

        firstTime = false;

    }

    else {
        mathVecSubtract(currAcc, currVel, prevVel, 3);

        mathVecCross(xProdCurrVelWpToTar, currVel, wpToTar);
        mathVecCross(xProdCurrAccWpToTar, currAcc, wpToTar);

        currSpeedParTar = mathVecInner(currVel, wpToTar, 3)/mathVecMagnitude(wpToTar, 3);
        currSpeedPerpTar = mathVecMagnitude(xProdCurrVelWpToTar, 3) / mathVecMagnitude(wpToTar, 3);

        currAccMagParTar = mathVecInner(currAcc, wpToTar, 3) / mathVecMagnitude(wpToTar, 3);
        currAccMagPerpTar = mathVecMagnitude(xProdCurrAccWpToTar, 3) / mathVecMagnitude(wpToTar, 3);

        for(int i = 0; i < 3; i++) {
            currVelParTar[i] = currSpeedParTar * dirParTar[i];
            currVelPerpTar[i] = currVel[i] - currVelParTar[i];
            currAccParTar[i] = currAccMagParTar * dirParTar[i];
            currAccPerpTar[i] = currAcc[i] - currAccParTar[i];

        }

        if(mathVecMagnitude(initToWp, 3) - mathVecMagnitude(travelledVector, 3) > 0.05f) {
            mathVecSubtract(travelledVector, currPos, initPos, 3);

            for(int i = 0; i < 3; i++)
                forces[i] = initToWp[i];

            mathVecNormalize(forces, 3);

            for(int i = 0; i < 3; i++)
                currForce[i] = forces[i];

            mathVecCross(xProdCurrForceWpToTar, currForce, wpToTar);

            currForceMagParTar = mathVecInner(currForce, wpToTar, 3) / mathVecMagnitude(wpToTar, 3);
            currForceMagPerpTar = mathVecMagnitude(xProdCurrForceWpToTar, 3) / mathVecMagnitude(wpToTar, 3);

            for(int i = 0; i < 3; i++) {
                currForceParTar[i] = currForceMagParTar * dirParTar[i];
                dblCurrForceParTar[i] = 2 * currForceParTar[i];
                dblCurrForcePerpTar[i] = 2 * currForcePerpTar[i];

            }

            mathVecSubtract(currForcePerpTar, forces, currForceParTar, 3);

            if(currSpeedParTar > maxSpeedParTar)
                mathVecSubtract(forces, forces, currForceParTar, 3);

            if(currSpeedPerpTar > maxSpeedPerpTar)
                mathVecSubtract(forces, forces, currForcePerpTar, 3);

        } else {
            if(firstTimeWp) {


                for(int i = 0; i < 3; i++) {
                    switchParVector[i] = currVel[i] / (-2 * MAX_ACCEL_ONE_AXIS  + waypoint[i] + target[i]) / 2;
                    goalAccPerpTar[i] = currVelPerpTar[i] / (-2 * mathVecMagnitude(wpToTar, 3));
                }


                mathVecAdd(switchParPos, switchParVector, waypoint, 3);

            }

            for(int i = 0; i < 3; i++)
                forces[i] = wpToTar[i];

            mathVecNormalize(forces, 3);

            for(int i = 0; i < 3; i++)
                currForce[i] = forces[i];

            mathVecCross(xProdCurrForceWpToTar, currForce, wpToTar);

            currForceMagParTar = mathVecInner(currForce, wpToTar, 3) / mathVecMagnitude(wpToTar, 3);
            currForceMagPerpTar = mathVecMagnitude(xProdCurrForceWpToTar, 3) / mathVecMagnitude(wpToTar, 3);

            if(fabsf(mathVecMagnitude(travelledVector, 3) - mathVecMagnitude(switchParVector, 3)) > 0.05f)
                mathVecSubtract(forces, forces, dblCurrForceParTar, 3);

            if(fabsf(mathVecMagnitude(travelledVector, 3) - mathVecMagnitude(switchPerpVector, 3)) > 0.05f)
                mathVecSubtract(forces, forces, dblCurrForcePerpTar, 3);

            if(fabsf(mathVecMagnitude(travelledVector, 3) - mathVecMagnitude(wpToTar, 3)) > 0.05f) {
                for (int i = 0; i < 3; i++)
                    forces[i] = 0;
                areWeThereYet = true;
                api.setPositionTarget(target);
                return;
            }
        }
    }   

    api.setForces(forces);
    // DEBUG(("Forces: %f %f %f\nmaxSpeedParTar: %f\ncurrSpeedParTar: %f\nmaxSpeedPerpTar: %f\ncurrSpeedPerpTar: %f", forces[0], forces[1], forces[2], maxSpeedParTar, currSpeedParTar, maxSpeedPerpTar, currSpeedPerpTar));
    for(int i = 0; i < 3; i++) {
        currForce[i] = forces[i];
        prevVel[i] = currVel[i];
    }

}