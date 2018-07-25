
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sched.h"


/*
 * init channel struct
 */
struct channel {
    char name[200]; // channel name
    int num; // channel number
};


/*
 * create a list database to store channel
 */
ChannelList CHANNEL_STORE_DATABASE = NULL;

/*
 * add getter methods
 */
const char *channelName(Channel c) { return c->name; } // get channel name
int channelNum(Channel c) { return c->num; } // get channel number


/*
 * get channel list
 */
ChannelList allChannels() {
    ChannelList list = NULL;
    list = CHANNEL_STORE_DATABASE;

    ChannelList new_list = NULL;
    while (list != NULL) { // start searching
        Channel temp_channel;
        temp_channel = list->c;
        list = list->next;

        ChannelList new_channel_list;
        new_channel_list = (ChannelList) malloc(sizeof(struct channelNode)); // create a channel list item
        new_channel_list->c = temp_channel; // add channel
        new_channel_list->next = new_list;
        new_list = new_channel_list;
    }
    return new_list;
}


/*
 * Create a new channel
 */
Channel create_channel(int num, const char *name) {

    Channel new_channel; // create channel instance
    new_channel = (Channel) malloc(sizeof(struct channel)); // allocate memory

    strcpy(new_channel->name, name); // store the value of name not refer
    new_channel->num = num;

    return new_channel;
}


/*
 * get channel by channel number
 */
Channel findChannel(int num) {
    ChannelList list = NULL;
    list = CHANNEL_STORE_DATABASE;

    while (list != NULL) { // start searching
        Channel temp_channel;
        temp_channel = list->c;
        list = list->next;
        if (channelNum(temp_channel) == num) return temp_channel; // return matched channel
    }
    return NULL;

}

/* create  new channel and add to database
 */
Channel addChannel(int num, const char *name) {


    if (findChannel(num)) { return NULL; } // return NULL, if another channel with the same number exists

    // Creating a new channel instance with num and name
    Channel new_channel;
    new_channel = create_channel(num, name);

    // add channel to the database
    ChannelList new_channel_list;
    new_channel_list = (ChannelList) malloc(sizeof(struct channelNode)); // create a channel list item
    new_channel_list->c = new_channel; // add channel
    new_channel_list->next = CHANNEL_STORE_DATABASE;
    CHANNEL_STORE_DATABASE = new_channel_list;

    return new_channel;
}


/*
 * init show struct
 */
struct show {
    Channel channel;
    char name[200];
    char day[5];
    struct showTime start_time;
    struct showTime end_time;
};


/*
 * create a list database to store show
 */
ShowList SHOW_STORE_DATABASE = NULL;


/*
 * add getter methods
 */
Channel showChannel(Show s) { return s->channel; } // get show channel
const char *showName(Show s) { return s->name; } // get show name
const char *showDay(Show s) { return s->day; } // get show day
struct showTime showStart(Show s) { return s->start_time; } // get show start time
struct showTime showEnd(Show s) { return s->end_time; } // get show end time
int showChannelNum(Show s) { return channelNum(showChannel(s)); } // get show channel number


/*
 * check in time
 */
int inTime(Show show, struct showTime *time) {
    struct showTime temp_start, temp_end;
    temp_start = showStart(show);
    temp_end = showEnd(show);
    int start_time = temp_start.hour * 60 + temp_start.min;
    int end_time = temp_end.hour * 60 + temp_start.min;
    int main_time = time->hour * 60 + time->min;
    if (start_time > main_time || end_time < main_time) return 1;
    return 0;
}

/*
 * convert hour minute to minute
 */
int convertedTimeToMinute(struct showTime time) {
    return time.hour * 60 + time.min;
}


/*
 * check time conflicts
 */

int isTimeConflicts(Show show, const char *show_day, struct showTime show_start_time, struct showTime show_end_time) {

    if (strcmp(show_day, showDay(show)) != 0) return 0;

    struct showTime show_start, show_end;

    show_start = showStart(show);
    show_end = showEnd(show);

    int show_start_in_munute = convertedTimeToMinute(show_start);
    int show_end_in_munute = convertedTimeToMinute(show_end);
    int show_start_time_in_munute = convertedTimeToMinute(show_start_time);
    int show_end_time_in_munute = convertedTimeToMinute(show_end_time);

    if ((show_start_in_munute > show_start_time_in_munute && show_start_in_munute > show_end_time_in_munute) ||
        (show_end_in_munute < show_start_time_in_munute && show_end_in_munute < show_end_time_in_munute))
        return 0;


    return 1;
}


/*
 * filter show list
 */
ShowList findShows(Channel c, const char *name,
                   const char *day, struct showTime *time) {

    // collect all show list
    ShowList list;
    list = SHOW_STORE_DATABASE;

    ShowList shows = NULL; // will store result data


    while (list != NULL) {

        // collect top show
        Show temp_show;
        temp_show = list->s;


        list = list->next;

        if (c != NULL && showChannelNum(temp_show) != channelNum(c)) continue; // check channel matching
        if (name != NULL && strcmp(showName(temp_show), name) != 0) continue; // check name matching
        if (day != NULL && strcmp(showDay(temp_show), day) != 0) continue; // check day matching
        if (time != NULL && !inTime(temp_show, time)) continue; // check time matching

        ShowList temp_show_list;
        temp_show_list = (ShowList) malloc(sizeof(struct showNode)); // create a show list item
        temp_show_list->s = temp_show; // add show
        temp_show_list->next = shows;
        shows = temp_show_list;
    }

    return shows;
}


Show createShow(Channel c, const char *name, const char *day,
                 struct showTime start, struct showTime end){
    Show new_show;
    new_show = (Show) malloc(sizeof(struct show));
    new_show->channel = c;
    strcpy(new_show->name, name);
    strcpy(new_show->day, day);
    new_show->start_time = start;
    new_show->end_time = end;

    return new_show;
}

Show addShow(Channel c, const char *name, const char *day,
             struct showTime start, struct showTime end) {
    printf("asdfaf");

    if (!findChannel(channelNum(c))) return NULL; // if channel not exists return NULL

    // collect all show
    ShowList list;
    list = SHOW_STORE_DATABASE;

    // start checking
    while (list != NULL) {
        Show show;
        show = list->s;


        list = list->next;


        if (isTimeConflicts(show,day, start, end))
            return NULL; // return null if time conflicts



    }

    // create a new show
    Show new_show;
    new_show = createShow(c,name,day,start,end);

    // add show to the show database
    ShowList new_show_list;
    new_show_list = (ShowList) malloc(sizeof(struct showNode));
    new_show_list->s = new_show; // add new show to database
    new_show_list->next = SHOW_STORE_DATABASE;
    SHOW_STORE_DATABASE = new_show_list;
    return new_show;
}


