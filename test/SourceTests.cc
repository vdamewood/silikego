#include <criterion/criterion.h>
#include <SilikegoCore/DataSource.h>
#include <SilikegoCore/StringSource.h>

#include <string>

using std::string;
using Silikego::StringSource;

#define SourceTest(NAME, STRING) \
Test(StringSourceTests, NAME) \
{ \
    string input(STRING); \
    std::unique_ptr<Silikego::DataSource> source = std::make_unique<StringSource>(input); \
\
    for (int i = 0; i < input.size(); i++) \
    { \
        cr_assert(source->current() == input[i]); \
        source->advance(); \
    } \
    cr_assert(source->current() == '\0'); \
}

SourceTest(EmptryString, "")
SourceTest(RandomString, "nk4jsormahlr0493+%@^%!#@@#$@##$^")
SourceTest(PlausibleExpression, "2 + 3 * 4.5^abs(3d6) - 5 / 1")
