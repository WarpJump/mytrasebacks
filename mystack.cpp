#include "mystack.h"
#include "colors.h"
#include "assert.h"

void MessageDestroy(Message* src) {
  assert(src != nullptr);
  assert(src->stringlog != nullptr);
  free(src->stringlog);
  free(src);
}

Message* MessageConstruct(const char* mess, size_t size) {
  assert(mess != nullptr);
  Message* dest = reinterpret_cast<Message*>(calloc(1, sizeof(Message)));

  dest->stringlog = reinterpret_cast<char*>(calloc(size, sizeof(char)));

  assert(dest->stringlog != nullptr);

  memcpy(dest->stringlog, mess, size * sizeof(char));

  return dest;
}

/////////////////////////////////////////////////////////////////////////////////////////

Mystack* StackResize(Mystack* src, size_t new_size) {
  assert(src != nullptr);

  if ((new_size == 0) && (src->size != 0)) {
    src->logs =
        reinterpret_cast<Message**>(realloc(src->logs, sizeof(Message*)));
    src->size = 1;
    return src;
  }

  if (src->size == 0) {
    static const size_t kDefaultMessAllocSize = 8;
    src->logs = reinterpret_cast<Message**>(
        calloc(kDefaultMessAllocSize, sizeof(Message*)));
    src->size = kDefaultMessAllocSize;
    return src;
  }
  src->logs = reinterpret_cast<Message**>(
      realloc(src->logs, sizeof(Message*) * new_size));
  src->size = new_size;
  return src;
}

Mystack* StackPush(Mystack* src, Message* mess) {
  if (src->pointer + 1 >= src->size) {
    StackResize(src, 2 * (src->size));
  }

  src->logs[src->pointer] = mess;

  ++(src->pointer);
  return src;
}

void StackPop(Mystack* src) {
  if (src->pointer == 0 || src->size == 0) {
    return;
  }

  Message* target = src->logs[src->pointer - 1];
  MessageDestroy(target);
  --(src->pointer);
}

void StackDestroy(Mystack* src) {
  for (size_t i = 0; i < (src->pointer); ++i) {
    free(src->logs[i]);
  }
  free(src->logs);
  free(src);
}

///////////////////////////////////////////////////////////////////////////

BACKTRACE* BackTraceConstruct() {
  //creating dynamic struct BACKTRACE and its dynamic field 'stack'
  BACKTRACE* trace = reinterpret_cast<BACKTRACE*>(calloc(1, sizeof(BACKTRACE)));
  trace->stack = reinterpret_cast<Mystack*>(calloc(1, sizeof(Mystack)));
  
  //adding initial message "TRACEBACK: "
  const size_t initialmess_size = sizeof(RedText("TRACEBACK: "));
  AddMessage(trace, RedText("TRACEBACK: "), initialmess_size);
  return trace;
}

void BackTraceDestroy(BACKTRACE* src) {
  //deleting initial message "TRACEBACK: "
  BackTracePop(src);

  StackDestroy(src->stack);
  free(src);
}

BACKTRACE* AddMessage(BACKTRACE* src, const char* str, size_t size) {
  Message* mess = MessageConstruct(str, size);

  StackPush(src->stack, mess);

  return src;
}

BACKTRACE* BackTracePop(BACKTRACE* src) {
  StackPop(src->stack);
  return src;
}

BACKTRACE* PrintTrace(BACKTRACE* src) {
  for (size_t i = 0; i < (src->stack)->pointer; ++i) {
    Message* mess = (src->stack)->logs[i];
    printf("%s ", mess->stringlog);
  }
  putchar('\n');
  return src;
}
