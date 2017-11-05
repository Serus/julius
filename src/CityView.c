#include "CityView.h"

#include "ui/citybuildings.h"

#include "data/cityview.hpp"
#include "data/constants.hpp"
#include "data/grid.hpp"
#include "data/screen.hpp"
#include "data/settings.hpp"
#include "data/state.hpp"

static void setViewport(int xOffset, int yOffset, int widthInTiles, int heightInTiles);

void CityView_setViewport()
{
	if (Data_State.sidebarCollapsed) {
		CityView_setViewportWithoutSidebar();
	} else {
		CityView_setViewportWithSidebar();
	}
}

void CityView_setViewportWithoutSidebar()
{
	setViewport(0, 24,
		(Data_Screen.width - 40) / 60,
		(Data_Screen.height - 24) / 15);
}

void CityView_setViewportWithSidebar()
{
	setViewport(0, 24,
		(Data_Screen.width - 160) / 60,
		(Data_Screen.height - 24) / 15);
}

static void setViewport(int xOffset, int yOffset, int widthInTiles, int heightInTiles)
{
	Data_CityView.xOffsetInPixels = xOffset;
	Data_CityView.yOffsetInPixels = yOffset;
	Data_CityView.widthInPixels = widthInTiles * 60 - 2;
	Data_CityView.heightInPixels = heightInTiles * 15;
	Data_CityView.widthInTiles = widthInTiles;
	Data_CityView.heightInTiles = heightInTiles;
	Data_CityView.xInTiles = GRID_SIZE / 2;
	Data_CityView.yInTiles = GRID_SIZE;
}

static void resetLookup()
{
	for (int y = 0; y < VIEW_Y_MAX; y++) {
		for (int x = 0; x < VIEW_X_MAX; x++) {
			ViewToGridOffset(x,y) = -1;
		}
	}
}

void CityView_calculateLookup()
{
	resetLookup();
	int yViewStart;
	int yViewSkip;
	int yViewStep;
	int xViewStart;
	int xViewSkip;
	int xViewStep;
	switch (Data_Settings_Map.orientation) {
		default:
		case Dir_0_Top:
			xViewStart = VIEW_X_MAX - 1;
			xViewSkip = -1;
			xViewStep = 1;
			yViewStart = 1;
			yViewSkip = 1;
			yViewStep = 1;
			break;
		case Dir_2_Right:
			xViewStart = 1;
			xViewSkip = 1;
			xViewStep = 1;
			yViewStart = VIEW_X_MAX - 1;
			yViewSkip = 1;
			yViewStep = -1;
			break;
		case Dir_4_Bottom:
			xViewStart = VIEW_X_MAX - 1;
			xViewSkip = 1;
			xViewStep = -1;
			yViewStart = VIEW_Y_MAX - 2;
			yViewSkip = -1;
			yViewStep = -1;
			break;
		case Dir_6_Left:
			xViewStart = VIEW_Y_MAX - 2;
			xViewSkip = -1;
			xViewStep = -1;
			yViewStart = VIEW_X_MAX - 1;
			yViewSkip = -1;
			yViewStep = 1;
			break;
	}

	for (int y = 0; y < GRID_SIZE; y++) {
		int xView = xViewStart;
		int yView = yViewStart;
		for (int x = 0; x < GRID_SIZE; x++) {
			int gridOffset = x + GRID_SIZE * y;
			if (Data_Grid_graphicIds[gridOffset] < 6) {
				ViewToGridOffset(xView/2, yView) = -1;
			} else {
				ViewToGridOffset(xView/2, yView) = gridOffset;
			}
			xView += xViewStep;
			yView += yViewStep;
		}
		xViewStart += xViewSkip;
		yViewStart += yViewSkip;
	}
}

void CityView_goToGridOffset(int gridOffset)
{
	int x, y;
	CityView_gridOffsetToXYCoords(gridOffset, &x, &y);
	Data_Settings_Map.camera.x = x - Data_CityView.widthInTiles / 2;
	Data_Settings_Map.camera.y = y - Data_CityView.heightInTiles / 2;
	Data_Settings_Map.camera.y &= ~1;
	CityView_checkCameraBoundaries();
}

void CityView_checkCameraBoundaries()
{
	int xMin = (165 - Data_Settings_Map.width) / 2;
	int yMin = (323 - 2 * Data_Settings_Map.height) / 2;
	if (Data_Settings_Map.camera.x < xMin - 1) {
		Data_Settings_Map.camera.x = xMin - 1;
	}
	if (Data_Settings_Map.camera.x > 165 - xMin - Data_CityView.widthInTiles) {
		Data_Settings_Map.camera.x = 165 - xMin - Data_CityView.widthInTiles;
	}
	if (Data_Settings_Map.camera.y < yMin) {
		Data_Settings_Map.camera.y = yMin;
	}
	if (Data_Settings_Map.camera.y > 327 - yMin - Data_CityView.heightInTiles) {
		Data_Settings_Map.camera.y = 327 - yMin - Data_CityView.heightInTiles;
	}
	Data_Settings_Map.camera.y &= ~1;
}

void CityView_gridOffsetToXYCoords(int gridOffset, int *xInTiles, int *yInTiles)
{
	*xInTiles = *yInTiles = 0;
	for (int y = 0; y < VIEW_Y_MAX; y++) {
		for (int x = 0; x < VIEW_X_MAX; x++) {
			if (ViewToGridOffset(x,y) == gridOffset) {
				*xInTiles = x;
				*yInTiles = y;
				return;
			}
		}
	}
}

int CityView_pixelCoordsToGridOffset(int xPixels, int yPixels)
{
	if (xPixels < Data_CityView.xOffsetInPixels ||
		xPixels >= Data_CityView.xOffsetInPixels + Data_CityView.widthInPixels ||
		yPixels < Data_CityView.yOffsetInPixels ||
		yPixels >= Data_CityView.yOffsetInPixels + Data_CityView.heightInPixels) {
		return 0;
	}

	int odd = ((xPixels - Data_CityView.xOffsetInPixels) / 30 + (yPixels - Data_CityView.yOffsetInPixels) / 15) & 1;
	int xOdd = ((xPixels - Data_CityView.xOffsetInPixels) / 30) & 1;
	int yOdd = ((yPixels - Data_CityView.yOffsetInPixels) / 15) & 1;
	int xMod = ((xPixels - Data_CityView.xOffsetInPixels) % 30) / 2;
	int yMod = (yPixels - Data_CityView.yOffsetInPixels) % 15;
	int xViewOffset = (xPixels - Data_CityView.xOffsetInPixels) / 60;
	int yViewOffset = (yPixels - Data_CityView.yOffsetInPixels) / 15;
	if (odd) {
		if (xMod + yMod >= 15 - 1) {
			yViewOffset++;
			if (xOdd && !yOdd) {
				xViewOffset++;
			}
		}
	} else {
		if (yMod > xMod) {
			yViewOffset++;
		} else if (xOdd && yOdd) {
			xViewOffset++;
		}
	}
	Data_CityView.selectedTile.xOffsetInPixels =
		Data_CityView.xOffsetInPixels + 60 * xViewOffset;
	if (yViewOffset & 1) {
		Data_CityView.selectedTile.xOffsetInPixels -= 30;
	}
	Data_CityView.selectedTile.yOffsetInPixels =
		Data_CityView.yOffsetInPixels + 15 * yViewOffset - 15; // TODO why -1?
	int gridOffset = ViewToGridOffset(Data_CityView.xInTiles + xViewOffset, Data_CityView.yInTiles + yViewOffset);
	return gridOffset < 0 ? 0 : gridOffset;
}

void CityView_rotateLeft()
{
	int centerGridOffset = ViewToGridOffset(
		Data_Settings_Map.camera.x + Data_CityView.widthInTiles / 2,
		Data_Settings_Map.camera.y + Data_CityView.heightInTiles / 2);

	Data_Settings_Map.orientation += 2;
	if (Data_Settings_Map.orientation > 6) {
		Data_Settings_Map.orientation = Dir_0_Top;
	}
	CityView_calculateLookup();
	if (centerGridOffset >= 0) {
		int x, y;
		CityView_gridOffsetToXYCoords(centerGridOffset, &x, &y);
		Data_Settings_Map.camera.x = x - Data_CityView.widthInTiles / 2;
		Data_Settings_Map.camera.y = y - Data_CityView.heightInTiles / 2;
		if (Data_Settings_Map.orientation == Dir_0_Top ||
			Data_Settings_Map.orientation == Dir_4_Bottom) {
			Data_Settings_Map.camera.x++;
		}
	}
}

void CityView_rotateRight()
{
	int centerGridOffset = ViewToGridOffset(
		Data_Settings_Map.camera.x + Data_CityView.widthInTiles / 2,
		Data_Settings_Map.camera.y + Data_CityView.heightInTiles / 2);
	
	Data_Settings_Map.orientation -= 2;
	if (Data_Settings_Map.orientation < 0) {
		Data_Settings_Map.orientation = Dir_6_Left;
	}
	CityView_calculateLookup();
	if (centerGridOffset >= 0) {
		int x, y;
		CityView_gridOffsetToXYCoords(centerGridOffset, &x, &y);
		Data_Settings_Map.camera.x = x - Data_CityView.widthInTiles / 2;
		Data_Settings_Map.camera.y = y - Data_CityView.heightInTiles / 2;
		if (Data_Settings_Map.orientation == Dir_0_Top ||
			Data_Settings_Map.orientation == Dir_4_Bottom) {
			Data_Settings_Map.camera.y += 2;
		}
	}
}
