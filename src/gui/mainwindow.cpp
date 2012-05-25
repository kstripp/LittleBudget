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

/* mainwindow.cpp
 * g. m. beddingfield
 * august 2005
 *
 * This defines the mainWindow class, which is the main view
 * for the lbgui application.
 *
 * The code was mostly created by the uic compiler, then I
 * merged it with my implementation.  This was done because
 * Qt3 .ui files are not compatible with Qt4 .ui files.
 *
 */

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cassert>

#include <qaction.h>
#include <qaction.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <qfont.h>
#include <qimage.h>
#include <qlayout.h>
#include <qmenubar.h>
#include <qmessagebox.h>
#include <qpixmap.h>
#include <qpopupmenu.h>
#include <qstring.h>
#include <qtextbrowser.h>
#include <qtoolbar.h>
#include <qtooltip.h>
#include <qvariant.h>
#include <qwhatsthis.h>

#include "../core/lb.h"
#include "mainwindow.h"
#include "choosefile.h"

using namespace std;

static const unsigned char image0_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x16,
    0x08, 0x06, 0x00, 0x00, 0x00, 0xc4, 0xb4, 0x6c, 0x3b, 0x00, 0x00, 0x00,
    0x85, 0x49, 0x44, 0x41, 0x54, 0x38, 0x8d, 0x63, 0x60, 0xa0, 0x11, 0x60,
    0xc4, 0x22, 0xf6, 0x9f, 0x02, 0xbd, 0x70, 0xc0, 0x84, 0x4d, 0xf0, 0xff,
    0xff, 0xff, 0x38, 0x31, 0x03, 0x03, 0x03, 0x83, 0xa6, 0x8e, 0x26, 0x41,
    0x07, 0x60, 0x35, 0x98, 0x10, 0x30, 0x34, 0x34, 0x24, 0x68, 0x38, 0x59,
    0x06, 0x6b, 0x6a, 0x68, 0x32, 0x54, 0x87, 0x56, 0xe3, 0x35, 0x9c, 0x2c,
    0x83, 0x6b, 0x12, 0x6b, 0x18, 0xee, 0xb3, 0xdc, 0x27, 0xca, 0xe5, 0xc8,
    0xe0, 0x3f, 0x3e, 0x00, 0x97, 0x7f, 0xf6, 0xff, 0x7f, 0x73, 0x6b, 0xf3,
    0xff, 0x25, 0x8d, 0x4b, 0xfe, 0x6b, 0xea, 0x68, 0xfe, 0x47, 0x37, 0x9c,
    0x85, 0x1c, 0x17, 0x33, 0x32, 0xe2, 0x4d, 0x10, 0xe4, 0x19, 0x0c, 0x4b,
    0x19, 0x84, 0x2c, 0x23, 0x2b, 0x8c, 0x89, 0x01, 0xa3, 0x06, 0x8f, 0x1a,
    0x3c, 0x6a, 0x30, 0x1e, 0x40, 0x49, 0x0d, 0x42, 0xac, 0x79, 0x43, 0x04,
    0x00, 0x00, 0x5a, 0x31, 0x53, 0x9e, 0x97, 0xce, 0x3c, 0xd8, 0x00, 0x00,
    0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

static const unsigned char image1_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x16,
    0x08, 0x06, 0x00, 0x00, 0x00, 0xc4, 0xb4, 0x6c, 0x3b, 0x00, 0x00, 0x00,
    0xae, 0x49, 0x44, 0x41, 0x54, 0x38, 0x8d, 0xed, 0x94, 0x4b, 0x0e, 0xc3,
    0x20, 0x0c, 0x44, 0x9f, 0x51, 0x8e, 0x92, 0xd3, 0xe4, 0x4e, 0x55, 0x55,
    0xe5, 0x4e, 0xb9, 0x9e, 0xbb, 0x00, 0x5a, 0x08, 0x84, 0x02, 0x72, 0x77,
    0x99, 0x8d, 0xf9, 0xc4, 0xcf, 0xc3, 0x2c, 0x02, 0xb7, 0x82, 0x64, 0xa2,
    0x47, 0x7f, 0x30, 0x74, 0x86, 0x7b, 0x86, 0xc6, 0x33, 0x3d, 0xaf, 0x67,
    0x1c, 0xf7, 0x0c, 0x15, 0xf7, 0x07, 0x28, 0x00, 0xcb, 0xc5, 0xc5, 0xc8,
    0x4b, 0x6a, 0xf1, 0x20, 0x80, 0xaa, 0xc6, 0x7b, 0x01, 0x14, 0x91, 0x6e,
    0xee, 0xe5, 0x87, 0xcb, 0x77, 0xa8, 0x7c, 0xaa, 0x9f, 0x24, 0xc5, 0xf9,
    0xc8, 0x70, 0x97, 0x18, 0x1f, 0xac, 0x6d, 0xb9, 0x9a, 0xe3, 0xbe, 0xda,
    0x56, 0x35, 0xe3, 0x89, 0xac, 0x0b, 0x55, 0x33, 0x4e, 0xa1, 0xfb, 0x0b,
    0x36, 0xe0, 0x48, 0x9a, 0x5a, 0xfb, 0xc7, 0xd3, 0xd7, 0x6a, 0xc6, 0x19,
    0x74, 0xf5, 0x4d, 0xdb, 0x1a, 0x20, 0x8d, 0x7d, 0x84, 0x02, 0x12, 0xa2,
    0xb0, 0x73, 0x1a, 0x40, 0xb9, 0x63, 0x0b, 0xa7, 0xe9, 0x42, 0x55, 0xed,
    0x9c, 0x46, 0x39, 0x1f, 0x88, 0x9d, 0xd3, 0xcc, 0xb1, 0xa5, 0xd3, 0x02,
    0x3c, 0x29, 0xcb, 0xdf, 0x6e, 0x9f, 0xde, 0xbd, 0x46, 0x65, 0x49, 0xca,
    0x46, 0xef, 0xe4, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae,
    0x42, 0x60, 0x82
};

static const unsigned char image2_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x16,
    0x08, 0x06, 0x00, 0x00, 0x00, 0xc4, 0xb4, 0x6c, 0x3b, 0x00, 0x00, 0x01,
    0x2f, 0x49, 0x44, 0x41, 0x54, 0x38, 0x8d, 0xb5, 0x95, 0x4d, 0x6e, 0x83,
    0x30, 0x10, 0x85, 0x3f, 0xaa, 0x1e, 0x22, 0x07, 0x60, 0x95, 0x55, 0x95,
    0x65, 0x22, 0xf5, 0x00, 0xdc, 0x29, 0xaa, 0x10, 0x67, 0xe1, 0x0a, 0x1c,
    0x00, 0xc9, 0xec, 0x88, 0xba, 0xca, 0xa2, 0x62, 0x8d, 0x72, 0x8b, 0xc9,
    0x02, 0x6c, 0xc6, 0xfc, 0x04, 0x93, 0xa4, 0x4f, 0xb2, 0x3c, 0x4f, 0x30,
    0x4f, 0xc3, 0xf3, 0x30, 0x86, 0x7f, 0x42, 0xd4, 0xef, 0xf2, 0x6e, 0xdd,
    0x0f, 0x1b, 0xd5, 0x39, 0x64, 0x69, 0x17, 0x9b, 0xd2, 0x20, 0xad, 0x20,
    0xa5, 0x20, 0xad, 0xac, 0x72, 0xc0, 0x71, 0x0b, 0x27, 0x5c, 0x34, 0x90,
    0xf4, 0xf1, 0x31, 0x3e, 0x52, 0x35, 0x15, 0xc4, 0x50, 0x35, 0xd5, 0x2a,
    0x07, 0x1c, 0x1f, 0x43, 0xea, 0x1c, 0xc9, 0x52, 0x04, 0x10, 0x29, 0x45,
    0xa4, 0x15, 0x31, 0xa5, 0x11, 0x69, 0x65, 0x9e, 0xff, 0x0d, 0x9c, 0xce,
    0x4a, 0xbd, 0xfc, 0x8a, 0x1d, 0x54, 0x65, 0x97, 0xdf, 0x2b, 0xec, 0x66,
    0x2a, 0xbd, 0x29, 0x4e, 0x67, 0xa3, 0xb5, 0xd2, 0x13, 0x4e, 0xd4, 0x67,
    0x58, 0x11, 0x1a, 0x38, 0xc4, 0x7b, 0x2e, 0xcd, 0xd5, 0x71, 0x2b, 0xa6,
    0xf9, 0x38, 0x3f, 0xcc, 0xe3, 0x5b, 0xc5, 0xe1, 0x6b, 0xbf, 0xea, 0xb1,
    0xce, 0x7f, 0xcd, 0xe3, 0xd6, 0xf7, 0x58, 0xe7, 0x07, 0x79, 0x1c, 0xd4,
    0x1d, 0xe3, 0xfc, 0x10, 0x8f, 0xe7, 0x3c, 0x0d, 0xf1, 0xf8, 0x73, 0xe2,
    0x71, 0x0a, 0xa7, 0xef, 0xd3, 0x8c, 0x5b, 0xcb, 0xc8, 0xd2, 0x21, 0xff,
    0x3c, 0x16, 0x4e, 0xe2, 0xfe, 0x61, 0xdc, 0x89, 0x6b, 0xbe, 0x65, 0xb7,
    0x70, 0xb3, 0x42, 0xf7, 0x60, 0x02, 0x14, 0x3c, 0xc7, 0xcf, 0x3f, 0x9e,
    0xee, 0x70, 0xaa, 0x75, 0x3e, 0xf9, 0x8b, 0x82, 0xd6, 0xc3, 0xae, 0x48,
    0x98, 0x9e, 0xee, 0x12, 0xec, 0x20, 0x32, 0xa5, 0xf1, 0xf2, 0x2d, 0xbc,
    0xae, 0x28, 0x98, 0x9e, 0xee, 0x12, 0xbc, 0xee, 0x50, 0xf9, 0x13, 0x61,
    0x5b, 0x69, 0x68, 0xc5, 0x9b, 0xfb, 0x78, 0x4b, 0xc5, 0x9b, 0x66, 0xc5,
    0x96, 0x8a, 0x1f, 0xcd, 0x8a, 0x48, 0xc5, 0xef, 0xba, 0x9e, 0xa2, 0xf5,
    0x57, 0x5e, 0xc0, 0x1d, 0xd6, 0x22, 0x6c, 0x46, 0x81, 0xd0, 0xcf, 0x5f,
    0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

static const unsigned char image3_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x16,
    0x08, 0x06, 0x00, 0x00, 0x00, 0xc4, 0xb4, 0x6c, 0x3b, 0x00, 0x00, 0x00,
    0xd2, 0x49, 0x44, 0x41, 0x54, 0x38, 0x8d, 0xd5, 0x95, 0x41, 0x0e, 0x82,
    0x30, 0x10, 0x45, 0xdf, 0x18, 0x17, 0x9e, 0x47, 0x57, 0x26, 0x9c, 0x80,
    0xc4, 0x1b, 0xe9, 0xaa, 0x6e, 0x88, 0xf7, 0xe9, 0x2d, 0xf0, 0x4a, 0xdf,
    0x45, 0x85, 0x80, 0xb6, 0xb4, 0x14, 0x37, 0xfe, 0xa4, 0x99, 0x94, 0x19,
    0x7e, 0xdf, 0xb4, 0xa4, 0xc0, 0xbf, 0xc9, 0x00, 0x15, 0xd4, 0xd4, 0x19,
    0x4b, 0x71, 0x6f, 0x33, 0x9b, 0xd6, 0xad, 0xd2, 0x3e, 0x62, 0x02, 0x61,
    0xb5, 0x31, 0x9a, 0x99, 0xd6, 0x9a, 0x8f, 0xc6, 0x29, 0xea, 0x5a, 0xf3,
    0x2c, 0xb1, 0xd9, 0x13, 0xf0, 0x40, 0x0f, 0x78, 0x41, 0xfb, 0x9e, 0x0f,
    0xf1, 0x96, 0x5c, 0x4c, 0x29, 0x01, 0x02, 0xb7, 0x90, 0xef, 0x05, 0x2e,
    0xd9, 0xea, 0xc4, 0x64, 0x3e, 0x86, 0x97, 0x63, 0xf9, 0xf0, 0xcc, 0xc9,
    0x35, 0x8a, 0x9a, 0xef, 0xa6, 0xad, 0x7f, 0x8e, 0x20, 0x1f, 0xcd, 0x0f,
    0x3a, 0x5c, 0xe0, 0xdc, 0x5c, 0x89, 0x99, 0x6f, 0x22, 0x96, 0xa4, 0x47,
    0xa7, 0x2f, 0xf2, 0x82, 0xaf, 0xc2, 0x03, 0xc7, 0x44, 0xbe, 0xc5, 0xec,
    0x3e, 0x9b, 0x87, 0x85, 0xc3, 0x81, 0x56, 0x13, 0xa7, 0x0f, 0xf4, 0x07,
    0x7b, 0x1c, 0xef, 0x70, 0xf4, 0x5c, 0x4f, 0xbc, 0x54, 0xbf, 0x89, 0x38,
    0x57, 0x4f, 0x9e, 0xa4, 0x9e, 0xd8, 0x96, 0x47, 0x3d, 0x71, 0x46, 0x1b,
    0xf7, 0x38, 0xad, 0x3a, 0xe2, 0x82, 0x6b, 0xb0, 0xd7, 0xfc, 0x36, 0x2b,
    0x89, 0xa7, 0xaa, 0xbf, 0x4e, 0x91, 0x5e, 0x03, 0xde, 0xd2, 0x44, 0xe0,
    0xd9, 0xd3, 0xa4, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae,
    0x42, 0x60, 0x82
};

mainWindow::mainWindow( int argc,
			char* argv[],
			QWidget* parent,
			const char* name,
			WFlags fl )
    : QMainWindow( parent, name, fl )
{
	(void)statusBar();
	QImage img;
	img.loadFromData( image0_data, sizeof( image0_data ), "PNG" );
	image0 = img;
	img.loadFromData( image1_data, sizeof( image1_data ), "PNG" );
	image1 = img;
	img.loadFromData( image2_data, sizeof( image2_data ), "PNG" );
	image2 = img;
	img.loadFromData( image3_data, sizeof( image3_data ), "PNG" );
	image3 = img;
	if ( !name )
		setName( "lbMainWindowBase" );
	setCentralWidget( new QWidget( this, "qt_central_widget" ) );
	lbMainWindowBaseLayout = new QVBoxLayout( centralWidget(), 11, 6, "lbMainWindowBaseLayout"); 

	textBrowser = new QTextBrowser( centralWidget(), "textBrowser" );
	lbMainWindowBaseLayout->addWidget( textBrowser );

	// actions
	fileNewAction = new QAction( this, "fileNewAction" );
	fileNewAction->setIconSet( QIconSet( image0 ) );
	fileOpenAction = new QAction( this, "fileOpenAction" );
	fileOpenAction->setIconSet( QIconSet( image1 ) );
	fileSaveAction = new QAction( this, "fileSaveAction" );
	fileSaveAction->setIconSet( QIconSet( image2 ) );
	fileSaveAsAction = new QAction( this, "fileSaveAsAction" );
	fileExitAction = new QAction( this, "fileExitAction" );
	editCopyAction = new QAction( this, "editCopyAction" );
	editCopyAction->setIconSet( QIconSet( image3 ) );
	helpContentsAction = new QAction( this, "helpContentsAction" );
	helpAboutAction = new QAction( this, "helpAboutAction" );
	editReportAction = new QAction( this, "editReportAction" );
	editSelectAllAction = new QAction( this, "editSelectAllAction" );


	// toolbars
	toolBar = new QToolBar( QString(""), this, DockTop ); 

	fileNewAction->addTo( toolBar );
	fileOpenAction->addTo( toolBar );
	fileSaveAction->addTo( toolBar );
	toolBar->addSeparator();
	toolBar->addSeparator();
	editCopyAction->addTo( toolBar );


	// menubar
	MenuBar = new QMenuBar( this, "MenuBar" );


	fileMenu = new QPopupMenu( this );
	fileNewAction->addTo( fileMenu );
	fileOpenAction->addTo( fileMenu );
	fileSaveAction->addTo( fileMenu );
	fileSaveAsAction->addTo( fileMenu );
	fileExitAction->addTo( fileMenu );
	MenuBar->insertItem( QString(""), fileMenu, 1 );

	editMenu = new QPopupMenu( this );
	editSelectAllAction->addTo( editMenu );
	editReportAction->addTo( editMenu );
	editMenu->insertSeparator();
	editCopyAction->addTo( editMenu );
	editMenu->insertSeparator();
	MenuBar->insertItem( QString(""), editMenu, 2 );

	helpMenu = new QPopupMenu( this );
	helpContentsAction->addTo( helpMenu );
	helpMenu->insertSeparator();
	helpAboutAction->addTo( helpMenu );
	MenuBar->insertItem( QString(""), helpMenu, 3 );

	languageChange();
	resize( QSize(737, 447).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );

	// signals and slots connections
	connect( fileNewAction, SIGNAL( activated() ), this, SLOT( fileNew() ) );
	connect( fileOpenAction, SIGNAL( activated() ), this, SLOT( fileOpen() ) );
	connect( fileSaveAction, SIGNAL( activated() ), this, SLOT( fileSave() ) );
	connect( fileSaveAsAction, SIGNAL( activated() ), this, SLOT( fileSaveAs() ) );
	connect( fileExitAction, SIGNAL( activated() ), this, SLOT( fileExit() ) );
	connect( editCopyAction, SIGNAL( activated() ), this, SLOT( editCopy() ) );
	connect( helpContentsAction, SIGNAL( activated() ), this, SLOT( helpContents() ) );
	connect( helpAboutAction, SIGNAL( activated() ), this, SLOT( helpAbout() ) );
	connect( editReportAction, SIGNAL( activated() ), this, SLOT( editReport() ) );

	// my custom stuff:
	textBrowser->setTextFormat( Qt::PlainText );
	QFont font("Courier");
	font.setStyleHint(QFont::TypeWriter);
	textBrowser->setFont(font);
	textBrowser->setWordWrap(QTextEdit::NoWrap);
	set_args(argc,(const char**)argv);
	_lb = new lb(argc,(const char**)argv);
	ostringstream os;
	_lb->run(os);
	QString txt(os.str());
	textBrowser->setText(txt);

	connect( editSelectAllAction, SIGNAL( activated() ),
		 textBrowser, SLOT( selectAll() ) );

}

void mainWindow::set_args( int argc,const char** argv )
{
	QStringList sl;
	int i;
	for( i = 0 ; i < argc ; ++i )
		sl.push_back( argv[i] );
	_args = sl;
	return;
}

/*  
 *  Destroys the object and frees any allocated resources
 */
mainWindow::~mainWindow()
{
	delete _lb;
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void mainWindow::languageChange()
{
	if( _fn.isEmpty() )
		setCaption( tr( "lb - The Little Budget Tool" ) );
	else
		setCaption( tr( "lb - The Little Budget Tool: " + _fn ) );
	textBrowser->setSource( QString::null );
	fileNewAction->setText( tr( "New" ) );
	fileNewAction->setMenuText( tr( "&New" ) );
	fileNewAction->setAccel( tr( "Ctrl+N" ) );
	fileOpenAction->setText( tr( "Open" ) );
	fileOpenAction->setMenuText( tr( "&Open..." ) );
	fileOpenAction->setAccel( tr( "Ctrl+O" ) );
	fileSaveAction->setText( tr( "Save" ) );
	fileSaveAction->setMenuText( tr( "&Save" ) );
	fileSaveAction->setAccel( tr( "Ctrl+S" ) );
	fileSaveAsAction->setText( tr( "Save As" ) );
	fileSaveAsAction->setMenuText( tr( "Save &As..." ) );
	fileSaveAsAction->setAccel( QString::null );
	fileExitAction->setText( tr( "Exit" ) );
	fileExitAction->setMenuText( tr( "E&xit" ) );
	fileExitAction->setAccel( QString::null );
	editCopyAction->setText( tr( "Copy" ) );
	editCopyAction->setMenuText( tr( "&Copy" ) );
	editCopyAction->setAccel( tr( "Ctrl+C" ) );
	helpContentsAction->setText( tr( "Contents" ) );
	helpContentsAction->setMenuText( tr( "&Contents..." ) );
	helpContentsAction->setAccel( QString::null );
	helpAboutAction->setText( tr( "About" ) );
	helpAboutAction->setMenuText( tr( "&About" ) );
	helpAboutAction->setAccel( QString::null );
	editReportAction->setText( tr( "Edit Report" ) );
	editSelectAllAction->setText( tr( "Select All" ) );
	toolBar->setLabel( tr( "Tools" ) );
	if (MenuBar->findItem(1))
		MenuBar->findItem(1)->setText( tr( "&File" ) );
	if (MenuBar->findItem(2))
		MenuBar->findItem(2)->setText( tr( "&Edit" ) );
	if (MenuBar->findItem(3))
		MenuBar->findItem(3)->setText( tr( "&Help" ) );
}

/*
 * public slot
 */
void mainWindow::fileNew()
{
	QStringList a,b;
	b = chooseFile::get_lb_cmds( a );
	if( a != b ) {
		_args = b;
		update_output( _args );
	}

	return;
}

/*
 * public slot
 */
void mainWindow::fileOpen()
{
	QStringList b;
	b = chooseFile::get_lb_cmds( _args );
	if( _args != b ) {
		_args = b;
		update_output( _args );
	}
	return;
}

/*
 * public slot
 */
void mainWindow::fileSave()
{
	if( _fn.isEmpty() )
		fileSaveAs();
	else
		save_buffer( _fn );

	return;
}

/*
 * public slot
 */
void mainWindow::fileSaveAs()
{
	QString fn = QFileDialog::getSaveFileName( QString::null,
						   QString::null,
						   this,
						   0,
						   "Save output to file...");
	if( fn.isEmpty() ) return;

	if( save_buffer( fn ) ) {
		_fn = fn;
		languageChange();
	}

	return;
}

int mainWindow::save_buffer( const QString& fn )
{
	int rv = 0;
	assert( !fn.isEmpty() );
	QFile fp(fn);
	if( fp.open( IO_WriteOnly ) ) {
		QTextStream str( &fp );
		str << textBrowser->text() << "\n";
		fp.close();
		rv = 1;
	} else {
		QMessageBox::critical(
			this,
			"Cannot save file",
			fp.errorString(),
			QMessageBox::Ok,
			0 );
		rv = 0;
	}
	
	return rv;
}

/*
 * Public Slot
 */
void mainWindow::fileExit()
{
	QMainWindow::close();
	return;
}

/*
 * public slot
 */
void mainWindow::editCopy()
{
	textBrowser->copy();
	return;
}

/* helpContents()
 *
 * Desire:  Show the man page.
 * Short-term solution, show --help without losing the actual args.
 * 
 */
void mainWindow::helpContents()
{
	lb tmp;
	ostringstream os;
	tmp.run(os);
	textBrowser->setText( QString(os.str()) );
}

/*
 * public slot
 */
void mainWindow::helpAbout()
{
	QMessageBox::about( this, "The Little Budget Tool",
			    "LB - The Little Budget Utility for GnuCash\n"
			    "\n"
			    "Copyright (C) 2004 by Gabriel M. Beddingfield\n"
			    "gabriel@teuton.org\n"
			    "\n"
			    "This program is free software; you can redistribute it and/or modify\n"
			    "it under the terms of the GNU General Public License as published by\n"
			    "the Free Software Foundation; either version 2 of the License, or\n"
			    "(at your option) any later version.\n"
			    "\n"
			    "This program is distributed in the hope that it will be useful,\n"
			    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
			    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
			    "GNU General Public License for more details.\n"
			    "\n"
			    "You should have received a copy of the GNU General Public License\n"
			    "along with this program; if not, write to the\n"
			    "Free Software Foundation, Inc.,\n"
			    "59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.\n");
}

/*
 * public slot
 */
void mainWindow::editReport()
{
	_args = chooseFile::get_lb_cmds( _args );
	update_output( _args );

	return;
}

void mainWindow::update_output( const QStringList& args )
{
	// Translate args into argc/argv style

	int argc;
	argc = args.size();

	char* argv[argc];
	char** j;

	QStringList::const_iterator i;
	for( i = args.begin(), j = argv ; 
	     i != args.end(), j != &argv[argc] ;
	     ++i, ++j ) {
		*j = new char[(*i).length()+1];
		strcpy( (*j), (*i).ascii() );
	}

	ostringstream os;
	_lb->run(argc,(const char**)argv,os);
	textBrowser->setText( QString(os.str()) );

	for( j = argv ; j != &argv[argc] ; ++j )
		delete [] (*j);

	return;
}