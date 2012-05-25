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

/* budget.cpp
   Gabriel M. Beddingfield
   02OCT2004

   Class definition for ReadBudget object.
*/

#include <cctype>
#include "budget.h"

using namespace std;

#define BUFFER 256

void ReadBudget::getline( string& s ) {
	char ch;
	s.erase();
	if( !good() || eof() ) return;

	get(ch);
	while( ('\n'!=ch)&&(!eof()) ) {
		s.push_back(ch);
		get(ch);
	}
}

bool ReadBudget::more(void)
{
	string s;
	size_t pos;
	string delims = ",\n";

	if(!good()) { close(); return false; }
	if(eof()) { close(); return false; }

	// Skip over blank lines and comments
	getline(s);
	while((s.empty())||(s[0]=='#')||isspace(s[0])) {
		if(eof()) {
			close();
			return false;
		}
		getline(s);
	}

	// Read in one record
	if( ('\''==s[0])||('\"'==s[0]) ) {
		string odelim;
		odelim.push_back(s[0]);
		s.erase(0,1);
		pos = s.find_first_of(odelim);
		if( string::npos == pos ) {
			close();
			return false;
		}
		while( '\\'==s[pos-1] ) {
			--pos;
			s.erase( pos, 1 );
			++pos;
			pos = s.find_first_of(odelim,pos);
			if( string::npos == pos ) {
				close();
				return false;
			}
		}
		s.erase(pos,1);
	} else {  // deal with quotes
		pos = s.find_first_of(delims);
	}
	if( (string::npos==pos)||(0==pos) ) {
		close();
		return false;
	}
	name_ = s.substr(0,pos);

	++pos;
	val_ = atof( s.substr(pos).c_str() );

	// Check for bad input
	for( string::const_iterator i = name_.begin();
	     i != name_.end() ;
	     ++i ) {
		if( !isprint(*i) ) return false;
	}

	return true;
}
