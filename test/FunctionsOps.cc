#include <criterion/criterion.h>
#include <SilikegoCore/FunctionCaller.h>

#define SUITE FunctionOperatorTests
#define INSTALL_FUNCTION InstallOperators
#include "FunctionMacros.h"

using Silikego::InstallOperators;

TestFunctionForEquality(TwoPlusThreeIsFive,
    "add", 5,
    2,
    3,
)

TestFunctionForEquality(TwoPlusThreeIsFiveWithFloats,
    "add", 5.0,
    2.0,
    3.0,
)

TestFunctionForEquality(TwoPlusNegativeThreeIsNegativeOne,
    "add", -1,
    2,
    -3,
)

TestFunctionForEquality(TwoPlusNegativeThreeIsNegativeOneWithFloats,
    "add", -1.0,
    2.0,
    -3.0,
)

TestFunctionForEquality(OneEighthPlusHalfIsFiveEighths,
    "add", 0.625,
    0.125,
    0.5,
)

TestFunctionForEquality(FiveIntegerCanBeAdded,
    "add", 28,
    2,
    3,
    5,
    7,
    11,
)

TestFunctionForEquality(FiveFloatsCanBeAdded,
    "add", 28.0,
    2.0,
    3.0,
    5.0,
    7.0,
    11.0,
)

TestFunctionForEquality(AddingMixedTypesGivesFloats,
    "add", 10.0,
    2.0,
    3.0,
    5.0,
)

TestFunctionForEquality(ThreeMinusTwoIsOne,
    "subtract", 1,
    3,
    2,
)

TestFunctionForEquality(ThreeMinusTwoIsOneWithFloats,
    "subtract", 1.0,
    3.0,
    2.0,
)

TestFunctionForEquality(TwoMinusThreeIsNegativeOne,
    "subtract", -1,
    2,
    3,
)

TestFunctionForEquality(TwoMinusThreeIsNegativeOneWithFloats,
    "subtract", -1.0,
    2.0,
    3.0,
)

TestFunctionForEquality(SubtractingMixedNumbersGivesFloat,
    "subtract", 19.5,
    23,
    3.5,
)

TestFunctionForEquality(SubtractingMultipleIntegersDoesntCrash,
    "subtract", 4,
    100,
    50,
    25,
    12,
    6,
    3,
)

TestFunctionForEquality(SubtractingMultipleFloatsDoesntCrash,
    "subtract", 3.125,
    100.0,
    50.0,
    25.0,
    12.5,
    6.25,
    3.125
)

TestFunctionForEquality(TwoTimesThreeIsSix,
    "multiply", 6,
    2,
    3
)

TestFunctionForEquality(TwoTimesThreeIsSixWithFloats,
    "multiply", 6.0,
    2.0,
    3.0
)

TestFunctionForEquality(TwoTimesNegativeFiveIsNegativeTen,
    "multiply", -10,
    2,
    -5
)

TestFunctionForEquality(TwoTimesNegativeFiveIsNegativeTenWithFloats,
    "multiply", -10.0,
    2.0,
    -5.0,
)

TestFunctionForEquality(TwoWrongsMakeARight,
    "multiply", 70,
    -7,
    -10,
)

TestFunctionForEquality(TwoWrongsMakeARightWithFloats,
    "multiply", 70.0,
    -7.0,
    -10.0,
)

TestFunctionForEquality(MultiplyingMultipleValuesDoesntCrash,
    "multiply", 60,
    2,
    3,
    5,
    2,
)

TestFunctionForEquality(MultiplyingMultipleValuesDoesntCrashWithFloats,
    "multiply", 60.0,
    2.0,
    3.0,
    5.0,
    2.0,
)

TestFunctionForEquality(MultiplyingIntegerAndFloatIsFloat,
    "multiply", 30.0,
    2,
    15.0,
)

TestFunctionForEquality(TenDividedByTwoIsFive,
    "divide", 5,
    10,
    2
)

TestFunctionForEquality(TenDividedByTwoIsFiveWithFloats,
    "divide", 5.0,
    10.0,
    2.0
)

TestFunctionForEquality(MultipleDividesDontCrash,
    "divide", 1,
    100,
    25,
    2,
    2
)

TestFunctionForEquality(MultipleDividesDontCrashWithFloats,
    "divide", 1.0,
    100.0,
    25.0,
    2.0,
    2.0
)

TestFunctionForEquality(IntegersWillDivideToFloat,
    "divide", 2.5,
    10,
    4
)

TestFunctionForEquality(DividingIntByFloatWorks,
    "divide", 5.0,
    10,
    2.0
)

TestFunctionForEquality(TwoToTheTwoIsFour,
    "power", 4.0,
    2,
    2
)

TestFunctionForEquality(OneHundredToTheHalfIsTen,
    "power", 10.0,
    100.0,
    0.5
)
