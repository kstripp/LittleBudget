#ifndef __BUD_H__
#define __BUD_H__
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

/* bud.h
   Gabriel M. Beddingfield
   02OCT2004

   Object declaration for data when merging
   a GnuCash report with a "published"
   budget.

   Since it's a simple object, this is also
   the definition.
*/

#include <map>
#include <string>

using namespace std;

class bud {
	float           act_;
	float          plan_;
	float       ytd_act_; // ytd data *without* act_
	float      ytd_plan_; // ytd data *without* plan_
	bool           flag_; // true by default, false after being processed.
public:
	bud(float a=0.0, float p=0.0, float ya=0.0, float yp=0.0) :
		act_(a), plan_(p), ytd_act_(ya), ytd_plan_(yp), flag_(true)
		{ }
	virtual ~bud() { }

	// properties:
	float actual(void) const { return act_; }
	float planned(void) const { return plan_; }
	float ytd_actual(void) const { return (act_ + ytd_act_); }
	float ytd_planned(void) const { return (plan_ + ytd_plan_); }
	float diff(void) const { return act_ - plan_; }
	float ytd_diff(void) const { return ytd_actual() - ytd_planned(); }
	bool  flag(void) const { return flag_; }

	// Note:  the diff(k) methods have been removed because it doesn't
	// scale well when you are spanning multiple files.

	// methods:
	bool set(void) { return flag_ = true; }
	bool clear(void) { return flag_ = false; }
	bool toggle(void) { return (flag_)?false:true; }
	bool reset(void) { act_=0.0; plan_=0.0; return flag_=true; }
	float actual(const float v) { return act_ += v;	}
	float planned(const float v) { return plan_ += v; }
	float ytd_actual(const float v) {
		ytd_act_ += v;
		return ytd_actual();
	}
	float ytd_planned(const float v) {
		ytd_plan_ += v;
		return ytd_planned();
	}
};

typedef std::map<string, bud> accounts;

#endif // __BUD_H__
