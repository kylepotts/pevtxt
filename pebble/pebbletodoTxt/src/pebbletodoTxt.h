#pragma once
extern char tasks[8][18];
extern char projects[8][18];
extern int32_t size;

typedef struct {
char taskString[18];
char taskProject[18];
char taskContext[18];


}taskItem;

extern taskItem taskItems[8];
extern bool isFinished;


void getData(char* string);
