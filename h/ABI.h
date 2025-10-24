#ifndef ABI_h
#define ABI_h

#include "../lib/hw.h"

extern "C" void supervisorTrap();

extern "C" __attribute__((used)) void handleSupervisorTrap();

#endif
