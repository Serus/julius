#ifndef CITYVIEW_H
#define CITYVIEW_H

void CityView_setViewport();
void CityView_setViewportWithSidebar();
void CityView_setViewportWithoutSidebar();

void CityView_calculateLookup();

void CityView_goToGridOffset(int gridOffset);

void CityView_checkCameraBoundaries();

void CityView_gridOffsetToXYCoords(int gridOffset, int *xInTiles, int *yInTiles);

int CityView_pixelCoordsToGridOffset(int xPixels, int yPixels);

void CityView_rotateLeft();

void CityView_rotateRight();

#endif
