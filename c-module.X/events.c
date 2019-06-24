#include "events.h"
#include "queue.h"
#include "test.h"

static Queue events;

static Queue subsequentEvents;

void resetEventQueues() {
    queueReset(&events);
    queueReset(&subsequentEvents);
}

void enqueueEvent(EventType event, unsigned char value) {
    enqueue(&events, event);
    enqueue(&events, value);
}

Event* dequeueEvent() {
    static Event event;
    if (queueIsEmpty(&events)) {
        return 0;
    } else {
        event.event = dequeue(&events);
        event.value = dequeue(&events);
        return &event;
    }
}

void enqueueSubsequentEvent(EventType event, unsigned char value) {
    enqueue(&subsequentEvents, event);
    enqueue(&subsequentEvents, value);
}

Event* dequeueSubsequentEvent() {
    static Event event;
    if (queueIsEmpty(&subsequentEvents)) {
        return 0;
    } else {
        event.event = dequeue(&subsequentEvents);
        event.value = dequeue(&subsequentEvents);
        return &event;
    }
}

#ifdef TEST
void test_queuing_events() {
    Event* event;
    
    resetEventQueues();
    assertEquals("EV101", enqueueEvent(NO_EVENT, 10));
    assertEquals("EV102", dequeueSubsequentEvent(), 0);

    event = dequeueEvent();
    assertEquals("EV103", event->event, NO_EVENT);    
    assertEquals("EV104", event->value, 10);

    assertEquals("EV105", dequeueEvent(), 0);
}
void test_queuing_subsequent_events() {
    Event* event;
    
    resetEventQueues();
    assertEquals("EV201", enqueueSubsequentEvent(NO_EVENT, 12));
    assertEquals("EV202", dequeueEvent(), 0);

    event = dequeueSubsequentEvent();
    assertEquals("EV203", event->event, NO_EVENT);    
    assertEquals("EV204", event->value, 12);

    assertEquals("EV205", dequeueSubsequentEvent(), 0);
}

void test_events() {
    test_queuing_events();
    test_queuing_subsequent_events();
}
#endif