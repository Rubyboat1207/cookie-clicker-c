#include<stdbool.h>

typedef struct RandomEventType {
    void (*on_triggered)();
    void (*on_timeout)();
    bool (*on_clicked)(char*, double*);
} RandomEventType;

RandomEventType* get_random_event();