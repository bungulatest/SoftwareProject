#ifndef ErrorHandling_h__
#define ErrorHandling_h__
#include "../ModelInfrastructure/gui.h"

void freeGui(GUI* gui);

void freeGuiArray();
void exitProgram(void);
void freeModel(GUI* gui);
#endif // ErrorHandling_h__
