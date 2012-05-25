#ifndef __LBRC_H__
#define __LBRC_H__
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

/* lbrc.h
   Gabriel M. Beddingfield
   2005-08-09

   This class manages the global settings for lb.
*/

#include <vector>
#include <list>
#include <ostream>
#include "bud.h"

using std::vector;
using std::ostream;
using std::list;

class lbrcItem : public bud
{
	friend class lbrc;
	enum { Section = 1, Report = 2 } _type;
	string _id;
	string _name;
	string _match;
	char   _sign;
	lbrc*  _parent;

public:
	// constructors
	lbrcItem( lbrc* _parent );
	lbrcItem( float act, float plan, float ytd_act, float ytd_plan, lbrc* _parent );
	~lbrcItem(void);

	// Report interface
	string oneline( accounts::iterator i );
	string summary( accounts* a );
	bool match( accounts::iterator i );

private:
	string sectionSummary( accounts* i );
	string reportSummary( accounts* i );
	unsigned char line2lbrcItem( const string& ln,
				     string* err );
};

class lbrc : public list<lbrcItem>
{
public:
	lbrc( const string& filename = "" );
	lbrc( const char* filename );
	~lbrc();

	// Report interface
	string header( accounts* a );
	string footer( accounts* a );
	void reset( const string& filename = "" );

protected:
	void parse_config( const string& filename = "" );
	void parse_config_text( string& s );
	void default_config( void );

private:
	void init( const string& filename );

};

#endif // __LBRC_H__
