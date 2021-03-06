#include "message.h"

#include "core/file.h"
#include "core/lang.h"
#include "core/string.h"
#include "core/time.h"
#include "game/time.h"

#include "formation.h"
#include "city/message.h"
#include "sound.h"
#include "ui/tooltip.h"
#include "ui/messagedialog.h"
#include "ui/window.h"

#define MAX_MESSAGES 1000
#define MAX_QUEUE 20
#define MAX_MESSAGE_CATEGORIES 20

static struct
{
    city_message messages[MAX_MESSAGES];

    int queue[20];
    int consecutive_message_delay;

    int next_message_sequence;
    int total_messages;
    int current_message_id;

    struct
    {
        int pop500;
        int pop1000;
        int pop2000;
        int pop3000;
        int pop5000;
        int pop10000;
        int pop15000;
        int pop20000;
        int pop25000;
    } population_shown;

    int message_count[MAX_MESSAGE_CATEGORIES];
    int message_delay[MAX_MESSAGE_CATEGORIES];

    time_millis last_sound_time[MESSAGE_CAT_RIOT_COLLAPSE+1];

    int problem_count;
    int problem_index;
    time_millis problem_last_click_time;
} data;

static int should_play_sound = 1;

void city_message_init_scenario()
{
    for (int i = 0; i < MAX_MESSAGES; i++)
    {
        data.messages[i].message_type = 0;
    }
    for (int i = 0; i < MAX_QUEUE; i++)
    {
        data.queue[i] = 0;
    }
    data.consecutive_message_delay = 0;

    data.next_message_sequence = 0;
    data.total_messages = 0;
    data.current_message_id = 0;

    for (int i = 0; i < MAX_MESSAGE_CATEGORIES; i++)
    {
        data.message_count[i] = 0;
        data.message_delay[i] = 0;
    }
    // population
    data.population_shown.pop500 = 0;
    data.population_shown.pop1000 = 0;
    data.population_shown.pop2000 = 0;
    data.population_shown.pop3000 = 0;
    data.population_shown.pop5000 = 0;
    data.population_shown.pop10000 = 0;
    data.population_shown.pop15000 = 0;
    data.population_shown.pop20000 = 0;
    data.population_shown.pop25000 = 0;

    for (int i = 0; i <= MESSAGE_CAT_RIOT_COLLAPSE; i++)
    {
        data.last_sound_time[i] = 0;
    }

    city_message_init_problem_areas();
}

void city_message_init_problem_areas()
{
    data.problem_count = 0;
    data.problem_index = 0;
    data.problem_last_click_time = time_get_millis();
}

static int new_message_id()
{
    for (int i = 0; i < MAX_MESSAGES; i++)
    {
        if (!data.messages[i].message_type)
        {
            return i;
        }
    }
    return -1;
}

static int has_video(int textId)
{
    const lang_message *msg = lang_get_message(textId);
    if (!msg->video.text)
    {
        return 0;
    }
    return file_exists(msg->video.text);
}

static void enqueue_message(int sequence)
{
    for (int i = 0; i < MAX_QUEUE; i++)
    {
        if (!data.queue[i])
        {
            data.queue[i] = sequence;
            break;
        }
    }
}

static void play_sound(int text_id)
{
    if (lang_get_message(text_id)->urgent == 1)
    {
        Sound_Effects_playChannel(SoundChannel_FanfareUrgent);
    }
    else
    {
        Sound_Effects_playChannel(SoundChannel_Fanfare);
    }
}

static void show_message_popup(int message_id)
{
    city_message *msg = &data.messages[message_id];
    data.consecutive_message_delay = 5;
    msg->is_read = 1;
    int text_id = city_message_get_text_id(msg->message_type);
    UI_Tooltip_resetTimer();
    if (!has_video(text_id))
    {
        play_sound(text_id);
    }
    UI_MessageDialog_setPlayerMessage(
        msg->year, msg->month, msg->param1, msg->param2,
        city_message_get_advisor(msg->message_type), 1);
    UI_MessageDialog_show(text_id, 0);
}

void city_message_disable_sound_for_next_message()
{
    should_play_sound = 0;
}

void city_message_apply_sound_interval(message_category category)
{
    time_millis now = time_get_millis();
    if (now <= 15000 + data.last_sound_time[category])
    {
        city_message_disable_sound_for_next_message();
    }
    else
    {
        data.last_sound_time[category] = now;
    }
}

void city_message_post(int use_popup, int message_type, int param1, int param2)
{
    int id = new_message_id();
    if (id < 0)
    {
        return;
    }
    data.total_messages++;
    data.current_message_id = id;

    city_message *msg = &data.messages[id];
    msg->message_type = message_type;
    msg->is_read = 0;
    msg->year = game_time_year();
    msg->month = game_time_month();
    msg->param1 = param1;
    msg->param2 = param2;
    msg->sequence = data.next_message_sequence++;

    int text_id = city_message_get_text_id(message_type);
    lang_message_type langMessageType = lang_get_message(text_id)->message_type;
    if (langMessageType == MESSAGE_TYPE_DISASTER || langMessageType == MESSAGE_TYPE_INVASION)
    {
        data.problem_count = 1;
        UI_Window_requestRefresh();
    }
    if (use_popup && UI_Window_getId() == Window_City)
    {
        show_message_popup(id);
    }
    else if (use_popup)
    {
        // add to queue to be processed when player returns to city
        enqueue_message(msg->sequence);
    }
    else if (should_play_sound)
    {
        play_sound(text_id);
    }
    should_play_sound = 1;
}

void city_message_post_with_popup_delay(message_category category, int message_type, int param1, short param2)
{
    int use_popup = 0;
    if (data.message_delay[category] <= 0)
    {
        use_popup = 1;
        data.message_delay[category] = 12;
    }
    city_message_post(use_popup, message_type, param1, param2);
    data.message_count[category]++;
}

void city_message_post_with_message_delay(message_category category, int use_popup, int message_type, int delay)
{
    if (category == MESSAGE_CAT_FISHING_BLOCKED || category == MESSAGE_CAT_NO_WORKING_DOCK)
    {
        // bug in the original game: delays for 'fishing blocked' and 'no working dock'
        // are stored in message_count with manual countdown
        if (data.message_count[category] > 0)
        {
            data.message_count[category]--;
        }
        else
        {
            data.message_count[category] = delay;
            city_message_post(use_popup, message_type, 0, 0);
        }
    }
    else
    {
        if (data.message_delay[category] <= 0)
        {
            data.message_delay[category] = delay;
            city_message_post(use_popup, message_type, 0, 0);
        }
    }
}

void city_message_process_queue()
{
    if (data.consecutive_message_delay > 0)
    {
        data.consecutive_message_delay--;
        return;
    }
    int sequence = 0;
    for (int i = 0; i < MAX_QUEUE; i++)
    {
        if (data.queue[i])
        {
            sequence = data.queue[i];
            data.queue[i] = 0;
            break;
        }
    }
    if (sequence == 0)
    {
        return;
    }
    int message_id = -1;
    for (int i = 0; i < 999; i++)
    {
        if (!data.messages[i].message_type)
        {
            return;
        }
        if (data.messages[i].sequence == sequence)
        {
            message_id = i;
            break;
        }
    }
    if (message_id >= 0)
    {
        show_message_popup(message_id);
    }
}


void city_message_sort_and_compact()
{
    city_message tmp_message;
    for (int i = 0; i < MAX_MESSAGES; i++)
    {
        for (int a = 0; a < MAX_MESSAGES - 1; a++)
        {
            int swap = 0;
            if (data.messages[a].message_type)
            {
                if (data.messages[a].sequence < data.messages[a+1].sequence)
                {
                    if (data.messages[a+1].message_type)
                    {
                        swap = 1;
                    }
                }
            }
            else if (data.messages[a+1].message_type)
            {
                swap = 1;
            }
            if (swap)
            {
                tmp_message = data.messages[a];
                data.messages[a] = data.messages[a+1];
                data.messages[a+1] = tmp_message;
            }
        }
    }
    data.total_messages = 0;
    for (int i = 0; i < MAX_MESSAGES; i++)
    {
        if (data.messages[i].message_type)
        {
            data.total_messages++;
        }
    }
}

int city_message_get_text_id(city_message_type message_type)
{
    if (message_type > 50)
    {
        return message_type + 199;
    }
    else
    {
        return message_type + 99;
    }
}

message_advisor city_message_get_advisor(city_message_type message_type)
{
    switch (message_type)
    {
    case Message_22_LocalUprising:
    case Message_23_BarbarianAttack:
    case Message_24_CaesarArmyAttack:
    case Message_25_DistantBattle:
    case Message_26_EnemiesClosing:
    case Message_27_EnemiesAtTheDoor:
        return MessageAdvisor_Military;

    case Message_28_CaesarRequestsGoods:
    case Message_29_CaesarRequestsMoney:
    case Message_30_CaesarRequestsArmy:
    case Message_31_RequestReminder:
    case Message_32_RequestReceived:
    case Message_33_RequestRefused:
    case Message_34_RequestRefusedOverdue:
    case Message_35_RequestReceivedLate:
        return MessageAdvisor_Imperial;

    case Message_36_Unemployment:
    case Message_37_WorkersNeeded:
        return MessageAdvisor_Labor;

    case Message_49_NotEnoughFood:
    case Message_50_FoodNotDelivered:
        return MessageAdvisor_Population;

    case Message_102_HealthIllness:
    case Message_103_HealthDisease:
    case Message_104_HealthPestilence:
        return MessageAdvisor_Health;

    default:
        return MessageAdvisor_None;
    }
}

void city_message_reset_category_count(message_category category)
{
    data.message_count[category] = 0;
}

void city_message_increase_category_count(message_category category)
{
    data.message_count[category]++;
}

int city_message_get_category_count(message_category category)
{
    return data.message_count[category];
}

void city_message_decrease_delays()
{
    for (int i = 0; i < MAX_MESSAGE_CATEGORIES; i++)
    {
        if (data.message_delay[i] > 0)
        {
            data.message_delay[i]--;
        }
    }
}

int city_message_mark_population_shown(int population)
{
    int *field = 0;
    switch (population)
    {
    case 500:
        field = &data.population_shown.pop500;
        break;
    case 1000:
        field = &data.population_shown.pop1000;
        break;
    case 2000:
        field = &data.population_shown.pop2000;
        break;
    case 3000:
        field = &data.population_shown.pop3000;
        break;
    case 5000:
        field = &data.population_shown.pop5000;
        break;
    case 10000:
        field = &data.population_shown.pop10000;
        break;
    case 15000:
        field = &data.population_shown.pop15000;
        break;
    case 20000:
        field = &data.population_shown.pop20000;
        break;
    case 25000:
        field = &data.population_shown.pop25000;
        break;
    }
    if (field && !*field)
    {
        *field = 1;
        return 1;
    }
    return 0;
}

const city_message *city_message_get(int message_id)
{
    return &data.messages[message_id];
}

int city_message_set_current(int message_id)
{
    return data.current_message_id = message_id;
}

void city_message_mark_read(int message_id)
{
    data.messages[message_id].is_read = 1;
}

void city_message_delete(int message_id)
{
    data.messages[message_id].message_type = 0;
    city_message_sort_and_compact();
}

int city_message_count()
{
    return data.total_messages;
}

int city_message_problem_area_count()
{
    return data.problem_count;
}

int city_message_next_problem_area_grid_offset()
{
    time_millis now = time_get_millis();
    if (now - data.problem_last_click_time > 3000)
    {
        data.problem_index = 0;
    }
    data.problem_last_click_time = now;

    city_message_sort_and_compact();
    data.problem_count = 0;
    for (int i = 0; i < 999; i++)
    {
        city_message *msg = &data.messages[i];
        if (msg->message_type && msg->year >= game_time_year() - 1)
        {
            const lang_message *lang_msg = lang_get_message(city_message_get_text_id(msg->message_type));
            lang_message_type lang_message_type = lang_msg->message_type;
            if (lang_message_type == MESSAGE_TYPE_DISASTER || lang_message_type == MESSAGE_TYPE_INVASION)
            {
                if (lang_message_type != MESSAGE_TYPE_INVASION || Formation_getInvasionGridOffset(msg->param1) > 0)
                {
                    data.problem_count++;
                }
            }
        }
    }
    if (data.problem_count <= 0)
    {
        data.problem_index = 0;
        return 0;
    }
    if (data.problem_index >= data.problem_count)
    {
        data.problem_index = 0;
    }
    int index = 0;
    int current_year = game_time_year();
    for (int i = 0; i < 999; i++)
    {
        city_message *msg = &data.messages[i];
        if (msg->message_type && msg->year >= current_year - 1)
        {
            int text_id = city_message_get_text_id(msg->message_type);
            lang_message_type langMessageType = lang_get_message(text_id)->message_type;
            if (langMessageType == MESSAGE_TYPE_DISASTER || langMessageType == MESSAGE_TYPE_INVASION)
            {
                if (langMessageType != MESSAGE_TYPE_INVASION || Formation_getInvasionGridOffset(msg->param1) > 0)
                {
                    index++;
                    if (data.problem_index < index)
                    {
                        data.problem_index++;
                        int gridOffset = msg->param2;
                        if (langMessageType == MESSAGE_TYPE_INVASION)
                        {
                            gridOffset = Formation_getInvasionGridOffset(msg->param1);
                        }
                        return gridOffset;
                    }
                }
            }
        }
    }
    return 0;
}

void city_message_save_state(buffer *messages, buffer *extra, buffer *counts, buffer *delays, buffer *population)
{
    for (int i = 0; i < MAX_MESSAGES; i++)
    {
        city_message *msg = &data.messages[i];
        buffer_write_i32(messages, msg->param1);
        buffer_write_i16(messages, msg->year);
        buffer_write_i16(messages, msg->param2);
        buffer_write_i16(messages, msg->message_type);
        buffer_write_i16(messages, msg->sequence);
        buffer_write_u8(messages, msg->is_read);
        buffer_write_u8(messages, msg->month);
        buffer_write_i16(messages, 0);
    }

    buffer_write_i32(extra, data.next_message_sequence);
    buffer_write_i32(extra, data.total_messages);
    buffer_write_i32(extra, data.current_message_id);

    for (int i = 0; i < MAX_MESSAGE_CATEGORIES; i++)
    {
        buffer_write_i32(counts, data.message_count[i]);
        buffer_write_i32(delays, data.message_delay[i]);
    }
    // population
    buffer_write_u8(population, 0);
    buffer_write_u8(population, data.population_shown.pop500);
    buffer_write_u8(population, data.population_shown.pop1000);
    buffer_write_u8(population, data.population_shown.pop2000);
    buffer_write_u8(population, data.population_shown.pop3000);
    buffer_write_u8(population, data.population_shown.pop5000);
    buffer_write_u8(population, data.population_shown.pop10000);
    buffer_write_u8(population, data.population_shown.pop15000);
    buffer_write_u8(population, data.population_shown.pop20000);
    buffer_write_u8(population, data.population_shown.pop25000);
}

void city_message_load_state(buffer *messages, buffer *extra, buffer *counts, buffer *delays, buffer *population)
{
    for (int i = 0; i < MAX_MESSAGES; i++)
    {
        city_message *msg = &data.messages[i];
        msg->param1 = buffer_read_i32(messages);
        msg->year = buffer_read_i16(messages);
        msg->param2 = buffer_read_i16(messages);
        msg->message_type = buffer_read_i16(messages);
        msg->sequence = buffer_read_i16(messages);
        msg->is_read = buffer_read_u8(messages);
        msg->month = buffer_read_u8(messages);
        buffer_skip(messages, 2);
    }

    data.next_message_sequence = buffer_read_i32(extra);
    data.total_messages = buffer_read_i32(extra);
    data.current_message_id = buffer_read_i32(extra);

    for (int i = 0; i < MAX_MESSAGE_CATEGORIES; i++)
    {
        data.message_count[i] = buffer_read_i32(counts);
        data.message_delay[i] = buffer_read_i32(delays);
    }
    // population
    buffer_skip(population, 1);
    data.population_shown.pop500 = buffer_read_u8(population);
    data.population_shown.pop1000 = buffer_read_u8(population);
    data.population_shown.pop2000 = buffer_read_u8(population);
    data.population_shown.pop3000 = buffer_read_u8(population);
    data.population_shown.pop5000 = buffer_read_u8(population);
    data.population_shown.pop10000 = buffer_read_u8(population);
    data.population_shown.pop15000 = buffer_read_u8(population);
    data.population_shown.pop20000 = buffer_read_u8(population);
    data.population_shown.pop25000 = buffer_read_u8(population);
}
