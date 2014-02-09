#include "AllWindows.h"
#include "Window.h"
#include "../Widget.h"
#include "../Graphics.h"
#include "../CityInfoUpdater.h"
#include "Advisors_private.h"
#include "../Data/CityInfo.h"
#include "../Data/Screen.h"
#include "../Data/Mouse.h"

static void drawButtons();
static void buttonGod(int param1, int param2);
static void buttonSize(int param1, int param2);
static void buttonHelp(int param1, int param2);
static void buttonClose(int param1, int param2);
static void buttonHoldFestival(int param1, int param2);

static ImageButton imageButtonsBottom[] = {
	{58, 316, 27, 27, 4, 134, 0, buttonHelp, Widget_Button_doNothing, 1, 0, 0, 0, 0, 0},
	{558, 319, 24, 24, 4, 134, 4, buttonClose, Widget_Button_doNothing, 1, 0, 0, 0, 0, 0},
	{358, 317, 34, 34, 4, 96, 0, buttonHoldFestival, Widget_Button_doNothing, 1, 0, 0, 0, 1, 0},
	{400, 317, 34, 34, 4, 96, 4, buttonClose, Widget_Button_doNothing, 1, 0, 0, 0, 0, 0},
};

static CustomButton buttonsGodsSize[] = {
	{70, 96, 150, 186, buttonGod, Widget_Button_doNothing, 1, 0, 0},
	{170, 96, 250, 186, buttonGod, Widget_Button_doNothing, 1, 1, 0},
	{270, 96, 350, 186, buttonGod, Widget_Button_doNothing, 1, 2, 0},
	{370, 96, 450, 186, buttonGod, Widget_Button_doNothing, 1, 3, 0},
	{470, 96, 550, 186, buttonGod, Widget_Button_doNothing, 1, 4, 0},
	{102, 216, 532, 242, buttonSize, Widget_Button_doNothing, 1, 1, 0},
	{102, 246, 532, 272, buttonSize, Widget_Button_doNothing, 1, 2, 0},
	{102, 276, 532, 302, buttonSize, Widget_Button_doNothing, 1, 3, 0},
};

static int focusButtonId;

void UI_HoldFestivalDialog_drawBackground()
{
	UI_Advisor_drawGeneralBackground();

	int baseOffsetX = Data_Screen.offset640x480.x;
	int baseOffsetY = Data_Screen.offset640x480.y;

	Widget_Panel_drawOuterPanel(baseOffsetX + 48, baseOffsetY + 48, 34, 20);
	Widget_GameText_drawCentered(58, 25 + Data_CityInfo.festivalGod,
		baseOffsetX + 48, baseOffsetY + 60, 544, Font_LargeBlack);
	for (int god = 0; god < 5; god++) {
		if (god == Data_CityInfo.festivalGod) {
			Widget_Panel_drawButtonBorder(
				baseOffsetX + 100 * god + 66,
				baseOffsetY + 92, 90, 100, 1);
			Graphics_drawImage(GraphicId(ID_Graphic_PanelWindows) + god + 21,
				baseOffsetX + 100 * god + 70, baseOffsetY + 96);
		} else {
			Graphics_drawImage(GraphicId(ID_Graphic_PanelWindows) + god + 16,
				baseOffsetX + 100 * god + 70, baseOffsetY + 96);
		}
	}
	drawButtons();
	Widget_GameText_draw(58, 30 + Data_CityInfo.festivalSize,
		baseOffsetX + 180, baseOffsetY + 322, Font_NormalBlack);
}

void UI_HoldFestivalDialog_drawForeground()
{
	drawButtons();
	Widget_Button_drawImageButtons(
		Data_Screen.offset640x480.x, Data_Screen.offset640x480.y,
		imageButtonsBottom, 4);
}

static void drawButtons()
{
	int width;
	int baseOffsetX = Data_Screen.offset640x480.x;
	int baseOffsetY = Data_Screen.offset640x480.y;

	// small festival
	Widget_Panel_drawButtonBorder(
		baseOffsetX + 102, baseOffsetY + 216,
		430, 26, focusButtonId == 6);
	width = Widget_GameText_draw(58, 31,
		baseOffsetX + 110, baseOffsetY + 224, Font_NormalBlack);
	Widget_GameText_drawNumberWithDescription(8, 0, Data_CityInfo.festivalCostSmall,
		baseOffsetX + 110 + width, baseOffsetY + 224, Font_NormalBlack);

	// large festival
	Widget_Panel_drawButtonBorder(
		baseOffsetX + 102, baseOffsetY + 246,
		430, 26, focusButtonId == 7);
	width = Widget_GameText_draw(58, 32,
		baseOffsetX + 110, baseOffsetY + 254, Font_NormalBlack);
	Widget_GameText_drawNumberWithDescription(8, 0, Data_CityInfo.festivalCostLarge,
		baseOffsetX + 110 + width, baseOffsetY + 254, Font_NormalBlack);

	// grand festival
	Widget_Panel_drawButtonBorder(
		baseOffsetX + 102, baseOffsetY + 276,
		430, 26, focusButtonId == 8);
	width = Widget_GameText_draw(58, 33,
		baseOffsetX + 110, baseOffsetY + 284, Font_NormalBlack);
	width += Widget_GameText_drawNumberWithDescription(8, 0, Data_CityInfo.festivalCostGrand,
		baseOffsetX + 110 + width, baseOffsetY + 284, Font_NormalBlack);
	width += Widget_GameText_drawNumberWithDescription(8, 10, Data_CityInfo.festivalWineGrand,
		baseOffsetX + 120 + width, baseOffsetY + 284, Font_NormalBlack);
	Graphics_drawImage(GraphicId(ID_Graphic_ResourceIcons) + Resource_Wine,
		baseOffsetX + 120 + width, baseOffsetY + 279);
	
	// greying out of buttons
	if (Data_CityInfo.treasury <= -5000) {
		Graphics_shadeRect(baseOffsetX + 104, baseOffsetY + 218, 426, 22, 0);
		Graphics_shadeRect(baseOffsetX + 104, baseOffsetY + 248, 426, 22, 0);
		Graphics_shadeRect(baseOffsetX + 104, baseOffsetY + 278, 426, 22, 0);
	} else if (Data_CityInfo.festivalNotEnoughWine) {
		Graphics_shadeRect(baseOffsetX + 104, baseOffsetY + 278, 426, 22, 0);
	}
}

void UI_HoldFestivalDialog_handleMouse()
{
	if (Data_Mouse.right.wentUp) {
		UI_Window_goTo(Window_Advisors);
		return;
	}

	int baseOffsetX = Data_Screen.offset640x480.x;
	int baseOffsetY = Data_Screen.offset640x480.y;

	if (!Widget_Button_handleImageButtons(baseOffsetX, baseOffsetY, imageButtonsBottom, 4)) {
		Widget_Button_handleCustomButtons(baseOffsetX, baseOffsetY, buttonsGodsSize, 8, &focusButtonId);
	}
}

static void buttonGod(int god, int param2)
{
	Data_CityInfo.festivalGod = god;
	UI_Window_requestRefresh();
}

static void buttonSize(int size, int param2)
{
	if (Data_CityInfo.treasury > -5000) {
		if (size != 3 || !Data_CityInfo.festivalNotEnoughWine) {
			Data_CityInfo.festivalSize = size;
			UI_Window_requestRefresh();
		}
	}
}

static void buttonHelp(int param1, int param2)
{
	// TODO showHelpDialog(28, 1);
}

static void buttonClose(int param1, int param2)
{
	UI_Window_goTo(Window_Advisors);
}

static void buttonHoldFestival(int param1, int param2)
{
	if (Data_CityInfo.treasury <= -5000) {
		return;
	}
	Data_CityInfo.plannedFestivalGod = Data_CityInfo.festivalGod;
	Data_CityInfo.plannedFestivalSize = Data_CityInfo.plannedFestivalSize;
	int cost;
	if (Data_CityInfo.festivalSize == 1) {
		Data_CityInfo.plannedFestivalMonthsToGo = 2;
		cost = Data_CityInfo.festivalCostSmall;
	} else if (Data_CityInfo.festivalSize == 2) {
		Data_CityInfo.plannedFestivalMonthsToGo = 3;
		cost = Data_CityInfo.festivalCostLarge;
	} else {
		Data_CityInfo.plannedFestivalMonthsToGo = 4;
		cost = Data_CityInfo.festivalCostGrand;
	}

	Data_CityInfo.treasury -= cost;
	Data_CityInfo.financeSundriesThisYear += cost;

	if (Data_CityInfo.festivalSize == 3) {
		// TODO removeGoodsFromStorage(Resource_Wine, Data_CityInfo.festivalGrandWine);
	}
	UI_Window_goTo(Window_Advisors);
}
