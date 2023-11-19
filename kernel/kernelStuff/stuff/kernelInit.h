#pragma once
#include "../memory/memory.h"

void InitKernel(BootInfo* bootInfo);

void PrepareMemory(BootInfo* bootInfo);

void PrepareInterrupts();

void DoPCIWithoutMCFG(BootInfo* bootInfo);
void PrepareACPI(BootInfo* bootInfo);

void DoGdtStuff();