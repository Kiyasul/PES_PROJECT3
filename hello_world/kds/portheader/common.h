#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "time.h"
#include "fsl_debug_console.h"


#define TRUE                (1)
#define FALSE               (0)
#define MULTIPLY            (5)
#define CONSTANT            (2)
#define POWER               (65536)


uint32_t* ptrToMemory;

uint32_t ask_backspace();
uint8_t address_mode_selection();
uint32_t* assign_address(uint32_t);
uint8_t check_bound_for_address(uint32_t*);
