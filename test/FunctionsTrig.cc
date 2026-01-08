#include <criterion/criterion.h>
#include <SilikegoCore/Engine.h>
#include <SilikegoCore/Math.h>
#include <SilikegoCore/Operator.h>
#include <SilikegoCore/Math.h>
#include <SilikegoCore/Operator.h>

#include <math.h>
#include <float.h>

#define SUITE FunctionsTrigonometric
#define INSTALL_FUNCTION InstallMathFunctions
#include "FunctionMacros.h"

using Silikego::InstallMathFunctions;

TestFunctionForEquality(SinZeroIsZero,
    "sin", 0.0,
    0.0
)

TestFunctionForEpsilon(SinNintyIsAbouOne,
    "sin", 1.0, DBL_EPSILON,
    M_PI*0.5
)

TestFunctionForEpsilon(SinOneEightyIsAbouZero,
    "sin", 0.0, DBL_EPSILON,
    M_PI
)

TestFunctionForEpsilon(SinTwoSeventyIsAboutNegativeOne,
    "sin", -1.0, DBL_EPSILON,
    M_PI*1.5
)

TestFunctionForEquality(CosZeroIsOne,
    "cos", 1.0,
    0.0
)

TestFunctionForEpsilon(CosNintyIsAboutZero,
    "cos", 0.0, DBL_EPSILON,
    M_PI*0.5
)

TestFunctionForEpsilon(CosOneEightyIsAboutNegativeOne,
    "cos", -1.0, DBL_EPSILON,
    M_PI
)

TestFunctionForEpsilon(CosTwoSeventyIsAboutZero,
    "cos", 0.0, DBL_EPSILON,
    M_PI*1.5
)

TestFunctionForEquality(TanZeroIsZero,
    "tan", 0.0,
    0.0
)

TestFunctionForEpsilon(TanFortyFiveIsAbouOne,
    "tan", 1.0, DBL_EPSILON,
    M_PI*0.25
)

TestFunctionForEpsilon(TanOneThirtyFiveIsAbouNegativeOne,
    "tan", -1.0, DBL_EPSILON,
    M_PI*0.75
)

TestFunctionForEpsilon(TanOneEightyIsAbouZero,
    "tan", 0.0, DBL_EPSILON,
    M_PI
)
