#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define getname "FUNCTION" __FUNCTION__ "CALL CAUSED -> "

struct Message {
  char* stringlog;
};

Message* MessageConstruct(const char* mess, size_t size);

void MessageDestroy(Message* src);

struct Mystack {
  Message** logs;
  size_t size = 0;
  size_t pointer = 0;
};

Mystack* StackPush(Mystack* src, Message* mess);

void StackPop(Mystack* src);

Mystack* StackResize(Mystack* src, size_t new_size);

Mystack* StackPush(Mystack* src, Message mess);

void StackDestroy(Mystack* src);

struct BACKTRACE {
  Mystack* stack;
};

BACKTRACE* BackTraceConstruct();

void BackTraceDestroy(BACKTRACE* src);

BACKTRACE* AddMessage(BACKTRACE* src, const char* str, size_t size);

BACKTRACE* BackTracePop(BACKTRACE* src);

BACKTRACE* PrintTrace(BACKTRACE* src);
