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

/* report.cpp
   Gabriel M. Beddingfield
   03OCT2004

   Definition of ReadReport object
*/

#include "report.h"
#include <string>
#include <sstream>
#include <fstream>
#include <cctype>   // for toupper
#include <cstring>  // for atof
#include <iostream>
#include <stdexcept>

using namespace std;

// Constructor.
// Opens file and reads in dates.
ReadReport::ReadReport(const char * str)
{
	init(str);
}

ReadReport::ReadReport(const string& str)
{
	init(str.c_str());
}

/* tagsToUpper()
 *
 * converts HTML tags to upper-case.
 * only converts up to first space.
 *
 * <h3> --> <H3>
 * <img src="foo"> --> <IMG src="foo">
 */
static void tagsToUpper( string* s )
{
	size_t a, b, i;

	a = s->find( "<" );
	while( string::npos != a ) {
		b = s->find_first_of( " >", a );
		for( i=a ; (i<b)&&(i<s->length()) ; ++i )
			(*s)[i] = toupper( (*s)[i] );
		// Note, I thought if b==npos, then find("foo",b) == npos.
		// Either I'm mistaken or there's a g++ bug.
		if( string::npos != b )
			a = s->find( "<", b );
		else
			a = string::npos;
	}
}

void ReadReport::init(const char* str)
{
	// Initialize *all* variables
	bad_ = false;
	data_.erase();
	beg_.erase();
	end_.erase();
	name_.erase();
	val_ = 0.0;
	data_.erase();
	head_ = false;
	credits_ = false;
	debits_ = true;

	// Read the file into a string.
	fstream inf;
	inf.open(str,ios::in);
	if( !inf.good() ) {
		bad_ = true;
		string xj = string("Error opening file ") +
			string(str);
		throw runtime_error( xj );
		return;
	}

	ostringstream os;
	os << inf.rdbuf();
	data_.erase();
	data_ = os.str();

	inf.close();

	// Convert tags to upper case.
	tagsToUpper( &data_ );

	string srch("<H3>Cash Flow - ");
	size_t pa, pb;

	// Get beginning date
	pa = data_.find(srch);
	if( pa == string::npos ) {
		bad_ = true;
		string xj = string("Error opening file ") +
			string(str);
		throw runtime_error( xj );
		return;
	}
	if( pa != string::npos ) pa += srch.size();
	pa = data_.find_first_of("0123456789",pa);
	if( pa != string::npos ) {
		pb = data_.find(" ",pa);
		if( pb != string::npos ) {
			beg_ = data_.substr( pa, pb-pa );
		} else {
			beg_ = data_.substr( pa );
		}
	}
	if( beg_.size()!=8 ) {
		bad_ = true;
		string xj = string("Error opening file ") +
			string(str);
		throw runtime_error( xj );
		return;
	}

	// Get ending date
	pa = data_.find_first_of("0123456789",pb);
	if( pa != string::npos ) {
		pb = data_.find(" ",pa);
		if( pb != string::npos ) {
			end_ = data_.substr( pa, pb-pa );
		} else {
			beg_ = data_.substr( pa );
		}
	}
	if( end_.size()!=8 ) {
		bad_ = true;
		string xj = string("Error opening file ") +
			string(str);
		throw runtime_error( xj );
		return;
	}

	// Delete everything that we've processed
	data_.erase(0,pb);

	if( bad_ ) {
		string xj = string("Error opening file ") +
			string(str);
		throw runtime_error( xj );
	}

	return;
}

bool ReadReport::more(void) {
	size_t pa;
	string tmp;
  
	val_=0;

	if( (data_.size()==0)||(head_&&credits_&&debits_) ) {
		return false;
	}
 
	if(!head_) {
		// First step is to jump to the first <HR>
		pa = data_.find("<HR");

		// Burn thru one <TR> section
		pa = data_.find("<TR",pa);
		pa = data_.find("<TR",pa);

		// Burn through one "Money into selected accounts..."
		pa = data_.find("Money",pa);

		// Delete processed data
		if( pa != string::npos ) ++pa;
		data_.erase(0,pa);

		head_ = true;

	} // if(!head_)

	if(!credits_) {

		// Process one account
		credits_ = oneacct();

		// If there are no more credits, then
		// it's time to switch to debits_.
		if(!credits_) {
			// Negate value of any expense accounts.
			// tmp = "Expenses";
			// if( name_.compare(0,tmp.size(),tmp) )
			//	val_ = -val_;
			// tmp = "Assets";
			// if( name_.compare(0,tmp.size(),tmp) )
			// 	val_ = -val_;
		}else{
			debits_ = false;

			// Jump to the next <HR>
			pa = data_.find("<HR");

			// Burn thru one <TR> section
			pa = data_.find("<TR",pa);
			pa = data_.find("<TR",pa);

			// Burn through one "Money into selected accounts..."
			pa = data_.find("Money",pa);

			// Delete processed data
			if( pa != string::npos ) ++pa;
			data_.erase(0,pa);

		}  // if(!credits_)
	}  // if(!credits_)

	if(!debits_) {

		// process one account
		debits_ = oneacct();
		val_ = -val_;

		if(debits_) {
			return false;
		}

	} // if(!debits_)

	return true;
}

bool ReadReport::oneacct(void) {

// assume that we've already been placed at a <TR>
// The upcoming section either has data, or it is
// a transition between section.  We are either
// looking for "<A HREF=", "$", or "Money".  The
// HREF points us to an account name.  The $ points
// us to the value, and "Money" signals the end.

	size_t pa, pb, mon;

	if( data_.size() == 0 ) return true;

	// Find the end of the current section
	mon = data_.find("Money");

	// Get the account name.
	pa = data_.find("<A HREF=");
	if( pa>mon ) return true;
	pb = string::npos;
	if( pa != string::npos ) pa = data_.find(">",pa);
	if( pa != string::npos ) pb = data_.find("<",pa);
	if( pb != string::npos ) name_ = data_.substr(pa+1,pb-pa-1);
	// Delete processed data
	data_.erase(0,pb);
	if( name_.size() == 0 ) return true;

	// Get the account value.
	pa = data_.find("$");
	if( pa>mon ) return true;
	pb = string::npos;
	if( pa != string::npos ) {
		++pa;
		pb = data_.find(",",pa);
		if( pb != string::npos )
			data_.erase(pb,1);
		pb = data_.find_first_not_of("0123456789.",pa);
		val_ = atof( data_.substr(pa,pb-1).c_str() );
	}

	data_.erase(0,pb);
	if( pb>mon ) return true;
	return false;

}
  
