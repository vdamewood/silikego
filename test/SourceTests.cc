#include <criterion/criterion.h>
#include <SilikegoCore/Input.h>
#include <SilikegoCore/StringInput.h>

#include <string>

using std::string;
using Silikego::StringInput;

#define SourceTest(NAME, STRING) \
Test(StringSourceTests, NAME) \
{ \
    string input(STRING); \
    std::unique_ptr<Silikego::Input> source = std::make_unique<StringInput>(input); \
\
    for (int i = 0; i < input.size(); i++) \
    { \
        cr_assert(source->character() == input[i]); \
        source->advance(); \
    } \
    cr_assert(source->character() == '\0'); \
}

SourceTest(EmptryString, "")
SourceTest(RandomString, "nk4jsormahlr0493+%@^%!#@@#$@##$^")
SourceTest(PlausibleExpression, "2 + 3 * 4.5^abs(3d6) - 5 / 1")
