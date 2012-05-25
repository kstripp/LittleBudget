/***************************************************************************
 *                                                                         *
 *   BOMmer - The Product Configuration Manager                            *
 *                                                                         *
 *   Copyright (C) 2005 by G. M. Beddingfield <gabriel@teuton.org>         *
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


/* testmain.cpp
   G. M. Beddingfield
   13MAR2005

   Main program to test the Tester class.  I was tempted to test Tester with
   itself... but that's pretty goofy.  If it breaks... how will I know?  So,
   I test Tester using assert().  I create a convenience macro b_assert() to
   make the syntax quicker.
*/

#include <iostream>
#include <string>
#include <cassert>  // just in case
#include "tester.h"

using namespace std;
using namespace bommer;

class Mytest
{

public:
	Mytest();
	~Mytest();
	int run(void);
};

Mytest::
Mytest(void)
{ }

Mytest::
~Mytest()
{ }

#ifndef b_assert
#define b_assert(x,p,f,t) assert((x).passed()==(p)); \
                          assert((x).failed()==(f)); \
                          assert((x).total()==(t));
#endif // b_asert
	
// Even though Mytest is derived from Tester, there are two goals:
// 1. to test the Tester class.
// 2. to test itself.
int Mytest::
run(void)
{
	Tester x;
	try {
		x.Test(int(5)==int(5),__Tester__);
		b_assert(x,1,0,1);
	} catch(TestErr e) {
		cout << e.what() << endl;
	}
	try {
		x.Test(int(5)==int(6),__Tester__);
		b_assert(x,1,1,2);
	} catch(TestErr e) {
		cout << e.what() << endl;
	}
	try {
		x.Test(string("foo")==string("foo"),__Tester__);
		b_assert(x,2,1,3);
	} catch(TestErr e) {
		cout << e.what() << endl;
	}
	try {
		x.Test(string("foo")!=string("foo"),__Tester__);
		b_assert(x,2,2,4);
	} catch(TestErr e) {
		cout << e.what() << endl;
	}
	try {
		x.Test(string("foo")==string("bar"),__Tester__);
		b_assert(x,2,3,5);
	} catch(TestErr e) {
		cout << e.what() << endl;
	}
	try {
		x.Test(true,__Tester__);
		b_assert(x,3,3,6);
	} catch(TestErr e) {
		cout << e.what() << endl;
	}
	try {
		x.Test(false,__Tester__);
		b_assert(x,3,4,7);
	} catch(TestErr e) {
		cout << e.what() << endl;
	}
	try {
		x.Test(!true,__Tester__);
		b_assert(x,3,5,8);
	} catch(TestErr e) {
		cout << e.what() << endl;
	}
	try {
		x.Test(!false,__Tester__);
		b_assert(x,4,5,9);
	} catch(TestErr e) {
		cout << e.what() << endl;
	}
	try {
		x.Test("foo",__Tester__);
		b_assert(x,5,5,10);
	} catch(TestErr e) {
		cout << e.what() << endl;
	}

	// uncomment to force a b_assert failure.
	// b_assert(x,6,5,10);

	cout << x.Report() << endl;
	cout << "Should be 5 pass 5 fail 10 total." << endl;
	cout << "If we've made it this far, it's a success." << endl;
	return EXIT_SUCCESS;
}

int main(void)
{
	Mytest t;

	return t.run();
}
