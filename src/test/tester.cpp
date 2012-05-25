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


/* tester.cpp
   G. M. Beddingfield
   13MAR2005

   Class implementation of the Tester class.
*/

#include <ostream>
#include <sstream>
#include <string>
#include "tester.h"

using namespace bommer;

Tester::
Tester(void) : _p(0), _f(0), _n(0)
{ }

Tester::
~Tester()
{ }

// Tester::Test()
//
// I really like how assert() gives you filename and line number of the error.
// This is how I implemented it without using RTTI, expression templates, etc.
// Use of Test() is made easier using the macro __Tester__ which pops in the
// standard __FILE__ and __LINE__ macros for you.

bool Tester::
Test(bool t, const char* file, int line)
{
	(t)?(++_p):(++_f);
	++_n;
	if(t==0) throw TestErr(file,line);
	return t;
}

void Tester::
Reset(void)
{
	_p=0;
	_f=0;
	_n=0;
}

using std::ostringstream;
using std::string;

string Tester::
Report(void)
{
	ostringstream os;
	os << "Passed: " << _p << "  Failed: " << _f
	   << "  Total: " << _n;
	if((_p+_f)!=_n) os << "  BUG!! ERROR!!  SUM IS INCORRECT!! ";
	return os.str();
}

