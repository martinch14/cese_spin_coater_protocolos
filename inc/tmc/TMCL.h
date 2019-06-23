#ifndef TMCL_H
#define TMCL_H

#include <stddef.h>
#include "tmc/helpers/API_Header.h"
#include "comm/cdc.h"
#include "boards/Board.h"
#include "defs.h"
#include "main.h"
#include "ciaaIO.h"

void tmcl_init();
void tmcl_process();
void tmcl_boot();

#endif /* TMCL_H */
