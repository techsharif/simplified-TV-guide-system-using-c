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
            return list->c;
        list = list->next;
    }
    return NULL;

}


Show addShow(Channel c, const char* name, const char* day,
             struct showTime start, struct showTime end){

    Show s;
    return s;
}

Channel showChannel(Show s){
    return s->channel;
}








const char* showName(Show s){
    return "sdf";
}
const char* showDay(Show s){
    return "MON";
}
struct showTime showStart(Show s){
    struct showTime ss;
    return ss;
}
struct showTime showEnd(Show s){
    struct showTime ss;
    return ss;
}










ShowList findShows(Channel c, const char* name,
                   const char *day, struct showTime* time){
    ShowList sl;
    return sl;
}