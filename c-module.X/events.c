#include "events.h"
#include "queue.h"

static Queue events;

static Queue subsequentEvents;

unsigned char anyEvent() {
    if (queueIsEmpty(&events)) {
        return 0;
    } else {
        return 255;
    }
}

void enqueueEvent(EventType event, unsigned char value) {
    enqueue(&events, event);
    enqueue(&events, value);
}

Event dequeueEvent() {
    Event event;
    if (queueIsEmpty(&events)) {
        event.event = NO_EVENT;
        event.value = 0;
    } else {
        event.event = dequeue(&events);
        event.value = dequeue(&events);
    }
    return event;
}

unsigned char anySubsequentEvent() {
    if (queueIsEmpty(&subsequentEvents)) {
        return 0;
    }
    return 255;
}

void enqueueSubsequentEvent(EventType event, unsigned char value) {
    enqueue(&subsequentEvents, event);
    enqueue(&subsequentEvents, value);
}

Event dequeueSubsequentEvent() {
    Event event;
    if (queueIsEmpty(&subsequentEvents)) {
        event.event = NO_EVENT;
        event.value = 0;
    } else {
        event.event = dequeue(&subsequentEvents);
        event.value = dequeue(&subsequentEvents);
    }
    return event;
}
