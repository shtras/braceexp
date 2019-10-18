#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Parser.h"

TEST_CASE("Good cases", "[parser]")
{
    SECTION("{A,B,C}")
    {
        std::string s = "{A,B,C}";
        Parser p(s);
        REQUIRE(p.Parse());
    }
    SECTION("{A,B}{C,D}")
    {
        std::string s = "{A,B}{C,D}";
        Parser p(s);
        REQUIRE(p.Parse());
    }
    SECTION("{A,B{C,D}}")
    {
        std::string s = "{A,B{C,D}}";
        Parser p(s);
        REQUIRE(p.Parse());
    }
    SECTION("{ABC}")
    {
        std::string s = "{ABC}";
        Parser p(s);
        REQUIRE(p.Parse());
    }
    SECTION("ABC")
    {
        std::string s = "ABC";
        Parser p(s);
        REQUIRE(p.Parse());
    }
    SECTION("A{B,C}")
    {
        std::string s = "A{B,C}";
        Parser p(s);
        REQUIRE(p.Parse());
    }
}

TEST_CASE("Bad cases", "[parser]")
{
    SECTION("}ABC")
    {
        std::string s = "}ABC";
        Parser p(s);
        REQUIRE_FALSE(p.Parse());
    }
    SECTION("{ABC")
    {
        std::string s = "{ABC";
        Parser p(s);
        REQUIRE_FALSE(p.Parse());
    }
    SECTION("}{")
    {
        std::string s = "}{";
        Parser p(s);
        REQUIRE_FALSE(p.Parse());
    }
    SECTION("{}")
    {
        std::string s = "{}";
        Parser p(s);
        REQUIRE_FALSE(p.Parse());
    }
    SECTION("A,B,C")
    {
        std::string s = "A,B,C";
        Parser p(s);
        REQUIRE_FALSE(p.Parse());
    }
    SECTION("{A,}")
    {
        std::string s = "{A,}";
        Parser p(s);
        REQUIRE_FALSE(p.Parse());
    }
}

TEST_CASE("Exoansion", "[main]")
{
    SECTION("{A,B,C}")
    {
        std::string s = "{A,B,C}";
        Parser p(s);
        REQUIRE(p.Parse());
        std::stringstream ss;
        p.Flush(ss);
        REQUIRE(ss.str() == "A B C\n");
    }
    SECTION("{A,B}{C,D}")
    {
        std::string s = "{A,B}{C,D}";
        Parser p(s);
        REQUIRE(p.Parse());
        std::stringstream ss;
        p.Flush(ss);
        REQUIRE(ss.str() == "AC AD BC BD\n");
    }
    SECTION("{A,B{C,D}}")
    {
        std::string s = "{A,B{C,D}}";
        Parser p(s);
        REQUIRE(p.Parse());
        std::stringstream ss;
        p.Flush(ss);
        REQUIRE(ss.str() == "A BC BD\n");
    }
    SECTION("{ABC}")
    {
        std::string s = "{ABC}";
        Parser p(s);
        REQUIRE(p.Parse());
        std::stringstream ss;
        p.Flush(ss);
        REQUIRE(ss.str() == "ABC\n");
    }
    SECTION("ABC")
    {
        std::string s = "ABC";
        Parser p(s);
        REQUIRE(p.Parse());
        std::stringstream ss;
        p.Flush(ss);
        REQUIRE(ss.str() == "ABC\n");
    }
    SECTION("A{B,C}")
    {
        std::string s = "A{B,C}";
        Parser p(s);
        REQUIRE(p.Parse());
        std::stringstream ss;
        p.Flush(ss);
        REQUIRE(ss.str() == "AB AC\n");
    }
}
