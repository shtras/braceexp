#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Parser.h"

using BraceExpand::Parser;

TEST_CASE("Parsing valid strings", "[parser]")
{
    Parser p;
    SECTION("{A,B,C}") { REQUIRE(p.Parse("{A,B,C}")); }
    SECTION("{A,B}{C,D}") { REQUIRE(p.Parse("{A,B}{C,D}")); }
    SECTION("{A,B{C,D}}") { REQUIRE(p.Parse("{A,B{C,D}}")); }
    SECTION("{ABC}") { REQUIRE(p.Parse("{ABC}")); }
    SECTION("ABC") { REQUIRE(p.Parse("ABC")); }
    SECTION("A{B,C}") { REQUIRE(p.Parse("A{B,C}")); }
}

TEST_CASE("Parsing invalid strings", "[parser]")
{
    Parser p;
    SECTION("}ABC") { REQUIRE_FALSE(p.Parse("}ABC")); }
    SECTION("{ABC") { REQUIRE_FALSE(p.Parse("{ABC")); }
    SECTION("}{") { REQUIRE_FALSE(p.Parse("}{")); }
    SECTION("{}") { REQUIRE_FALSE(p.Parse("{}")); }
    SECTION("A,B,C") { REQUIRE_FALSE(p.Parse("A,B,C")); }
    SECTION("{A,}") { REQUIRE_FALSE(p.Parse("{A,}")); }
    SECTION("A B") { REQUIRE_FALSE(p.Parse("A B")); }
    SECTION(" AB") { REQUIRE_FALSE(p.Parse(" AB")); }
    SECTION("AB ") { REQUIRE_FALSE(p.Parse("AB ")); }
    SECTION("{A,B{C,D}") { REQUIRE_FALSE(p.Parse("{A,B{C,D}")); }
}

TEST_CASE("Expanding valid strings", "[main]")
{
    Parser p;
    std::stringstream ss;
    SECTION("{A,B,C}")
    {
        REQUIRE(p.Parse("{A,B,C}"));
        p.Flush(ss);
        REQUIRE(ss.str() == "A B C\n");
    }
    SECTION("{A,B}{C,D}")
    {
        REQUIRE(p.Parse("{A,B}{C,D}"));
        p.Flush(ss);
        REQUIRE(ss.str() == "AC AD BC BD\n");
    }
    SECTION("{A,B{C,D}}")
    {
        REQUIRE(p.Parse("{A,B{C,D}}"));
        p.Flush(ss);
        REQUIRE(ss.str() == "A BC BD\n");
    }
    SECTION("{ABC}")
    {
        REQUIRE(p.Parse("{ABC}"));
        p.Flush(ss);
        REQUIRE(ss.str() == "ABC\n");
    }
    SECTION("ABC")
    {
        REQUIRE(p.Parse("ABC"));
        p.Flush(ss);
        REQUIRE(ss.str() == "ABC\n");
    }
    SECTION("A{B,C}")
    {
        REQUIRE(p.Parse("A{B,C}"));
        p.Flush(ss);
        REQUIRE(ss.str() == "AB AC\n");
    }
    SECTION("{A,B}C")
    {
        REQUIRE(p.Parse("{A,B}C"));
        p.Flush(ss);
        REQUIRE(ss.str() == "AC BC\n");
    }
    SECTION("{A,B}C{D,E}")
    {
        REQUIRE(p.Parse("{A,B}C{D,E}"));
        p.Flush(ss);
        REQUIRE(ss.str() == "ACD ACE BCD BCE\n");
    }
}

TEST_CASE("Expanding invalid strings", "[main]")
{
    Parser p;
    std::stringstream ss;
    SECTION("}ABC")
    {
        REQUIRE_FALSE(p.Parse("}ABC"));
        p.Flush(ss);
        REQUIRE(ss.str() == "");
    }
    SECTION("{ABC")
    {
        REQUIRE_FALSE(p.Parse("{ABC"));
        p.Flush(ss);
        REQUIRE(ss.str() == "");
    }
    SECTION("}{")
    {
        REQUIRE_FALSE(p.Parse("}{"));
        p.Flush(ss);
        REQUIRE(ss.str() == "");
    }
    SECTION("{}")
    {
        REQUIRE_FALSE(p.Parse("{}"));
        p.Flush(ss);
        REQUIRE(ss.str() == "");
    }
    SECTION("A,B,C")
    {
        REQUIRE_FALSE(p.Parse("A,B,C"));
        p.Flush(ss);
        REQUIRE(ss.str() == "");
    }
    SECTION("{A,}")
    {
        REQUIRE_FALSE(p.Parse("{A,}"));
        p.Flush(ss);
        REQUIRE(ss.str() == "");
    }
}
