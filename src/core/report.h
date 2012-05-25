#ifndef __REPORT_H__
#define __REPORT_H__
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


/* report.h
   Gabriel M. Beddingfield
   03OCT2004

   The purpose of this object is to take as
   an input the Gnucash HTML cash-flow report
   and integrate it into the LB program.

   The general format for the Gnucash report
   in question is as follows (with regard to
   the data we want to extract):
   =========================================
   <HTML>
   ...
   <H3>Cash Flow - 01/01/2004 to 10/01/2004 for</H3>
   ...
   <HR>

   </TD>
   </TR>
   <TR>
   <TD BGCOLOR="#ffffff">
   Money into selected accounts comes from
   </TD>
   <TD BGCOLOR="#ffffff">
   <BR>
   </TD>
   </TR>
   { then a recurrance of: }
   <TR>
   <TD>
   <A HREF="gnc-register:guid=...">Account:Name</A>
   </TD>
   <TD NOWRAP ALIGN="right">
   $VALUE
   </TD>
   </TR>
   { end of recurrance }
   <TR>
   <TD BGCOLOR="#ffffff">
   Money In
   </TD>
   <TD BGCOLOR="#ffffff" ALIGN="right">
   <FONT SIZE="3"><B>$MONEYIN</B></FONT>
   </TD>
   </TR>
   <TR>
   <TD COLSPAN="2">
   
   <HR>
   
   </TD>
   </TR>
   <TR>
   <TD BGCOLOR="#ffffff">
   Money out of selected accounts goes to
   </TD>
   <TD BGCOLOR="#ffffff">
   <BR>
   </TD>
   </TR>
   { then a recurrance of: }   
   <TR>
   <TD>
   <A HREF="gnc-register:guid=....">Account:Name</A>
   </TD>
   <TD NOWRAP ALIGN="right">
   $VALUE
   </TD>
   </TR>
   { end of recurrance }
   <TR>
   <TD BGCOLOR="#ffffff">
   Money Out
   </TD>
   <TD BGCOLOR="#ffffff" ALIGN="right">
   <FONT SIZE="3"><B>$MONEYOUT</B></FONT>
   </TD>
   </TR>
   <TR>
   <TD COLSPAN="2">
   
   <HR>

   </TD>
   </TR>
   <TR>
   <TD BGCOLOR="#ffffff">
   Difference
   </TD>
   <TD BGCOLOR="#ffffff" ALIGN="right">
   <FONT SIZE="3"><B>$DIFFVAL</B></FONT>
   </TD>
   </TR>
   </TABLE>
   
   </BODY>
   </HTML>
   =================================================

   The desired interface is similar to the one desired
   for ReadBudget (budget.h).  Note that the "money into"
   section (delimited by <HR> tokens) needs special
   handling.  In general, if it refers to an expense
   account, the value should be negated.

   OPEN("report.html")
   GET BEGINNING-DATE
   GET ENDING-DATE
   FOR EACH ENTRY {
      READ("AccountName,Value")
      accts["AccountName"].planned(Value);
   }
   CLOSE("report.html")

   So, let's define the interface like this:

   ReadReport report("filename");
   date beg,end;

   beg=report.beginning();
   end=report.ending();
   while(report.more()) {
     accts[report.acct()].actual(report.val());
   }

   When report.more() returns 0, it will also close the file.
   
*/

#include <string>

using namespace std;

typedef string date;

class ReadReport {

	// Data the user may see:
	bool bad_;
	date beg_;
	date end_;
	string name_;
	float val_;

	// Internal data to ReadReport:
	string data_;
	// Only one of these should be true at a time:
	bool head_;   // parsing the header?
	bool credits_; // parsing the credits?
	bool debits_; // parsing the debits?

	// private methods:
	bool oneacct(void);

public:
	ReadReport(const char* str);
	ReadReport(const string& str);
	~ReadReport() { }

	// methods
	void init(const char* str);
	bool more(void);

	// properties
	const string& acct(void) const { return name_; }
	float val(void) const { return val_; }
	date beginning(void) const { return beg_; }
	date ending(void) const { return end_; }
	bool bad(void) const { return bad_; }
	bool good(void) const { return !bad_; }
};

#endif // __REPORT_H__
