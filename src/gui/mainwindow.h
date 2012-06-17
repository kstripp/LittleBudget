#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__
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

/* mainwindow.h
 * g. m. beddingfield
 * august 2005
 * 
 * much of this code was actually written by qt designer.  After Qt4
 * was released, I forked their machine-generaged code and merged it
 * into my code directly.  Why reinvent the wheel?
 *
 */

#define QT3_SUPPORT

#include <qvariant.h>
#include <qpixmap.h>
#include <q3mainwindow.h>
//Added by qt3to4:
#include <Q3ActionGroup>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <Q3PopupMenu>

class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QAction;
class Q3ActionGroup;
class Q3ToolBar;
class Q3PopupMenu;
class Q3TextBrowser;
class lb;

class mainWindow : public Q3MainWindow
{
	Q_OBJECT;

public:
	mainWindow( int argc = 0,
		    char* argv[] = 0,
		    QWidget* parent = 0,
		    const char* name = 0,
		    Qt::WFlags fl = 0 );
	~mainWindow();

protected:
	void update_output( const QStringList& args );
    
protected slots:

	virtual void languageChange();

public slots:

	void fileNew();
	void fileOpen();
	void fileSave();
	void fileSaveAs();
	void fileExit();
	void editCopy();
	void helpContents();
	void helpAbout();
	void editReport();
	void set_args(int argc, const char** argv);

protected:
	int save_buffer( const QString& filename );

private:
	// data
	lb* _lb;
	QStringList _args;
	QString _fn;

	// Widgets
	Q3TextBrowser* textBrowser;
	QMenuBar *MenuBar;
	Q3PopupMenu *fileMenu;
	Q3PopupMenu *editMenu;
	Q3PopupMenu *helpMenu;
	Q3ToolBar *toolBar;
	QAction* fileNewAction;
	QAction* fileOpenAction;
	QAction* fileSaveAction;
	QAction* fileSaveAsAction;
	QAction* fileExitAction;
	QAction* editCopyAction;
	QAction* helpContentsAction;
	QAction* helpAboutAction;
	QAction* editReportAction;
	QAction* editSelectAllAction;

	Q3VBoxLayout* lbMainWindowBaseLayout;

	QPixmap image0;
	QPixmap image1;
	QPixmap image2;
	QPixmap image3;

};

#endif // __MAINWINDOW_H__
