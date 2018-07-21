//
// Created by sharif on 7/18/18.
//
#include <stdio.h> // printf
#include <stdlib.h> // NULL
#include <string.h>  /* stcmp, strcpy */
#include "sched.h"


// channel struct
struct channel{
    char name[100];
    int num;
};


// show struct
struct show{
    Channel channel;
    char name[100];
    char day[4];
    struct showTime start_time;
    struct showTime end_time;
};


// System Channel store
ChannelList SYSTEM_CHANNEL_LIST = NULL;
ShowList SYSTEM_SHOW_LIST = NULL;


// collect channel number
int channelNum(Channel c){
    return c->num;
}

// collect channel name
const char* channelName(Channel c){
    return c->name;
}


// collect all channel list
ChannelList allChannels(){
    return SYSTEM_CHANNEL_LIST;
}


/* create a new channel and add to schedule
 * if another channel with the same number exists,
 * cannot create -- return NULL
 */
Channel addChannel(int num, const char* name){

    // check if another channel with the same number exists,
    if (findChannel(num)){
        return NULL; // channel not created -- return NULL
    }

    // Creating a new channel instance with num and name
    Channel c;
    c = (Channel) malloc(sizeof(struct channel));
    strcpy(c->name,name); // copy name
    c->num = num;

    // add channel to the system
    ChannelList cl;
    cl = (ChannelList) malloc(sizeof(struct channelNode)); // create a channel list item
    cl->c = c; // add channel
    cl->next = SYSTEM_CHANNEL_LIST;
    SYSTEM_CHANNEL_LIST = cl;

    return c;
}


/*
 * find a channel, given its number
 * if no such channel, returns NULL
 */
Channel findChannel(int num){
    ChannelList list = NULL;
    list = SYSTEM_CHANNEL_LIST;
    Channel chan;

    while (list != NULL) {
        chan = list->c;
        if (channelNum(chan)==num)
            return chan;
        list = list->next;
    }
    return NULL;

}


Show addShow(Channel c, const char* name, const char* day,
             struct showTime start, struct showTime end){

    // check if channel with this number is exists or not
    if (!findChannel(channelNum(c))){
        return NULL; // channel not created -- return NULL
    }


    // now we will search all the shows and will try to find if time conflicts
    Show show;
    const char *s_day;
    struct showTime start_, end_;
    ShowList list;
    list = SYSTEM_SHOW_LIST;

    // start checking
    while (list != NULL) {
        show = list->s;

        s_day = showDay(show);
        start_ = showStart(show);
        end_ = showEnd(show);

        int start_time = start_.hour * 60 + start_.min;
        int end_time = end_.hour * 60 + end_.min ;
        int s_start_time = start.hour * 60 + start.min;
        int s_end_time = end.hour * 60 + end.min ;

        // checking time conflicts
        if ( strcmp(day,s_day)==0 && !((start_time>s_start_time && start_time>s_end_time) || (end_time<s_start_time && end_time<s_end_time)  )){
            return NULL;
        }

        list = list->next;

    }





    // Creating a new show instance
    show = (Show) malloc(sizeof(struct show));
    show->channel=c;
    strcpy(show->name,name);
    strcpy(show->day,day);
    show->start_time = start;
    show->end_time=end;

    // add show to the system
    ShowList sl;
    sl = (ShowList) malloc(sizeof(struct showNode)); // create a show list item
    sl->s = show; // add show
    sl->next = SYSTEM_SHOW_LIST;
    SYSTEM_SHOW_LIST = sl;
    return show;
}

Channel showChannel(Show s){
    return s->channel;
}








const char* showName(Show s){
    return s->name;
}
const char* showDay(Show s){
    return s->day;
}
struct showTime showStart(Show s){
    return s->start_time;
}
struct showTime showEnd(Show s){
    return s->end_time;
}










ShowList findShows(Channel c, const char* name,
                   const char *day, struct showTime* time){

    ShowList result = NULL;

    Show show;
    const char *s_day;
    struct showTime start_, end_;
    ShowList list;
    list = SYSTEM_SHOW_LIST;

    // start checking
    while (list != NULL) {
        show = list->s;
        list = list->next;

        if (c!=NULL && channelNum(showChannel(show)) != channelNum(c))
            continue;

        if (name!=NULL && strcmp(showName(show),name)!=0)
            continue;

        if (day!=NULL && strcmp(showDay(show),day)!=0)
            continue;

        if (time!=NULL){
            start_ = showStart(show);
            end_ = showEnd(show);
            int start_time = start_.hour * 60 + start_.min;
            int end_time = end_.hour * 60 + start_.min;
            int t_time = time->hour * 60 + time->min;
            if (!(start_time <= t_time && end_time>=t_time)){
                continue;
            }
        }

        ShowList sl;
        sl = (ShowList) malloc(sizeof(struct showNode)); // create a show list item
        sl->s = show; // add show
        sl->next = result;
        result = sl;




    }

    return result;
}