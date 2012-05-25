/***************************************************************************
 *                                                                         *
 *   LB - The Little Budget Utility for GnuCash                            *
 *                                                                         *
 *   Copyright (C) 2004,2005 by Gabriel M. Beddingfield                    *
 *   gabriel@teuton.org                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* t_bud.cpp
   Gabriel M. Beddingfield
   10APR2005

   Test program to test and make sure the functionality of bud is
   according to my design intent.
*/

#include <iostream>
#include <limits> // for epsilon
#include "bud.h"
#include "../test/tester.h"

using namespace std;
using namespace bommer;

// precision is 7*epsilon because of roundoff errors
static const float eps = (7.0*numeric_limits<float>::epsilon());

class TestBud : public Tester
{

protected:
	void constructors(void);
	void methods(void);
	void operators(void);

public:
	TestBud(void) { };
	~TestBud() { };

	int run(void);
};

int main(void)
{
	try {
		TestBud t;

		return t.run();
	} catch (TestErr e) {
		cout << e.what() << endl;
		return EXIT_FAILURE;
	} catch (...) {
		cout << "Unhandled exception encountered in "
		     << __FILE__ << endl;
		return EXIT_FAILURE;
	}

	return EXIT_FAILURE; // never reach.
}

int TestBud::run(void)
{
	constructors();
	methods();
	operators();

	cout << Report() << endl;

	return verdict();
}

void TestBud::constructors(void)
{
	bud a, b(5.0,1.3), c(6.6);

	Test( a.actual()<=eps        , __Tester__ );
	Test( a.planned()<=eps       , __Tester__ );
	Test( (b.actual()-5.0)<=eps  , __Tester__ );
	Test( (b.planned()-1.3)<=eps , __Tester__ );
	Test( (c.actual()-6.6)<=eps  , __Tester__ );
	Test( (c.planned()<=eps)     , __Tester__ );

	return;
}

// methods() should also test all properties that
// haven't already been tested by constructors()

void TestBud::methods(void)
{
	bud a, b(3.145,9.111);

	Test( a.actual()<=eps            , __Tester__ );
	Test( a.planned()<=eps           , __Tester__ );
	Test( a.diff()<=eps              , __Tester__ );
//	Test( a.diff(5.1)<=eps           , __Tester__ );
	a.actual( 6.66 );
	Test( (a.actual()-6.66)<=eps     , __Tester__ );
	a.planned( -7.341 );
	Test( (a.planned()+7.341)<=eps   , __Tester__ );
	Test( (a.actual()-6.66)<=eps     , __Tester__ );
	Test( (a.diff()-14.001)<=eps     , __Tester__ );
//	Test( (a.diff(3.1)+29.4171)<=eps , __Tester__ );
	a.reset();
	Test( a.actual()<=eps            , __Tester__ );
	Test( a.planned()<=eps           , __Tester__ );
	Test( a.diff()<=eps              , __Tester__ );
//	Test( a.diff(5.1)<=eps           , __Tester__ );

	Test( (b.actual()-3.145)<=eps    , __Tester__ );
	Test( (b.planned()-9.111)<=eps   , __Tester__ );
	Test( (b.diff()-5.966)<=eps      , __Tester__ );
//	Test( b.diff(0.0)<=eps           , __Tester__ );
	b.actual( -1.1 );
	b.planned( 4.1 );
	Test( (b.actual()-2.045)<=eps    , __Tester__ );
	Test( (b.planned()-13.211)<=eps  , __Tester__ );
	Test( (b.diff()-11.166)<=eps     , __Tester__ );
//	Test( (b.diff(2.0)-24.377)<=eps  , __Tester__ );
	b.reset();
	Test( b.actual()<=eps            , __Tester__ );
	Test( b.planned()<=eps           , __Tester__ );
	Test( b.diff()<=eps              , __Tester__ );
//	Test( b.diff(4.3)<=eps           , __Tester__ );

	return;
}

void TestBud::operators(void)
{
	// no operators
	return;
}
