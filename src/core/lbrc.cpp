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

/* lbrc.cpp
   Gabriel M. Beddingfield
   2005-08-09

   This class manages the global settings for lb.
*/

#include <cassert>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ios>
#include <fstream>
#include <cstdlib> // for getenv()
#include "lbrc.h"
#include "lb.h" // for lb_error
#include "bud.h"

using namespace std;

template<class A, class B, class C, class D, class E, class F, class G>
inline static string ONELINE(A _a,B _b,C _c,D _d,E _e,F _f,G _g)
{
	ostringstream os;
	os << setw(37) << (_a) << "  "				     
	   << setw(9) << setprecision(2) 
	   << setiosflags(ios::right|ios::fixed) 
	   << (_b) << " "	
	   << setw(9) << (_c) << " " 
	   << setw(9) << (_d) << " | " 
	   << setw(9) << (_e) << " " 
	   << setw(9) << (_f) << " " 
	   << setw(9) << (_g) 
	   << endl;
	return os.str();
}




/** \brief Determines if the given file exists.
 *
 */

inline static bool exists( const string& fn )
{
	ifstream fs;
	fs.open( fn.c_str(), ios::in );
	return fs.is_open();
}

/** \brief Gives the name of the resource file.
 *
 * Returns the filename with the complete path of the files location.
 * Right now it only gives either ./.lbrc or $HOME/.lbrc.  If neither
 * exists, it returns an empty string.
 *
 */

static string findfile(void)
{
	string fn(".lbrc");
	char* str;
	if( exists(fn) ) return fn;

	str = getenv("HOME");
	if( str != 0 ) {
		fn = string(str) +
			string("/") + fn;
		if( exists(fn) ) return fn;
	}
	return string();
}

lbrcItem::lbrcItem(lbrc* parent) : bud( 0.0, 0.0, 0.0, 0.0 ), _parent(parent)
{

}

lbrcItem::lbrcItem( float act, float plan, float ytd_act, float ytd_plan, lbrc* parent ) :
	bud( act, plan, ytd_act, ytd_plan ), _parent(parent)
{

}

lbrcItem::~lbrcItem(void)
{

}

lbrc::lbrc( const string& filename )
{
	init(filename);
}

/** \brief Same as default constructor.
 *
 */
lbrc::lbrc( const char* filename )
{
	init( string(filename) );
}

lbrc::~lbrc()
{

}

void lbrc::init( const string& filename )
{
	if( filename.empty() ) {
		parse_config( findfile() );
	} else {
		assert( exists(filename) );
		parse_config( filename );
	}
	return;
}

void lbrc::reset( const string& filename )
{
	clear();
	assert( size() == 0 );
	init( filename );
}
string lbrc::header( accounts* a )
{
	ostringstream os;
	os << endl;
	os << ONELINE("Account","Planned","Actual","Diff","YTD Plan","YTD Act","YTD Diff");
	os << ONELINE("=====================================",
		      "=========","=========","=========","=========","=========","=========");
	return os.str();
}

string lbrc::footer( accounts* a )
{
	return string();
}

string lbrcItem::oneline( accounts::iterator i )
{
	ostringstream os;
	// Match i->first(), if matches, then insert to the stream.
	if( i->second.flag() && match(i) ) {
		os << ONELINE( i->first,
			       i->second.planned(),
			       _sign*i->second.actual(),
			       _sign*i->second.actual() - i->second.planned(),
			       i->second.ytd_planned(),
			       _sign*i->second.ytd_actual(),
			       _sign*i->second.ytd_actual() - i->second.ytd_planned() );
		i->second.clear();
		this->clear();
		// Add these sums to ours, for summary()
		planned( i->second.planned() );
		actual( _sign * i->second.actual() );
		ytd_planned( i->second.ytd_planned() - i->second.planned() );
		ytd_actual( _sign * (i->second.ytd_actual() - i->second.actual()) );
	}
	return os.str();
}

string lbrcItem::summary( accounts* a )
{
	// Two types:  Section and Report
	if( _type == lbrcItem::Section ) {
		return sectionSummary( a );
	} else {
		return reportSummary( a );
	}
}

string lbrcItem::sectionSummary( accounts* )
{
	if( flag() ) return string(); // i.e. no accounts were matched
	ostringstream os;
	os << ONELINE( "",
		       "=========",
		       "=========",
		       "=========",
		       "=========",
		       "=========",
		       "=========")
	   << ONELINE( _name,
		       planned(),
		       actual(),
		       diff(),
		       ytd_planned(),
		       ytd_actual(),
		       ytd_diff() )
	   << endl;
	return os.str();
}

string lbrcItem::reportSummary( accounts* )
{
	lbrc::iterator i;
	ostringstream os;
	size_t pos;
	unsigned char c;

	for( i=_parent->begin() ; &(*i)!=this ; ++i ) {
		pos = _match.find( i->_id );
		if( pos != string::npos ) {
			c = (pos==0)?'+':_match[pos-1];
			switch( c ) {
			case '+':
				planned( i->planned() );
				actual( i->actual() );
				ytd_planned( i->ytd_planned() - i->planned() );
				ytd_actual( i->ytd_actual() - i->actual() );
				break;
			case '-':
				planned( -i->planned() );
				actual( -i->actual() );
				ytd_planned( -i->ytd_planned() + i->planned() );
				ytd_actual( -i->ytd_actual() + i->actual() );
				break;
			}
		}
	}
	os << endl
	   << ONELINE( _name,
		       planned(),
		       actual(),
		       diff(),
		       ytd_planned(),
		       ytd_actual(),
		       ytd_diff() )
	   << endl;
	return os.str();
}


bool lbrcItem::match( accounts::iterator i )
{
	if( _match.length() == 0 ) {
		return i->second.flag();
	}
	return ( 0 == _match.compare( 0, _match.length(), i->first, 0, _match.length() ) ) ?
		 true : false;
}

void lbrc::parse_config( const string& filename )
{
	if( filename.empty() || !exists(filename) ) {
		default_config();
	} else {
		assert( exists(filename) );
		ifstream fp( filename.c_str(), ios::in );
		if( fp.bad() ) {
			default_config();
			return;
		}
		ostringstream os;
		os << fp.rdbuf();
		string s = os.str();
		if( s.empty() ) {
			default_config();
		} else {
			parse_config_text( s );
		}
	}

	return;
}

// Here's how I want to read in the configuration file...
// (open file)
// (while have lines) {
//       process the line
// }
//
// Line processing basically has 4 branches:
// + ignore (comment, whitespace)
// + warn -- same as ignore, but warn about bad syntax
// + Section (switch to warn on bad syntax)
// + Report (switch to warn on bad syntax)
//
// We'll still process the file as a whole from the string.
//

static size_t getOneField( const string& s,
			   size_t a,
			   string* x,
			   unsigned char* flag )
{
	size_t b;
	if( a == string::npos ) {
		*flag=2;
		return a;
	}
	a = s.find('\"',a);
	if( a == string::npos ) {
		*flag=2;
		return a;
	}
	++a;
	b = s.find('\"',a);
	if( b == string::npos ) {
		*flag=2;
		return b;
	}
	(*x) = s.substr(a,b-a);
	++b;
	return b;
}

static size_t getSign( const string& s,
		       size_t a,
		       char* x,
		       unsigned char* flag )
{
	size_t b;
	if( a == string::npos ) {
		*flag = 2;
		return a;
	}
	a = s.find_first_of("+-1");
	if( a == string::npos ) {
		*flag = 2;
		return a;
	}
	// Now, the string should be either "1", "+1", or "-1".
	b = s.find_first_not_of("+-1");
	string tmp = s.substr(a,b-a);
	if( tmp == "1" || tmp == "+1" ) {
		*x = 1;
	} else if ( tmp == "-1" ) {
		*x = -1;
	} else {
		*flag = 2;
	}
	return b+1;
}

unsigned char lbrcItem::line2lbrcItem( const string& ln,
				       string* err )
{
	unsigned char flag=0;
	const string ws = " \a\b\f\n\r\t\v";
	size_t a;

	// Ignore comments and whitespace.
	if( '#' == ln[0] ) return flag;
	if( ln.find_first_not_of(ws) == string::npos ) return flag;

	// Determine whether Section, Report, or other.
	if( 0 == ln.compare(0,6,"section",0,6) ) {
		// First part: name
		_type = Section;
		a = getOneField( ln, 7, &_id, &flag );
		a = getOneField( ln, a, &_name, &flag );
		a = getOneField( ln, a, &_match, &flag );
		a = getSign( ln, a, &_sign, &flag );
		if( 2 == flag ) {
			(*err) = string("Bad syntax ") + ln;
		} else {
			flag = 1;
		}
	} else if( 0 == ln.compare(0,5,"report",0,5) ) {
		// First part: name
		_type = Report;
		a = getOneField( ln, 6, &_name, &flag );
		a = getOneField( ln, a, &_match, &flag );
		if( 2 == flag ) {
			(*err) = string("Bad syntax ") + ln;
		} else {
			flag = 1;
		}
	} else {
		flag=2;
		(*err) = string("Bad syntax: ") + ln;
	}

	return flag;
}

void lbrc::parse_config_text( string& s )
{
	lbrcItem* x;
	size_t ptr;
	string ln;
	string err;
	unsigned char flag;
	int i=0;

	assert( !s.empty() );

	while( !s.empty() ) {
		x = new lbrcItem(this);
		++i;  // processing line 'i'
		ptr = s.find('\n');
		ln = s.substr(0,ptr);
		s.erase(0,ptr+1);
		flag = x->line2lbrcItem( ln, &err );
		switch( flag ) {
		case 0: // Ignored line.
			break;
		case 1: // Houston, we have data.
			push_back(*x);
			break;
		case 2: // Bad syntax - warn
			cerr << "config file line " << i 
			     << " " << err << endl;
			break;
		default:
			throw lb_error("Unhandled error in lbrc parse");
			break;
		}
		delete x;
	}
}

void lbrc::default_config( void )
{
	string s("section \"a\" \"Total Income:\" \"Income:\" 1\n"
		 "section \"b\" \"Total Expenses:\" \"\" -1\n"
		 "report \"Bottom line:\" \"a-b\"\n");

	parse_config_text( s );

	return;
}
