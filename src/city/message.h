#ifndef CITY_MESSAGE_H
#define CITY_MESSAGE_H

#include "core/buffer.h"


typedef enum {
    MESSAGE_CAT_RIOT = 0,
    MESSAGE_CAT_FIRE = 1,
    MESSAGE_CAT_COLLAPSE = 2,
    MESSAGE_CAT_RIOT_COLLAPSE = 3,
    MESSAGE_CAT_BLOCKED_DOCK = 4,
    MESSAGE_CAT_WORKERS_NEEDED = 8,
    MESSAGE_CAT_TUTORIAL3 = 9,
    MESSAGE_CAT_NO_WORKING_DOCK = 10,
    MESSAGE_CAT_FISHING_BLOCKED = 11,
} message_category;

typedef enum {
    MessageAdvisor_None = 0,
    MessageAdvisor_Labor = 1,
    MessageAdvisor_Trade = 2,
    MessageAdvisor_Population = 3,
    MessageAdvisor_Imperial = 4,
    MessageAdvisor_Military = 5,
    MessageAdvisor_Health = 6,
    MessageAdvisor_Religion = 7,
} message_advisor;

typedef enum {
    Message_2_Population500 = 2,
    Message_3_Population1000 = 3,
    Message_4_Population2000 = 4,
    Message_5_Population3000 = 5,
    Message_6_Population5000 = 6,
    Message_7_Population10000 = 7,
    Message_8_Population15000 = 8,
    Message_9_Population20000 = 9,
    Message_10_Population25000 = 10,
    Message_11_RiotInTheCity = 11,
    Message_12_FireInTheCity = 12,
    Message_13_CollapsedBuilding = 13,
    Message_14_DestroyedBuilding = 14,
    Message_15_NavigationImpossible = 15,
    Message_16_CityInDebt = 16,
    Message_17_CityInDebtAgain = 17,
    Message_18_CityStillInDebt = 18,
    Message_19_CaesarWrath = 19,
    Message_20_CaesarArmyContinue = 20,
    Message_21_CaesarArmyRetreat = 21,
    Message_22_LocalUprising = 22,
    Message_23_BarbarianAttack = 23,
    Message_24_CaesarArmyAttack = 24,
    Message_25_DistantBattle = 25,
    Message_26_EnemiesClosing = 26,
    Message_27_EnemiesAtTheDoor = 27,
    Message_28_CaesarRequestsGoods = 28,
    Message_29_CaesarRequestsMoney = 29,
    Message_30_CaesarRequestsArmy = 30,
    Message_31_RequestReminder = 31,
    Message_32_RequestReceived = 32,
    Message_33_RequestRefused = 33,
    Message_34_RequestRefusedOverdue = 34,
    Message_35_RequestReceivedLate = 35,
    Message_36_Unemployment = 36,
    Message_37_WorkersNeeded = 37,
    Message_38_SmallFestival = 38,
    Message_39_LargeFestival = 39,
    Message_40_GrandFestival = 40,
    Message_41_WrathOfCeres = 41,
    Message_42_WrathOfNeptuneNoSeaTrade = 42,
    Message_43_WrathOfMercury = 43,
    Message_44_WrathOfMarsNoMilitary = 44,
    Message_45_WrathOfVenus = 45,
    Message_46_PeopleDisgruntled = 46,
    Message_47_PeopleUnhappy = 47,
    Message_48_PeopleAngry = 48,
    Message_49_NotEnoughFood = 49,
    Message_50_FoodNotDelivered = 50,
    Message_52_Theft = 52,
    Message_53_TutorialFirstFire = 53,
    Message_54_TutorialFirstCollapse = 54,
    Message_55_GodsUnhappy = 55,
    Message_56_TutorialProvidingWater = 56,
    Message_57_TutorialGrowingYourCity = 57,
    Message_58_TutorialHungerHaltsImmigrants = 58,
    Message_59_TutorialReligion = 59,
    Message_60_TutorialTaxesIndustry = 60,
    Message_61_TutorialTrade = 61,
    Message_62_Earthquake = 62,
    Message_63_GladiatorRevolt = 63,
    Message_64_EmperorChange = 64,
    Message_65_LandTradeDisruptedSandstorms = 65,
    Message_66_SeaTradeDisrupted = 66,
    Message_67_LandTradeDisruptedLandslides = 67,
    Message_68_RomeRaisesWages = 68,
    Message_69_RomeLowersWages = 69,
    Message_70_ContaminatedWater = 70,
    Message_71_IronMineCollaped = 71,
    Message_72_ClayPitFlooded = 72,
    Message_73_GladiatorRevoltFinished = 73,
    Message_74_IncreasedTrading = 74,
    Message_75_DecreasedTrading = 75,
    Message_76_TradeStopped = 76,
    Message_78_PriceIncreased = 78,
    Message_79_PriceDecreased = 79,
    Message_77_EmpireHasExpanded = 77,
    Message_80_RoadToRomeBlocked = 80,
    Message_81_WrathOfNeptune = 81,
    Message_82_WrathOfMars = 82,
    Message_84_DistantBattleLostNoTroops = 84,
    Message_85_DistantBattleLostTooLate = 85,
    Message_86_DistantBattleLostTooWeak = 86,
    Message_87_DistantBattleWon = 87,
    Message_88_TroopsReturnFailed = 88,
    Message_89_TroopsReturnVictorious = 89,
    Message_90_DistantBattleCityRetaken = 90,
    Message_91_CeresIsUpset = 91,
    Message_92_NeptuneIsUpset = 92,
    Message_93_MercuryIsUpset = 93,
    Message_94_MarsIsUpset = 94,
    Message_95_VenusIsUpset = 95,
    Message_96_BlessingFromCeres = 96,
    Message_97_BlessingFromNeptune = 97,
    Message_98_BlessingFromMercury = 98,
    Message_99_BlessingFromMars = 99,
    Message_100_BlessingFromVenus = 100,
    Message_101_GodsWrathful = 101,
    Message_102_HealthIllness = 102,
    Message_103_HealthDisease = 103,
    Message_104_HealthPestilence = 104,
    Message_105_SpiritOfMars = 105,
    Message_106_CaesarRespect1 = 106,
    Message_107_CaesarRespect2 = 107,
    Message_108_CaesarRespect3 = 108,
    Message_109_WorkingHippodrome = 109,
    Message_110_WorkingColosseum = 110,
    Message_111_Emigration = 111,
    Message_112_Fired = 112,
    Message_114_EnemyArmyAttack = 114,
    Message_115_RequestCanComply = 115,
    Message_116_RoadToRomeObstructed = 116,
    Message_117_NoWorkingDock = 117,
    Message_118_FishingBoatBlocked = 118,
    Message_119_TutorialHealth = 119,
    Message_121_LocalUprisingMars = 121,
} city_message_type;

typedef struct {
    int sequence;
    int message_type;
    int year;
    int month;
    int param1;
    int param2;
    int is_read;
} city_message;

void city_message_init_scenario();

void city_message_init_problem_areas();

void city_message_disable_sound_for_next_message();

void city_message_apply_sound_interval(message_category category);

void city_message_post(int use_popup, int message_type, int param1, int param2);

void city_message_post_with_popup_delay(message_category category, int message_type, int param1, short param2);

void city_message_post_with_message_delay(message_category category, int use_popup, int message_type, int delay);

void city_message_process_queue();

void city_message_sort_and_compact();

int city_message_get_text_id(city_message_type message_type);

message_advisor city_message_get_advisor(city_message_type message_type);

void city_message_reset_category_count(message_category category);

void city_message_increase_category_count(message_category category);

int city_message_get_category_count(message_category category);

void city_message_decrease_delays();

int city_message_mark_population_shown(int population);

const city_message *city_message_get(int message_id);

int city_message_set_current(int message_id);

void city_message_mark_read(int message_id);

void city_message_delete(int message_id);

int city_message_count();

int city_message_problem_area_count();

int city_message_next_problem_area_grid_offset();

void city_message_save_state(buffer *messages, buffer *extra, buffer *counts, buffer *delays, buffer *population);

void city_message_load_state(buffer *messages, buffer *extra, buffer *counts, buffer *delays, buffer *population);

#endif // CITY_MESSAGE_H
