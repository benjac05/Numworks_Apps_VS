#include <ion.h>

extern const void * _process_stack_start;
extern const void * _process_stack_end;

bool Ion2::stackSafe() {
  volatile int stackDummy;
  volatile void * stackPointer = &stackDummy;
  return (stackPointer >= &_process_stack_end && stackPointer <= &_process_stack_start);
}
