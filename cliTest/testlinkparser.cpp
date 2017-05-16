// Copyright (C) 2017 Petros Koutsolampros

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "catch.hpp"
#include "../depthmapXcli/linkparser.h"
#include "argumentholder.h"

TEST_CASE("LINK args invalid", "")
{
    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "LINK", "-lf"};
        LinkParser p;
        REQUIRE_THROWS_WITH(p.parse(ah.argc(), ah.argv()), Catch::Contains("-lf requires an argument"));
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "LINK", "-lnk"};
        LinkParser p;
        REQUIRE_THROWS_WITH(p.parse(ah.argc(), ah.argv()), Catch::Contains("-lnk requires an argument"));
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "LINK", "-lf", "linksfile1", "-lf", "linksfile2"};
        LinkParser p;
        REQUIRE_THROWS_WITH(p.parse(ah.argc(), ah.argv()), Catch::Contains("-lf can only be used once at the moment"));
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "LINK", "-lf", "linksfile1", "-lnk", "0,0,0,0"};
        LinkParser p;
        REQUIRE_THROWS_WITH(p.parse(ah.argc(), ah.argv()), Catch::Contains("-lf can not be used in conjunction with -lnk"));
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "LINK", "-lnk", "LaLaLaLa"};
        LinkParser p;
        REQUIRE_THROWS_WITH(p.parse(ah.argc(), ah.argv()), Catch::Contains("Invalid link provided"));
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "LINK", "-lnk", "1.2;3.4;5.6;7.8"};
        LinkParser p;
        REQUIRE_THROWS_WITH(p.parse(ah.argc(), ah.argv()), Catch::Contains("Invalid link provided"));
    }
}

TEST_CASE("LINK args valid", "valid")
{
    // for this set of tests a difference less than 0.001 should
    // suffice to signify that two floats are the same
    const float EPSILON = 0.001;

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "LINK", "-lnk", "1.2,3.4,5.6,7.8"};
        LinkParser cmdP;
        cmdP.parse(ah.argc(), ah.argv());
        REQUIRE(cmdP.getMergeLines().size() == 1);
        REQUIRE(cmdP.getMergeLines()[0].start().x == Approx(1.2).epsilon(EPSILON));
        REQUIRE(cmdP.getMergeLines()[0].start().y == Approx(3.4).epsilon(EPSILON));
        REQUIRE(cmdP.getMergeLines()[0].end().x == Approx(5.6).epsilon(EPSILON));
        REQUIRE(cmdP.getMergeLines()[0].end().y == Approx(7.8).epsilon(EPSILON));
    }

    {
        ArgumentHolder ah{"prog", "-f", "infile", "-o", "outfile", "-m", "LINK", "-lnk", "1.2,3.4,5.6,7.8", "-lnk", "0.1,0.2,0.3,0.4"};
        LinkParser cmdP;
        cmdP.parse(ah.argc(), ah.argv());
        REQUIRE(cmdP.getMergeLines().size() == 2);
        REQUIRE(cmdP.getMergeLines()[0].start().x == Approx(1.2).epsilon(EPSILON));
        REQUIRE(cmdP.getMergeLines()[0].start().y == Approx(3.4).epsilon(EPSILON));
        REQUIRE(cmdP.getMergeLines()[0].end().x == Approx(5.6).epsilon(EPSILON));
        REQUIRE(cmdP.getMergeLines()[0].end().y == Approx(7.8).epsilon(EPSILON));
        REQUIRE(cmdP.getMergeLines()[1].start().x == Approx(0.1).epsilon(EPSILON));
        REQUIRE(cmdP.getMergeLines()[1].start().y == Approx(0.2).epsilon(EPSILON));
        REQUIRE(cmdP.getMergeLines()[1].end().x == Approx(0.3).epsilon(EPSILON));
        REQUIRE(cmdP.getMergeLines()[1].end().y == Approx(0.4).epsilon(EPSILON));
    }

}
