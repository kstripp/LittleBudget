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

/* t_budget.cpp
   Gabriel M. Beddingfield
   10APR2005

   Program to test the ReadBudget class to make sure that it
   works as designed.

   It will create temporary files in a beginning with:
   .testtemp_
*/


#include <iostream>
#include <fstream>
#include <cstdio>  // for remove()
#include <limits>  // for eps below
#include <cstring>  // for strcmp()
#include "../test/tester.h"
#include "budget.h"

using namespace std;
using namespace bommer;

static const float eps = (7.0*numeric_limits<float>::epsilon());

const char _bud_good_dat[] = "# budget.csv\n"
"#\n"
"# Monthly budget\n"
"Income:Salary,1000.00\n"
"Expenses:Tithe,100.00\n"
"Assets:Current Assets:Savings Account,100.00\n"
"Expenses:Rent,250.00\n"
"Expenses:Grocery,250.00\n"
"Expenses:Auto:Gas,80.00\n"
"Expenses:Utilities:Electric,80.00\n"
"Expenses:Misc,140.00\n\n";

const char _bud_good2_dat[] = "# budget.csv\n"
"#\n"
"# Monthly budget\n"
"'Income:Salary',1000.00\n"
"\"Expenses:Tithe\",100.00\n"
"\"Assets:Current Assets:Savings Account\",100.00\n"
"\"Expenses:Rent\",250.00\n"
"\'Expenses:Grocery\',250.00\n"
"Expenses:Auto:Gas,80.00\n"
"\"Expenses:Util\\\"ities:Electric\",80.00\n"
"\'Expen\\\'ses:Misc\',140.00\n\n";

const char _bud_bad_dat[] = "<html><!-- Blah:Blah,wAAAh>\n"
"<head>\n"
"  <title>My Home Page.\n"
"</head>\n"
"<body>\n"
"<H3>Hello, World!!</H3>\n"
"</body>\n"
"</html>\n\n";



class TestReadBudget : public Tester
{

protected:
	void constructors(void);
	void methods(void);
	void operators(void);

public:
	TestReadBudget(void) { }
	~TestReadBudget() { }

	int run(void);
};


int main(void)
{
	try {
		TestReadBudget t;

		return t.run();
	} catch (TestErr e) {
		cout << e.what() << endl;
		return EXIT_FAILURE;
	} catch (...) {
		cout << "Unhandled exception in "
		     << __FILE__ << endl;
		return EXIT_FAILURE;
	}

	return EXIT_FAILURE; // Never reach here
}

int TestReadBudget::run(void)
{
	constructors();
	methods();
	operators();

	cout << Report() << endl;

	return verdict();
}

void TestReadBudget::constructors(void)
{
	// This here code is just to prove my test
	// concept.  Delete it and create new
	// stuff later.
	const char cfn[] = ".testtemp_t_budget";
	const string sfn(cfn);

	fstream fp(cfn,ios::out);
	fp << _bud_good_dat;
	fp.close();

	{
		ReadBudget b(cfn);

		// Test that the following works in c++
		string s = "ABCD";
		Test( s.substr(4) == "" , __Tester__ );

		Test( b.more(), __Tester__ );

		Test( b.acct()=="Income:Salary", __Tester__ );
		Test( (b.val()-1000.00)<=eps, __Tester__ );
		Test( b.more(), __Tester__ );
		Test( b.acct()=="Expenses:Tithe", __Tester__ );
		Test( (b.val()-100.00)<=eps, __Tester__ );
		Test( b.more(), __Tester__ );
		Test( b.acct()=="Assets:Current Assets:Savings Account", __Tester__ );
		Test( (b.val()-100.00)<=eps, __Tester__ );
		Test( b.more(), __Tester__ );
		Test( b.acct()=="Expenses:Rent", __Tester__ );
		Test( (b.val()-250.00)<=eps, __Tester__ );
		Test( b.more(), __Tester__ );
		Test( b.acct()=="Expenses:Grocery", __Tester__ );
		Test( (b.val()-250.00)<=eps, __Tester__ );
		Test( b.more(), __Tester__ );
		Test( b.acct()=="Expenses:Auto:Gas", __Tester__ );
		Test( (b.val()-80.00)<=eps, __Tester__ );
		Test( b.more(), __Tester__ );
		Test( b.acct()=="Expenses:Utilities:Electric", __Tester__ );
		Test( (b.val()-80.00)<=eps, __Tester__ );
		Test( b.more(), __Tester__ );
		Test( b.acct()=="Expenses:Misc", __Tester__ );
		Test( (b.val()-140.00)<=eps, __Tester__ );
		Test( b.more()==false, __Tester__ );
	}

	fp.open(cfn,ios::out);
	fp << _bud_good2_dat;
	fp.close();

	{
		ReadBudget b(cfn);

		// Test that the following works in c++
		string s = "ABCD";
		Test( s.substr(4) == "" , __Tester__ );

		Test( b.more(), __Tester__ );

		Test( b.acct()=="Income:Salary", __Tester__ );
		Test( (b.val()-1000.00)<=eps, __Tester__ );
		Test( b.more(), __Tester__ );
		Test( b.acct()=="Expenses:Tithe", __Tester__ );
		Test( (b.val()-100.00)<=eps, __Tester__ );
		Test( b.more(), __Tester__ );
		Test( b.acct()=="Assets:Current Assets:Savings Account", __Tester__ );
		Test( (b.val()-100.00)<=eps, __Tester__ );
		Test( b.more(), __Tester__ );
		Test( b.acct()=="Expenses:Rent", __Tester__ );
		Test( (b.val()-250.00)<=eps, __Tester__ );
		Test( b.more(), __Tester__ );
		Test( b.acct()=="Expenses:Grocery", __Tester__ );
		Test( (b.val()-250.00)<=eps, __Tester__ );
		Test( b.more(), __Tester__ );
		Test( b.acct()=="Expenses:Auto:Gas", __Tester__ );
		Test( (b.val()-80.00)<=eps, __Tester__ );
		Test( b.more(), __Tester__ );
		Test( b.acct()=="Expenses:Util\"ities:Electric", __Tester__ );
		Test( (b.val()-80.00)<=eps, __Tester__ );
		Test( b.more(), __Tester__ );
		Test( b.acct()=="Expen\'ses:Misc", __Tester__ );
		Test( (b.val()-140.00)<=eps, __Tester__ );
		Test( b.more()==false, __Tester__ );
	}

	fp.open(cfn,ios::out);
	fp << _bud_bad_dat;
	fp.close();

	{
		ReadBudget b(sfn);

		Test( b.more(), __Tester__ );
		Test( b.acct()=="<html><!-- Blah:Blah", __Tester__ );
		Test( b.val() <= eps, __Tester__ );
		Test( b.more()==false, __Tester__ );
	}


	remove(cfn);

	return;
}

void TestReadBudget::methods(void)
{
	// these were all adequately tested in constructors()
	return;
}

void TestReadBudget::operators(void)
{
	// these were all adequately tested in constructors()
	return;
}
