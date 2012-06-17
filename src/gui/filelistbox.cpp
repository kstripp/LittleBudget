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


/* filelistbox.cpp
 * g. m. beddingfield
 * august 2005
 *
 * implements the custom Qt widget fileListBox
 */

#define QT3_SUPPORT

#include <cassert>
#include <iostream>

#include <qdir.h>
#include <q3filedialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <q3listbox.h>
#include <q3listview.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qwidget.h>
#include <qlineedit.h>
#include <qvalidator.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>

#include "filelistbox.h"

using namespace std;

/* fileListBox
 *
 * Here's what we're trying to create:
 *                     (optional scales)
 * +----------------------------------------------------+
 * |+-----------------------------+ +------------------+|
 * || filename1          | 1.000  | |       Add        ||
 * || filename2          | 2.100  | +------------------+|
 * || filename3          |  .980  | +------------------+|
 * ||                    |        | |      Remove      ||
 * ||                    |        | +------------------+|
 * ||                    |        | +------------------+|
 * ||                    |        | |     Move Up      ||
 * ||                    |        | +------------------+|
 * ||                    |        | +------------------+|
 * ||                    |        | |    Move Down     ||
 * |+-----------------------------+ +------------------+|
 * +----------------------------------------------------+
 *
 * This is designed to be reusable.
 *
 */

fileListBox::fileListBox( QWidget* parent,
			  const char* name,
			  bool use_scales ) :
	QWidget(parent,name),
	_scales(use_scales)
{
	// Lay out the widget:

	// Create base layout
	_layout = new Q3HBoxLayout(this,0,5,"_layout");

	// Create listbox setup
	_box = new Q3ListView(this);
	if (name) {
		_box->addColumn( name );
	} else {
		_box->addColumn("File List");
	}
	_box->addColumn("Scales");
	if (!_scales) {
		_box->hideColumn(1);
		_box->setColumnWidthMode( 1, Q3ListView::Manual );
	}
	_box->addColumn("Order");
	_box->setSorting( 2, true );
	_box->setColumnWidthMode( 2, Q3ListView::Manual );
	_box->hideColumn(2);
	_layout->addWidget(_box);

	// Create buttons
	_buttonLay = new Q3VBoxLayout(0,5,-1,"_buttonLay");
	_addBtn = new QPushButton(this,"_addBtn");
	_buttonLay->addWidget(_addBtn);
	_deleteBtn = new QPushButton(this,"_deleteBtn");
	_buttonLay->addWidget(_deleteBtn);
	_upBtn = new QPushButton(this,"_upBtn");
	_buttonLay->addWidget(_upBtn);
	_downBtn = new QPushButton(this,"_downBtn");
	_buttonLay->addWidget(_downBtn);
	_layout->addLayout(_buttonLay);

	// labels and stuff are all set in
	// languageChange()
	languageChange();

	// connect the signals and slots
	connect( _addBtn, SIGNAL( clicked() ),
		 this, SLOT( openFile() ) );
	connect( _deleteBtn, SIGNAL( clicked() ),
		 this, SLOT( deleteFile() ) );
	connect( _upBtn, SIGNAL( clicked() ),
		 this, SLOT( moveUp() ) );
	connect( _downBtn, SIGNAL( clicked() ),
		 this, SLOT( moveDown() ) );
	connect( _box, SIGNAL(doubleClicked( Q3ListViewItem*, const QPoint&, int)),
		 this, SLOT( changeScale( Q3ListViewItem*, const QPoint&, int ) ));


	return;
}

fileListBox::~fileListBox()
{ }

void fileListBox::setList(const QStringList& flist, const QStringList& scales )
{
	assert( flist.size() == scales.size() );
	clear();
	for( unsigned int i=0 ; i<flist.size() ; ++i ) {
		(void) new fileListItem( _box,
					 flist[i],
					 scales[i] );
	}
	set_pwd( QDir::currentDirPath() );
	return;
}

void fileListBox::setList(const QStringList& l)
{
	QStringList scales;
	for( unsigned int i = 0 ; i<l.size() ; ++i ) {
		scales.push_back( tr("1.000") );
	}
	setList( l, scales );
}

QStringList fileListBox::getList(void)
{
	QStringList s;
	Q3ListViewItemIterator i( _box );
	while( i.current() ) {
		s.push_back( reinterpret_cast<fileListItem*>(*i)->fullName() );
		++i;
	}
	return s;
}

QStringList fileListBox::getScales(void)
{
	QStringList s;
	Q3ListViewItemIterator i( _box );
	while( i.current() ) {
		s.push_back( (*i)->text( 1 ) );
		++i;
	}
	return s;
}

void fileListBox::languageChange(void)
{

	_addBtn->setText( tr("Add") );
	_deleteBtn->setText( tr("Remove") );
	_upBtn->setText( tr("Move Up") );
	_downBtn->setText( tr("Move Down") );
}

void fileListBox::openFile(void)
{
	QString fn = Q3FileDialog::getOpenFileName();
	if( !fn.isEmpty() ) {
		emit added();
		(void) new fileListItem( _box,
					 fn,
					 1.0 );
		set_pwd( QDir::currentDirPath() );
	}
	return;
}

void fileListBox::deleteFile(void)
{
	fileListItem* i = reinterpret_cast<fileListItem*>( _box->currentItem() );
//	emit deleting(i);
	delete i;
	return;
}

void fileListBox::clear(void)
{
	emit clearing();
	_box->clear();
	return;
}

void fileListBox::moveUp(void)
{
	Q3ListViewItem* cur = _box->currentItem();
	Q3ListViewItem* prev = cur->itemAbove();
	if( cur && prev) {
// 		emit movingUp(cur);
		QString tmp = prev->text(2);
		prev->setText( 2, cur->text(2) );
		cur->setText( 2, tmp );
	}
	_box->sort();
	return;
}

void fileListBox::moveDown(void)
{
	Q3ListViewItem* cur = _box->currentItem();
	Q3ListViewItem* next = cur->itemBelow();
	if( cur && next) {
// 		emit movingDown(cur);
		QString tmp = next->text(2);
		next->setText( 2, cur->text(2) );
		cur->setText( 2, tmp );
	}
	_box->sort();
	return;
}

/* getDouble()
 *
 * I originally used QInputDialog::getDouble(), but it gave odd
 * operation for the user if the user exceeded the number of decimals.
 * Instead of just living with it (and rounding), they disabled the
 * 'OK' button without explaination.  This is improved in Qt4 where
 * it's done with a spin box.
 *
 * This code here is basically stealing from the Qt3 implementation
 * and customizing it to what I need.
 */
static double getDouble( QWidget* parent,
			 const QString& title,
			 const QString& label,
			 double value = 0.0,
			 double minValue = 0.0,
			 double maxValue = 2147483647.0,
			 int decimals = 1,
			 bool* ok = 0 )
{
	QDialog dlg( parent );
	dlg.setCaption( title );

	// Build it with a vertical layout
	Q3VBoxLayout* vbl = new Q3VBoxLayout( &dlg );

	QLabel* lbl = new QLabel( &dlg );
	lbl->setText( label );
	vbl->addWidget( lbl );

	QLineEdit* led = new QLineEdit( &dlg );
	led->setValidator( new QDoubleValidator( minValue, maxValue, 100, led ) );
	led->setText( QString::number( value, 'f', decimals ) );
	vbl->addWidget( led );

	Q3HBoxLayout* hbl = new Q3HBoxLayout;
	vbl->addLayout( hbl );

	hbl->addStretch();

	QPushButton* okBtn = new QPushButton( &dlg );
	okBtn->setText( "OK" );
	hbl->addWidget( okBtn );

	QPushButton* cxBtn = new QPushButton( &dlg );
	cxBtn->setText( "Cancel" );
	hbl->addWidget( cxBtn );

	dlg.connect( okBtn, SIGNAL(clicked()), &dlg, SLOT(accept()) );
	dlg.connect( cxBtn, SIGNAL(clicked()), &dlg, SLOT(reject()) );

	bool accepted = ( QDialog::Accepted == dlg.exec() );
	if( ok )
		*ok = accepted;

	return led->text().toDouble();
}

void fileListBox::changeScale( Q3ListViewItem* i, const QPoint&, int )
{
	if( !_scales ) return;
	if( 0 == i ) return;

	double d = 1.0;
	bool ok;
	d = i->text(1).toDouble();
	d = getDouble( this, 
		       tr("Scale"),
		       tr("Enter value to multiply budget:"),
		       d,
		       0.0,
		       2147483647,
		       3,
		       &ok );
	if( ok ) {
		QString s;
		s.setNum( d, 'f', 3 );
		i->setText(1,s);
	}
	return;
}

const QString& fileListBox::set_pwd( const QString& pwd )
{
	Q3ListViewItemIterator i( _box );
	while( i.current() ) {
		reinterpret_cast<fileListItem*>(*i)->set_pwd( pwd );
		++i;
	}
	return _pwd = pwd;
}

// fileListItem definition

fileListItem::fileListItem( Q3ListView* parent,
			    const QString& fn,
			    double scale ) :
	Q3ListViewItem( parent,
		       QString(),
		       QString::number( scale, 'f', 3 ),
		       QString::number( parent->childCount() )),
	_fn( fn )
{
	setText( 0, filenameAbbrev() );
}

fileListItem::fileListItem( Q3ListView* parent,
			    const QString& fn,
			    const QString& scale ) :
	Q3ListViewItem( parent,
		       QString(),
		       QString::number( scale.toDouble(), 'f', 3 ),
		       QString::number( parent->childCount() )),
	_fn( fn )
{
	setText( 0, filenameAbbrev() );
}

fileListItem::~fileListItem()
{

}

static void strip_to_first_not_matching( QString& pwd, QString& fn )
{
	unsigned int n, nb;
	n  = pwd.length();
	nb = fn.length();
	n = ( n < nb ) ? n : nb;

	for( nb=0 ; nb < n ; ++nb ) {
		if( pwd[nb] != fn[nb] ) break;
	}

	nb = 1 + fn.findRev( QChar('/'), nb );
	assert( nb >= 0 );
	pwd.remove( 0, nb );
	fn.remove( 0, nb );

	return;
}

QString fileListItem::filenameAbbrev( const QString& ppwd )
{
	QString pwd = ppwd;
	QString fn = _fn;

	if( (fn[0]!='/') && (fn[1]!=':') ) {
		return fn;
	}

	strip_to_first_not_matching( pwd, fn );

	// On windows, if the files are on a drive different from the
	// one we're on, then we have to find a way to filter that
	// out.
	if( -1 != fn.find( QChar(':') )) return fn;  // different drives.

	int i = 0;
	if( 0 != pwd.length() ) fn = QString("../") + fn;
	while( (i>=0) && (i<(int)(pwd.length())) ) {
		assert( i >= 0 );
		i = pwd.find( QChar('/'), i );
		if( -1 == i ) break;
		fn = QString("../") + fn;
		if( i > 0 ) ++i;
	}

	return fn;
}

void fileListItem::set_pwd( const QString& pwd )
{
	setText( 0, filenameAbbrev(pwd) );
}
