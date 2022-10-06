#ifndef _FLIRTPP_H_
#define _FLIRTPP_H_

#ifdef WIN32
    #pragma optimize("g",on)
    #pragma optimize("i",on)
    #pragma optimize("p",on)
    #pragma optimize("t",on)
    // #pragma optimize("x",on)
    #pragma optimize("b2",on)
    #pragma optimize("2",on)
    #pragma optimize("y-",on)
#endif

#include <stdio.h>
#include <typeinfo>

#include "flirt.h"

#include "Array.h"
#include "Displacement.h"
#include "DistanceMeasure.h"
#include "Image.h"
#include "Log.h"
#include "Optimizer.h"
#include "Smoother.h"
#include "Solver.h"
#include "UpdateRule.h"
#include "StopRule.h"


#endif
