/* 
 * ECE 209 Summer 2018 -- Program 3
 * main() function, implements user interface
 */

#include <stdio.h>  /* fopen, fscanf, printf, fclose */
#include <stdlib.h>  /* atoi, free, EXIT_xxx */
#include <ctype.h>   /* isspace */
#include <string.h>  /* stcmp, strcasecmp */

#include "sched.h"

/* local functions - defs below */
void newChan(FILE *);
void newShow(FILE *);
void query(FILE *);
FILE* openCmdFile();
void printShows(ShowList);
void printChannels(ChannelList);
const char *stdDay(const char *);
int checkTime(int hr, int min);
int lessTime(struct showTime *, struct showTime *);
void readline(FILE *, char *, int);

int main(int argc, char *argv[]) {

    /* I/O stream for commands -- could be file, could be stdin */
    FILE *cmdstream = stdin;
    int prompt = 1;
    int status = 0;
    char inStr[21]; /* generic input buffer */
    ChannelList cList = NULL;

    while (1) { /* break out of loop when done */

        if (prompt) {
            printf("\nCommand: ");
            fflush(stdout);
        }
        status = fscanf(cmdstream, "%20s", inStr);
        /* if end of command file, switch to stdin */
        if ((status == EOF) && !prompt) {
            fclose(cmdstream);
            cmdstream = stdin;
            prompt = 1;
            continue;
        }

        /* Add Channel */
        if (strcmp(inStr, "c") == 0) {
            newChan(cmdstream);
        }

            /* Add Show */
        else if (strcmp(inStr, "s") == 0) {
            newShow(cmdstream);

        }

            /* Find Shows */
        else if (strcmp(inStr, "f") == 0) {
            query(cmdstream);
        }

            /* All Channels */
        else if (strcmp(inStr, "a") == 0) {
            cList = allChannels();
            printf("\n--- CHANNEL LIST ---\n");
            printChannels(cList);
        }

            /* eXecute Command File */
        else if (strcmp(inStr, "x") == 0) {
            if (cmdstream == stdin) {
                cmdstream = openCmdFile();
                if (cmdstream == NULL) {
                    cmdstream = stdin;
                    prompt = 1;
                }
                else {
                    prompt = 0;
                }
            }
            else {
                printf("\n ERROR: Cannot use x within a command file.\n");
            }
        }

            /* Quit */
        else if (strcmp(inStr, "q") == 0) {
            break;
        } else {
            printf("Unknown command: %s\n", inStr);
        }
    }

    return EXIT_SUCCESS;
}

/* Get channel number and name, add to schedule */
void newChan(FILE *str) {
    char inStr[21];
    int inNum;
    int status;
    int prompt = (str == stdin);
    Channel chan = NULL;

    if (prompt) {
        printf("ADD CHANNEL\nChannel number: ");
        fflush(stdout);
    }
    status = fscanf(str, "%d", &inNum);
    if (status == 0) {
        printf("ERROR: Illegal channel number\n");
        return;
    }
    if (prompt) {
        printf("Channel name: ");
        fflush(stdout);
    }
    readline(str, inStr, 20);
    chan = addChannel(inNum, inStr);
    if (chan == NULL) {
        printf("ERROR: Unable to add channel %d (%s)\n", inNum, inStr);
    }
    return;
}

/* Get channel, name, day, and times; add to schedule */
void newShow(FILE *str) {
    char inStr[21];
    char dayStr[21];
    const char *day;
    int inNum;
    int status;
    int prompt = (str == stdin);
    Channel chan = NULL;
    Show show = NULL;
    struct showTime start, end;

    if (prompt) {
        printf("NEW SHOW\nChannel number: ");
        fflush(stdout);
    }
    status = fscanf(str, "%d", &inNum);
    if (status == 0) {
        printf("ERROR: Illegal channel number.\n");
        return;
    }
    chan = findChannel(inNum);
    if (chan == NULL) {
        printf("ERROR: Unable to find channel %d.\n", inNum);
        return;
    }
    if (prompt) {
        printf("Show name: ");
        fflush(stdout);
    }
    readline(str, inStr, 20);
    if (prompt) {
        printf("Day: ");
        fflush(stdout);
    }
    fscanf(str, "%20s", dayStr);
    day = stdDay(dayStr);  /* convert to standard day string */
    if (day == NULL) {
        printf("ERROR: Illegal day: %s.\n", dayStr);
        return;
    }
    if (prompt) {
        printf("Start time (hh:mm): ");
        fflush(stdout);
    }
    status = fscanf(str, "%d:%d", &start.hour, &start.min);
    if ((status != 2) || !checkTime(start.hour, start.min)) {
        printf("ERROR: Illegal start time.\n");
        return;
    }
    if (prompt) {
        printf("End time (hh:mm): ");
        fflush(stdout);
    }
    status = fscanf(str, "%d:%d", &end.hour, &end.min);
    if ((status != 2) || !checkTime(end.hour, end.min)) {
        printf("ERROR: Illegal end time.\n");
        return;
    }
    if (lessTime(&end, &start)) {
        printf("ERROR: End time cannot be before start time.\n");
        return;
    }
    show = addShow(chan, inStr, day, start, end);
    if (show == NULL) {
        printf("ERROR: Unable to add show \"%s\".\n", inStr);
    }
    return;
}

/* get search values, call findShows, print results */
void query(FILE *str) {
    char nameStr[21] = {0};
    char dayStr[21] = {0};
    char timeStr[21] = {0};
    char chanStr[21] = {0};
    int inNum;
    struct showTime time, *timeP;
    Channel chan;
    char *name;
    const char *day;
    ShowList list;
    int prompt = (str == stdin);
    int status;

    if (prompt) {
        printf("\nFIND SHOWS\nChannel num (or *): ");
        fflush(stdout);
    }
    fscanf(str, "%20s", chanStr);
    if (strcmp(chanStr, "*") == 0) {
        chan = NULL;
    } else {
        inNum = atoi(chanStr);
        chan = findChannel(inNum);
        if (chan == NULL) {
            printf("ERROR: Cannot find channel %d.\n", inNum);
            return;
        }
    }
    if (prompt) {
        printf("Show name (or *): ");
        fflush(stdout);
    }
    readline(str, nameStr, 20);
    if (strcmp(nameStr, "*") == 0) {
        name = NULL;
    } else {
        name = nameStr;
    }
    if (prompt) {
        printf("Day (or *): ");
        fflush(stdout);
    }
    fscanf(str, "%s", dayStr);
    if (strcmp(dayStr, "*") == 0) {
        day = NULL;
    } else {
        day = stdDay(dayStr);
        if (day == NULL) {
            printf("ERROR: Illegal day: %s\n", dayStr);
            return;
        }
    }
    if (prompt) {
        printf("Time (hh:mm or *): ");
        fflush(stdout);
    }
    fscanf(str, "%20s", timeStr);
    if (strcmp(timeStr, "*") == 0) {
        timeP = NULL;
    } else {
        /* sscanf reads data from a string */
        status = sscanf(timeStr, "%d:%d", &time.hour, &time.min);
        if ((status != 2) || !checkTime(time.hour, time.min)) {
            printf("ERROR: Illegal time.\n");
            return;
        }
        timeP = &time;
    }
    list = findShows(chan, name, day, timeP);
    printf("\n--- SHOW LIST ---\n");
    if (list == NULL) {
        printf("No matching shows.\n");
    } else {
        printShows(list);
    }
    return;
}

/* open a new command file and return the file handle */
FILE* openCmdFile() {
    /* when this function is called, we know that input is coming from stdin */
    char fileStr[21] = {0};
    FILE *fp = NULL;

    printf("\nEXECUTE COMMENTS\nFile name: ");
    fflush(stdout);
    scanf("%20s", fileStr);
    fp = fopen(fileStr, "r");
    if (!fp) {
        printf("\nError: Cannot open file %s", fileStr);
    }
    return fp;
}

/* print a list of shows */
void printShows(ShowList list) {
    Show show;
    Channel chan;
    int cnum;
    const char *cname;
    const char *name;
    const char *day;
    struct showTime start, end;
    ShowList tmp;

    while (list != NULL) {
        show = list->s;
        chan = showChannel(show);
        cnum = channelNum(chan);
        cname = channelName(chan);
        name = showName(show);
        day = showDay(show);
        start = showStart(show);
        end = showEnd(show);

        printf("Channel %d (%s)\n", cnum, cname);
        printf("%-20s %3s %02d:%02d-%02d:%02d\n",
               name, day, start.hour, start.min, end.hour, end.min);

        tmp = list;
        list = list->next;
        free(tmp); /* delete list node -- no longer needed */
    }
}

/* print a list of shows */
void printChannels(ChannelList list) {
    Channel chan;
    int cnum;
    const char *cname;
    ChannelList tmp;

    while (list != NULL) {
        chan = list->c;
        cnum = channelNum(chan);
        cname = channelName(chan);
        printf("Channel %d (%s)\n", cnum, cname);

        tmp = list;
        list = list->next;
        free(tmp); /* delete list node -- no longer needed */
    }
}

/* translates string into one of the standard literal strings */
const char *stdDay(const char *day) {
    char *standard[7] = {MON, TUE, WED, THU, FRI, SAT, SUN};
    int i;
    /* compare against standard strings, ignoring case */
    for (i = 0; i < 7; i++) {
        if (strcasecmp(day, standard[i]) == 0) return standard[i];
    }
    /* if no match, return NULL */
    return NULL;
}

/* returns 1 for value time (00:00-23:59), 0 otherwise */
int checkTime(int hr, int min) {
    int good = 1;
    if ((hr < 0) || (hr > 23) ||
        (min < 0) || (min > 59))
        good = 0;

    return good;
}

/* reads characters up to linefeed (or until limit is reached) */
void readline(FILE *str, char *buf, int limit) {
    int i;
    char c;
    for (i = 1; i <= limit; i++) {
        fscanf(str, "%c", &c);
        /* skip initial whitespace */
        if ((i == 1) && isspace(c)) {
            i--;  /* prevent i from incrementing */
        } else if (c == '\n') {
            break;
        } else {
            *buf++ = c;
        }
    }
    *buf = '\0';
    while (c != '\n') fscanf(str, "%c", &c);
}

/* tells whether first time is earlier than second time */
int lessTime(struct showTime *t1, struct showTime *t2) {
    int min1 = t1->hour * 60 + t1->min;
    int min2 = t2->hour * 60 + t2->min;
    return min1 < min2;
}
