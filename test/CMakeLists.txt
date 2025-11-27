project(silikego-test
    VERSION   0.0.0
    LANGUAGES C
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Criterion REQUIRED criterion IMPORTED_TARGET)

add_executable(SilikegoTester)
target_sources(SilikegoTester
PRIVATE
    FunctionMacros.h
    SourceTests.cc
    Value.cc
    Caller.cc
    Functions.cc
    FunctionsTrig.cc
    FunctionsArc.cc
    FunctionsHyper.cc
    FunctionsOps.cc
    Node.cc
    LexerTests.cc
    ParserTests.cc
)
target_link_libraries(SilikegoTester
PRIVATE
    Silikego::SilikegoCore
    PkgConfig::Criterion
)
add_test(NAME Tests
    COMMAND SilikegoTester --verbose --color
)
