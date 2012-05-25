#ifndef __LB_H__
#define __LB_H__
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


/* lb.h
   Gabriel M. Beddingfield
   29MAR2005

   Application definition for lb.
   For merging a gnucash report
   with a published budget.
*/

#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "bud.h"

using namespace std;

#ifndef VERSION
#define VERSION "1.0.1"
#endif

class lbrc;

class lb
{
public:
	typedef vector<string> _FileT;
	typedef vector<double> _ScaleT;

private:
	bool _bad;   // General "bad inputs" flag
	bool _quiet; // Show copyright statement?
	_FileT _budget; // name of budget file
	_FileT _report; // name of gnucash report file
	_ScaleT _scale; // scale factor for budget
	lbrc* _rc;

	// Private methods and functions
	void set_args1( const int i, int& o, int* f, const int argc, const char* argv[] );
	void set_args2( const int i, int& o, int* f, const int argc, const char* argv[] );
	int  set_argsn( const int i, int& o, int* f, const int argc, const char* argv[] );

protected:
	void parse_args(int argc, const char* argv[]);
	void argv2vector(_FileT& f, const char* argv[], int i, int in);
	void argv2floatvector(_ScaleT& f, const char* argv[], int i, int in);
	void csv2vector(_FileT& f, const char* s);
	void csv2flra(vector<double>& f, const char* s);
	template<class A, class B, class C, class D>
		inline string ONELINE_(A,B,C,D) const;
	template<class A, class B, class C, class D, class E, class F, class G>
		inline string ONELINE(A,B,C,D,E,F,G) const;
	accounts parse_reports( ostream* os = 0 );

public:
	lb(void);
	lb(int argc, const char* argv[]);
	~lb();

	// The command-line application of this class
	// ==========================================
	int run(ostream& os = cout);
	int run(int argc, const char* argv[], ostream& os = cout);
	accounts parse_reports( int argc, const char* argv[] );
	int report_accounts( accounts& accts, ostream& os = cout );

	// Generic informational properties
	// ================================
	string about(void) const;
	string usage(void) const;
	bool bad(void) const { return _bad; }
	bool good(void) const { return !_bad; }
	bool quiet(void) const { return _quiet; }
	const _FileT& budget_fns(void) const { return _budget; }
	const _FileT& report_fns(void) const { return _report; }
	double scale(unsigned int i) const { return ( i&&(i<_scale.size()) ) ? _scale[i] : 0.0; }


};

class lb_error : public runtime_error
{
public:
	lb_error(const string& msg = "") :
		runtime_error(msg) { }
};

template <class A, class B, class C, class D>
inline string lb::ONELINE_(A _a, B _b, C _c, D _d) const
{
	ostringstream os;

	os << setw(37) << (_a) << "  "
	   << setw(9) << setprecision(2)
	   << setiosflags(ios::right|ios::fixed)
	   << (_b) << " "
	   << setw(9) << (_c) << " " 
	   << setw(9) << (_d) << endl;

	return os.str();
}

template <class A, class B, class C, class D, class E, class F, class G>
inline string lb::ONELINE(A _a, B _b, C _c, D _d, E _e, F _f, G _g) const
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


#endif // _LB_H_
