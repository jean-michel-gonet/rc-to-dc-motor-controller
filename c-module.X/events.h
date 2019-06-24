#ifndef EVENTS__H
#define EVENTS__H

/**
 * Liste des événements du système.
 */
typedef enum EVENT_TYPE {
    /** 
     * No event. 
     * Don't use it outside unit testing.
    */
    NO_EVENT,

    /** 
     * The position of the RC channel has just been captured.
     */
    RC_POSITION_CAPTURED,

    /**
     * Current RC position is unknown.
     * Maybe receiver is off, disconnected, or un pause.
     */
    RC_NO_POSITION,
            
} EventType;

/**
 * Structure containing one event plus
 * one associated value.
 */
typedef struct EVENT {
    EventType event;
    unsigned char value;
} Event;

typedef struct DASHBOARD {
    
} Dashboard;

Dashboard dashboard;

/**
 * Indicates if there is any event waiting in the queue.
 * @return 0 If there isn't any.
 */
unsigned char anyEvent();

/**
 * Enqueues one event with one associate value.
 */
void enqueueEvent(EventType event, unsigned char value);

/**
 * Dequeues one event.
 */
Event dequeueEvent();

/**
 * Indicates if there is any subsequent event waiting in the queue.
 * @return 0 if there isn't any.
 */
unsigned char anySubsequentEvent();

/**
 * Enqueues a subsequent event.
 * A subsequent event is an event that happens as a consequence of the
 * processing of a previous event.
 */
void enqueueSubsequentEvent(EventType event, unsigned char value);

/**
 * Dequeues a subsequent event.
 * A subsequent event is an event that happens as a consequence of the
 * processing of a previous event.
 */
Event dequeueSubsequentEvent();

#endif