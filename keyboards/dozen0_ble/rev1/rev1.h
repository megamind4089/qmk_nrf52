/*
 * rev1.h
 */

#pragma once

#include "quantum.h"


// This a shortcut to help you visually see your layout.
// The first section contains all of the arguements
// The second converts the arguments into a two-dimensional array
#define LAYOUT(\
    K00, K01, K02, K03, K04, K05, \
    K06, K07, K08, K09, K10, K11  \
) \
{ \
    { K00, K01, K02, K03, K04, K05, K06, K07, K08, K09, K10, K11 }, \
}
