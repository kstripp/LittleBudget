#ifndef __BUDGET_H__
#define __BUDGET_H__
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

/* budget.h
   Gabriel M. Beddingfield
   02OCT2004

   Declaration for the budget input file interface.

   What interface am I wanting.  I want to open
   the file, read in all the budgetary figures,
   and place those figures in the accounts object.

   In pseudo-code:

   OPEN("budget.txt")
   FOR EACH LINE {
      READ("AccountName,Value")
      accts["AccountName"].planned(Value);
   }
   CLOSE("budget.txt")

   This interface is different from what is planned
   for the "actual.html" input file.  This is just
   for comma-separated text.

   So then, I want the actual interface to be:

   budget("filename");
   while(budget.more()) {
     accts[budget.acct()].planned(budget.val());
   }

   When budget.more() returns 0, it will also close the file.
*/
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <cstdlib>

using namespace std;

class ReadBudget : private ifstream {
	string   name_;
	float    val_;

protected:
	 // reads one line into string, deleting whatever's there.
	void getline( string& s );

public:
	ReadBudget(const char * str) : ifstream(str, ios::in) {
		if( !good() ) {
			string s = string("Error opening file ") + 
				string(str);
			throw runtime_error(s);
		}
	}
	ReadBudget(const string& str) : ifstream(str.c_str()) {
		if( !good() ) {
			string s = string("Error opening file ") + 
				string(str);
			throw runtime_error(s);
		}
	}
	~ReadBudget() { }

	bool more(void);
	const string& acct(void) { return name_; }
	float val(void) { return val_; }
};

#endif // __BUDGET_H__
