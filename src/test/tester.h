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


/* tester.h
   G. M. Beddingfield
   13MAR2005

   Declares an object to be used for testing the BOMmer application.
   It's written with an eye to making it a generic test wuite for code
   development.

   In concept and operation, it borrows from Bruce Eckel's TestSuite
   provided with "Thinking in C++ Vol 2."  However, since his licence is
   not consistent with the licence for BOMmer, I've had to create my own
   implementation.  For this reason, I've intentionally have not looked
   at the source code for his TestSuite.  Therefore, any resemblance to
   my implementation and his is coincidental.  I have seen the public
   interface for his class in the text of the book, and this class resembles
   that interface.
*/
#ifndef __BOMMER_TESTER__
#define __BOMMER_TESTER__

#include <ostream>
#include <sstream>
#include <string>
#include <cstdlib>

using std::string;
using std::ostringstream;

namespace bommer
{

// This macro, when used with the Tester::Test() function, makes typing
// a little easier.
//
// I chose not to implement Tester::Test() as a macro.

#define __Tester__ (__FILE__),(__LINE__)

class TestErr
	{
		string _m;

	public:
		TestErr(const char* file, int line) {
			ostringstream os;
			os << "Tester::Test reports failure from " << file
			   << " at line " << line;
			_m=os.str();
		}
		~TestErr() { }
		string what() { 
			return _m;
		}
	};

class Tester
	{
		int  _p, _f, _n;  // pass, fail, number of tests.

	public:

		Tester(void);
		virtual ~Tester();

		bool Test(bool,const char*, int);   // Tallies the results of the test.
		void Reset(void);  // Reinitializes class
		std::string Report(void);

		inline int passed(void) const { return _p; }
		inline int failed(void) const { return _f; }
		inline int total(void) const { return _n; }
		inline int verdict(void) const { return (_f)?(EXIT_FAILURE):(EXIT_SUCCESS); }
	};

}

#endif // __BOMMER_TESTER__
