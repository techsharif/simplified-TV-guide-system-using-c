/* 
 * ECE 209 -- Summer 2018
 * Program 3: tvguide
 * 
 * sched.h -- header file for types and functions
 * 
 * This file specifies structs, typedefs, and function prototypes
 * that create and manipulate the schedule information for the
 * tvguide application. 
 * 
 */

/* Channel and Show are main types to be used. */
/* You must define "struct channel" and "struct show" in sched.c */

typedef struct channel * Channel;
typedef struct show * Show;

/* structs used for linked lists of channels, shows */

struct channelNode {
  Channel c;  /* points to a struct channel */
  struct channelNode *next;
};

typedef struct channelNode * ChannelList;

struct showNode {
  Show s;  /* points to a struct show */
  struct showNode *next;
};

typedef struct showNode * ShowList;

/* representing time */

struct showTime {
  int hour;  /* 0 = 12:00am, 23 = 11:00pm */
  int min;   /* minutes past the hour */
};

/* Day is a constant three-character string, all caps.
 * You can use the following macros to refer to days.
 * (Use of these macros is NOT REQUIRED.)
 */
#define MON "MON"
#define TUE "TUE"
#define WED "WED"
#define THU "THU"
#define FRI "FRI"
#define SAT "SAT"
#define SUN "SUN"

/*
 * FUNCTIONS
 */

/* Channel data type */

/* create a new channel and add to schedule
 * if another channel with the same number exists,
 * cannot create -- return NULL
 */
Channel addChannel(int num, const char* name);
/* NOTE: must copy name into struct, because caller may 
 * use its string for some other data later
 */

int channelNum(Channel c);
const char* channelName(Channel c);  /* pointer to name string */


/* Show data type */

/* create a new show and add it to schedule
 * if can't create for some reason (bad channel, already
 * another shows scheduled for that time on that channel),
 * return NULL
 */
Show addShow(Channel c, const char* name, const char* day, 
             struct showTime start, struct showTime end);
/* NOTE: must copy name into struct, because caller may 
 * use its string for some other data later
 */

Channel showChannel(Show s);
const char* showName(Show s);  /* pointer to name string */
const char* showDay(Show s);   /* pointer to day string */
struct showTime showStart(Show s);
struct showTime showEnd(Show s);


/*
 * get list of all channels
 */
ChannelList allChannels();
/* NOTE: must create a NEW LIST to return, caller will
 * delete this list (but not the channels!) when it's done using it
 */

/*
 * find a channel, given its number
 * if no such channel, returns NULL
 */
Channel findChannel(int num);

/* query function -- finds all shows in the schedule
 * that match the search criteria
 *  
 * any of the input parameters can be NULL, which means
 * that the value is "don't care" and should be considered a
 * "wildcard" in the search
 * 
 * See program spec for more information.
 */
ShowList findShows(Channel c, const char* name, 
                   const char *day, struct showTime* time);
/* NOTE: must create a NEW LIST to return, caller will
 * delete this list (but not the shows!) when it's done using it
 */
