#include "AllData.h"

#define DATA_INTERN 1
#include "Grid.h"

struct _Data_Scenario Data_Scenario;

struct _Data_Event Data_Event = {0};

struct _Data_Settings Data_Settings;
struct _Data_Settings_Map Data_Settings_Map;

struct _Data_Tutorial Data_Tutorial;

struct _Data_Message Data_Message;

struct _Data_Empire Data_Empire;
struct _Data_Empire_Sizes Data_Empire_Sizes = {2000, 1000, 16, 16, 120};
struct Data_Empire_Index Data_Empire_Index[40];
struct Data_Empire_Object Data_Empire_Objects[MAX_EMPIRE_OBJECTS];
struct Data_Empire_City Data_Empire_Cities[MAX_EMPIRE_CITIES];

struct _Data_Screen Data_Screen;// = { 800, 600, {80, 60} };

struct _Data_CityInfo Data_CityInfo;
struct _Data_CityInfo_CultureCoverage Data_CityInfo_CultureCoverage;
struct _Data_CityInfo_Extra Data_CityInfo_Extra;
struct _Data_CityInfo_Resource Data_CityInfo_Resource;

struct Data_KeyboardInput Data_KeyboardInput;

struct _Data_State Data_State;

struct Data_Figure Data_Figures[MAX_FIGURES];
struct _Data_Figure_Extra Data_Figure_Extra;

struct Data_Building Data_Buildings[MAX_BUILDINGS];
struct Data_Building_Storage Data_Building_Storages[MAX_STORAGES];
struct _Data_Buildings_Extra Data_Buildings_Extra;
struct _Data_BuildingList Data_BuildingList;

struct Data_Sound_City Data_Sound_City[70];

struct Data_CityView Data_CityView;

struct Data_InvasionWarning Data_InvasionWarnings[MAX_INVASION_WARNINGS];

struct _Data_FileList Data_FileList;

struct _Data_Routes Data_Routes;

struct _Data_Debug Data_Debug;

const int Constant_SalaryForRank[11] = {0, 2, 5, 8, 12, 20, 30, 40, 60, 80, 100};
const int Constant_DirectionGridOffsets[8] = {-162, -161, 1, 163, 162, 161, -1, -163};

const struct MissionId Constant_MissionIds[12] = {
	{0, 0},
	{1, 1},
	{2, 3},
	{4, 5},
	{6, 7},
	{8, 9},
	{10, 11},
	{12, 13},
	{14, 15},
	{16, 17},
	{18, 19},
	{20, 21},
};
