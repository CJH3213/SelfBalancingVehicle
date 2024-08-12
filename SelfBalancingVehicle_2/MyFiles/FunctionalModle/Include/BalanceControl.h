#ifndef __BALANCECONTROL_H
#define __BALANCECONTROL_H

#include "main.h"


extern float gPitch, gRoll, gYaw;

extern float gBalanceUprightKP;
extern float gBalanceUprightKD;
extern float gBalanceUprightMC;
extern float gBalanceVelocityKP;
extern float gBalanceVelocityKI;
extern float gBalanceSteeringKD;

void OnMainForBalanceControl(void);
void OnTIMForBalanceControl(void);

#endif
