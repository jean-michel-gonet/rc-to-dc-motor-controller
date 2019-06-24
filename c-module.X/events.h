#ifndef EVENTS__H
#define EVENTS__H

/**
 * Liste des événements du système.
 */
typedef enum EVENT {
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
            
} Event;


/**
 * Enqueues one event.
 */
void enqueueEvent(Event event);

/**
 * Dequeues one event.
 */
void Event dequeueEvent();

/**
 * Enqueues a subsequent event.
 * A subsequent event is an event that happens as a consequence of the
 * processing of a previous event.
 */
void enqueueSubsequentEvent(Event event);

/**
 * Dequeues a subsequent event.
 * A subsequent event is an event that happens as a consequence of the
 * processing of a previous event.
 */
void Event dequeueSubsequentEvent();

#endif