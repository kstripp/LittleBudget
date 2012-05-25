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

/* choosefile.cpp
 * g. m. beddingfield
 * august 2005
 *
 * This is a custom widget that gives a list of files with "up", "down",
 * "add", and "remove" buttons.
 *
 */

#include <qdialog.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qstringlist.h>
#include <qlistbox.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include "filelistbox.h"
#include "choosefile.h"

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

chooseFile::chooseFile( const QStringList& cmdln,
			QWidget* parent,
			const char* name,
			bool modal,
			WFlags fl) :
	QDialog(parent,name,modal,fl)
{

	// Initialize the string list.
	_result = new QStringList(cmdln);

	/*
	 * The intended layout:
	 *
	 * +---------------------------------+
	 * |+-------------------------------+|
	 * ||_scales . _budgets             ||
	 * ||        .                      ||
	 * |+-------------------------------+|
	 * |+-------------------------------+|
	 * ||_reports                       ||
	 * ||                               ||
	 * |+-------------------------------+|
	 * |               +-----++---------+|
	 * |[ ] Quiet      | OK  || Cancel  ||
	 * |               +-----++---------+|
	 * +---------------------------------+
	 *
	 */

	// Set up the layouts
	_vLay = new QVBoxLayout(this,10,-1,"_vLay");
	_budgetLabel = new QLabel(this);
	_vLay->addWidget(_budgetLabel);
	_budgets = new fileListBox(this, "Budget Files:", true);
	_vLay->addWidget(_budgets);
	_reportLabel = new QLabel(this);
	_vLay->addWidget(_reportLabel);
	_reports = new fileListBox(this, "Gnucash report Files:", false);
	_vLay->addWidget(_reports);

	// Set up bottom buttons
	_hLay = new QHBoxLayout(0,5,-1,"_hLay");
	_quiet = new QCheckBox(this,"_quiet");
	_hLay->addWidget(_quiet);
	_spacer = new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Minimum);
	_hLay->addItem(_spacer);
	_okBtn = new QPushButton(this,"okBtn");
	_hLay->addWidget(_okBtn);
	_cxBtn = new QPushButton(this,"cxBtn");
	_hLay->addWidget(_cxBtn);
	_vLay->addLayout(_hLay);

	
	// Set labels in languageChange()
	languageChange();
	
	// Connect signals and slots
	connect( _okBtn, SIGNAL( clicked() ), this, SLOT( okClicked() ) );
	connect( _cxBtn, SIGNAL( clicked() ), this, SLOT( reject() ) );

	// Set a nice default size
	resize( QSize(370,360).expandedTo(minimumSizeHint()) );

	refresh();
}


chooseFile::~chooseFile()
{
	delete _result;
}

/* refresh()
 * Sets the widgets to whatever is in _result;
 *
 */
void chooseFile::refresh(void)
{
	assert(_result);
	QStringList::const_iterator i;
	QStringList budgets, reports, scales, *ptr = 0;
	bool quiet = false;

	// Parse command line to assign to widgets
	for( i=_result->constBegin(); i!=_result->constEnd() ; ++i ) {
		if( (*i)=="-b" ) {
			ptr = &budgets;
			continue;
		} else if( (*i)=="-r" ) {
			ptr = &reports;
			continue;
		} else if( (*i)=="-s" ) {
			ptr = &scales;
			continue;
		} else if( (*i)=="-q" ) {
			quiet = true;
			continue;
		} else if( (*i)[0]=='-' ) {
			ptr = 0;
			continue;
		}

		if( ptr ) (*ptr).push_back(*i);
	}

	for( unsigned int j=scales.size() ; j<budgets.size() ; ++j )
		scales.push_back( QString("1.000") );

	_budgets->setList( budgets, scales );
	_reports->setList( reports );
	_quiet->setChecked( quiet );

}

QStringList chooseFile::get_lb_cmds( const QStringList& cmdln,
				     QWidget* parent,
				     const char* name,
				     bool modal,
				     WFlags fl )
{
	chooseFile dlg(cmdln,parent,name,modal,fl);

	QStringList res(cmdln);
	*dlg._result = cmdln;

	if( dlg.exec() == QDialog::Accepted )
		return *dlg._result;
	else
		return cmdln;

}

void chooseFile::languageChange(void)
{
	setCaption( tr("The Little Budget Tool") );
	_budgetLabel->setText( tr("Budget files:") );
	_budgets->languageChange();
	_reportLabel->setText( tr("Gnucash report files:") );
	_reports->languageChange();
	_quiet->setText( tr("Quiet?") );
	_okBtn->setText( tr("OK") );
	_cxBtn->setText( tr("Cancel") );
	return;
}

void chooseFile::okClicked(void)
{
	QStringList res, dat;
	QStringList::Iterator i;

	res.push_back( "lb" );

	dat = _budgets->getList();
	if( dat.size() > 0 ) res.push_back( "-b" );
	for( i = dat.begin() ; i != dat.end() ; ++i )
		res.push_back( *i );

	dat = _reports->getList();
	if( dat.size() > 0 ) res.push_back( "-r" );
	for( i = dat.begin() ; i != dat.end() ; ++i )
		res.push_back( *i );

	dat = _budgets->getScales();
	if( dat.size() > 0 ) res.push_back( "-s" );
	for( i = dat.begin() ; i != dat.end() ; ++i )
		res.push_back( *i );

	if( _quiet->isChecked() ) res.push_back( "-q" );

	*_result = res;
	accept();

	return;
}

void chooseFile::debugDump(void)
{
	dumpObjectInfo();
	dumpObjectTree();
	return;
}
