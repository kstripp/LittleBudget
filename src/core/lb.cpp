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

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstdlib> // for atof()
#include <cassert>
#include "lb.h"
#include "lbrc.h"
#include "bud.h"
#include "budget.h"
#include "report.h"

using namespace std;

#ifndef VERSION
#define VERSION "1.0.1"
#endif


lb::lb(void) : _scale(0,0)
{
	assert( _scale.size()==0 );
	parse_args(0,NULL);
	_rc = new lbrc;
}

lb::lb(int argc, const char* argv[]) : _scale(0,0)
{
	assert( _scale.size()==0 );
	parse_args(argc,argv);
	_rc = new lbrc;
}

lb::~lb()
{
	delete _rc;
}

string lb::about(void) const
{
	ostringstream os;

	os << "LB - The Little Budget Tool " << VERSION << 
		" (C)2004,2005 by G. M. Beddingfield, Romans 11:36";

	return os.str();
}

string lb::usage(void) const
{
	ostringstream os;

	os << "  lb [options] -b budget.txt -r report.html" << endl;
	os << endl;
	os << "  Options:" << endl;
	os << endl;
	os << "  -b budget1.txt budget2.txt ... -- Text file with the" << endl;
	os << "          budget figures." << endl;
	os << endl;
	os << "  -r report1.html report2.html ... -- Cash Flow reports" << endl;
	os << "          from Gnucash." << endl;
	os << endl;
	os << "  -s x.xx ... -- Scale factor for budgets.  All" << endl;
	os << "          figures will be multiplied by N." << endl;
	os << endl;
	os << "  -q -- quiet operation.  No display of title or version.";

	return os.str();
}

int lb::report_accounts( accounts& accts, ostream& os ) {

	lbrc::iterator rci;
	accounts::iterator cur;

	os << _rc->header( &accts );

	for( rci=_rc->begin() ; rci!=_rc->end() ; ++rci ) {
		for( cur=accts.begin() ; cur!=accts.end() ; ++cur ) {
			os << rci->oneline( cur );
		}
		os << rci->summary( &accts );
	}

	os << _rc->footer( &accts );
	return 0;
}

/**************************************************************************************
 * csv2vector()
 * Takes a comma-separated string from x and
 * feeds it into an array of strings.
 * Really only for use in parse_args()
 *************************************************************************************/
void lb::csv2vector(_FileT& f, const char* x)
{
	string s(x);
	size_t pl, pr;

	pl = 0;
	pr = s.find(',');
	while(pr != string::npos) {
		f.push_back(s.substr(pl,(pr-pl)));
		pl = pr+1;
		pr = s.find(',',pl);
	}
	f.push_back( s.substr(pl) );

	return;
}

/**************************************************************************************
 * argv2vector()
 * Takes a subset of the argv's and
 * feeds it into an array of strings.
 * Really only for use in parse_args()
 *************************************************************************************/
void lb::argv2vector(_FileT& f, const char* argv[], int i, int in)
{
	int nd = i+in;
	int j;
	_FileT::iterator k;

	for( j=i+1 ; j<=nd ; j++ ) {
		f.push_back( string(argv[j]) );
	}

	return;
}

/**************************************************************************************
 * argv2floatvector()
 * Takes a subset of the argv's and
 * feeds it into an array of strings.
 * Really only for use in parse_args()
 *************************************************************************************/
void lb::argv2floatvector( _ScaleT& f, const char* argv[], int i, int in)
{
	int nd = i+in;
	int j;

	f.erase(f.begin(),f.end());

	for( j=i+1 ; j<=nd ; j++ ) {
		f.push_back( atof(argv[j]) );
	}

	return;
}

/**************************************************************************************
 * csv2flra()
 * Takes a comma-separated string from x and
 * feeds it into an array of floats.
 * Really only for use in parse_args()
 *************************************************************************************/
void lb::csv2flra(vector<double>& f, const char* x)
{
	string s(x),sub;
	size_t pl, pr;

	pl = 0;
	pr = s.find(',');
	while(pr != string::npos) {
		sub=s.substr(pl,(pr-pl));
		f.push_back( atof(sub.c_str()) );
		pl = pr+1;
		pr = s.find(',',pl);
	}
	sub = s.substr(pl);
	f.push_back( atof(sub.c_str()) );

	return;
}

/******************************************************************************************
 * lb::set_args1()
 *
 * This function is used to process options that require no argument.  For example,
 * if the user passes lb '-q', then we need to parse (and mark parsed) just that we
 * need to be quiet
 *****************************************************************************************/
void lb::set_args1( const int i, int& o, int* f, const int argc, const char* argv[] = NULL )
{
	// calling function should not call use to parse args that have
	// already been parsed.  This checks for that.
	if( f[i] ) {
		_bad = true;
	} else {
		o = 1;
		f[i] = 1;
	}
	return;
}

/******************************************************************************************
 * lb::set_args2()
 *
 * This function is used to process options that require a single argument.  For example,
 * if the user passes lb '-s 1', then we need to parse (and mark parsed) both arguments
 * at one time.
 *****************************************************************************************/
void lb::set_args2( const int i, int& o, int* f, const int argc, const char* argv[] = NULL )
{
	// calling function should not call use to parse args that have
	// already been parsed.  This checks for that.
	if( f[i] ) {
		_bad = true;
		return;
	}

	f[i] = 1;
	o = i+1;
	if( i<argc ) {
		if( f[o] ) {
			_bad = true;
		} else {
			f[o] = 1;
		}
	} else {
		o=i;
		_bad = true;
	}

	if( !argv || (argv[o][0]=='-') ) _bad = true;

	return;
}

/******************************************************************************************
 * lb::set_argsn()
 *
 * This function is used to process options that require an indefinate number of arguments.
 * For example, if the user passes lb '-r foo.html bar.html bat.html', then we need to 
 * parse (and mark parsed) all arguments at one time.
 *
 * Returns: (int) number of arguments to process.
 *                                    Note:  Having a return value is a bit of a hack since
 *                                    I already established an API of returning void.  But
 *                                    there was a critical piece of info I needed back.
 *
 * Arguments:  `i`    - (const) The argv[i] index of the current command-line argument
 *                      that is being processed.
 *             `o`    - (volatile) The option flag for parse_args.  It will be set to i
 *             `f`    - (volatile) The int array of flags to show when the argument has
 *                      been parsed, processed, and tagged.  For example;
 *                         lb -b foo.txt -r bar.html
 *                      '-b' has i=1, 'foo.txt' has i=2, etc.  When we parse -b, we need
 *                      to flag parse_args() to *not* fool with i=2 (foo.txt).
 *             `argc` - The argc passed us by the main calling function (whether main or lb)
 *             `argv` - The argv passed us by the main calling function...
 *****************************************************************************************/
int lb::set_argsn( const int i, int& o, int* f, const int argc, const char* argv[] = NULL )
{
	// calling function should not call use to parse args that have
	// already been parsed.  This checks for that.
	if( f[i] ) {
		_bad = true;
		return 0;
	}

	f[i] = 1;
	o = i;

	int j=i+1;
	while( (j<argc) && !f[j] && (argv[j][0] != '-') ) {   // '-' signals an option
		f[j] = 1;
		++j;
	}

	return j-i-1;
}


/******************************************************************************************
 * lb::parse_args()
 *
 * This function initializes the application by setting the options in the class's data.
 *****************************************************************************************/
void lb::parse_args(int argc, const char* argv[])
{
	int b=0, r=0, s=0, q=0;
	int bn=0, rn=0, sn=0;
	_bad = false;
	_quiet = false;
	_budget.resize(0);
	_report.resize(0);

	// parse the command line
	int chk[argc];
	unsigned int i;
	chk[0] = 1;
	assert( argc >= 0 );
	for( i=1; i<(unsigned int)argc; ++i ) chk[i] = 0;

	for( i=1; i<(unsigned int)argc; ++i) {
		if( chk[i] ) continue;
		if(0==strcmp(argv[i],"-b")) { 
			bn = set_argsn( i, b, chk, argc, argv );
			continue;
		}
		if(0==strcmp(argv[i],"-r")) { 
			rn = set_argsn( i, r, chk, argc, argv );
			continue;
		}
		if(0==strcmp(argv[i],"-s")) { 
			sn = set_argsn( i, s, chk, argc, argv );
			continue;
		}
		if(0==strcmp(argv[i],"-q")) {
			set_args1( i, q, chk, argc, argv );
			continue;
		}
	}

	// Warning:  Using direct user input.
	_quiet = q;
	if(b) argv2vector(_budget,argv,b,bn);
	if(r) argv2vector(_report,argv,r,rn);
	if(s) argv2floatvector(_scale,argv,s,sn);

	// make sure there's a _scale[i] for each _budget[i]
	if( 0==_scale.size() ) _scale.push_back(1.0);
	for( i=_scale.size() ; i<_budget.size() ; ++i ) {
		_scale.push_back( _scale[i-1] );
	}


	// Check for bad inputs.
	if((b>=argc)||(r>=argc)||(s>=argc)||!(b||r))
		_bad = true;

	return;
}

int lb::run(int argc, const char* argv[], ostream& os)
{
	parse_args(argc,argv);
	return run(os);
}

int lb::run(ostream& os)
{

	// Standard checks.
	if(!_quiet) os << about() << endl;

	if(_bad) {
		os << endl << usage() << endl;
		return EXIT_FAILURE;
	}

	_rc->reset();
	accounts accts = parse_reports( &os );

	return report_accounts(accts,os);
}

accounts lb::parse_reports( int argc, const char* argv[] )
{
	parse_args(argc,argv);
	return parse_reports();
}

accounts lb::parse_reports( ostream* os )
{
	accounts accts;

	unsigned int i;
	for( i=0 ; i<_budget.size() ; ++i ) {
		try {
			ReadBudget bfile(_budget[i]);
  
			while(bfile.more()) {
				if(i) {
					accts[bfile.acct()].ytd_planned(_scale[i]*bfile.val());
				} else {
					accts[bfile.acct()].planned(_scale[i]*bfile.val());
				}
			}
		} catch ( runtime_error e ) {
			*os << e.what() << endl;
		}
	}

	for( i=0 ; i<_report.size() ; ++i ) {
		try {
			ReadReport rfile(_report[i]);

			if( (i==0)&&(os!=0) ) {
				*os << "Beginning date: " << rfile.beginning() << "." << endl;
				*os << "Ending date:    " << rfile.ending() << "." << endl;
			}

			while(rfile.more()) {
				if( i ) {
					accts[rfile.acct()].ytd_actual(rfile.val());
				} else {
					accts[rfile.acct()].actual(rfile.val());
				}
			}
		} catch ( runtime_error e ) {
			*os << e.what() << endl;
		}
	}


	return accts;
}
