#ifndef SECURITY_H
#define SECURITY_H

void Security_Tick_updateFireSpreadDirection();
void Security_Tick_updateBurningRuins();
int Security_Fire_getClosestBurningRuin(int x, int y, int *distance);

void Security_Tick_generateCriminal();
void Security_Tick_checkFireCollapse();

#endif
