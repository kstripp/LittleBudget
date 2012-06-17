#ifndef __FILELISTBOX_H__
#define __FILELISTBOX_H__
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

/* filelistbox.h
 * g. m. beddingfield
 * august 2005
 *
 * Defines a custom QT widget that implements a QListBox
 * with a list of file names, and 4 buttons to manipulate
 * the list.
 *
 */

#define QT3_SUPPORT

#include <qwidget.h>
#include <q3listview.h>
#include <qdir.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QLabel>

//class QWidget;
class QPushButton;
class QLabel;
class Q3VBoxLayout;
class Q3HBoxLayout;
class QString;

class fileListItem : public Q3ListViewItem
{
private:
	QString _fn; // filename, with full path

public:
	fileListItem( Q3ListView* parent,
		      const QString& fn,
		      double scale );
	fileListItem( Q3ListView* parent,
		      const QString& fn,
		      const QString& scale );
	virtual ~fileListItem();

	QString filenameAbbrev( const QString& ppwd = QDir::currentDirPath() );
	void set_pwd( const QString& pwd );
	inline const QString& fullName( void );
	inline const QString& setFullName( const QString& fn );
};

inline const QString& fileListItem::fullName( void )
{
	return _fn;
}

inline const QString& fileListItem::setFullName( const QString& fn )
{
	return _fn = fn;
}

class fileListBox : public QWidget
{
	Q_OBJECT
public:
	fileListBox( QWidget* parent = 0,
		     const char* name = 0,
		     bool usescales = false );
	virtual ~fileListBox();

	void setList( const QStringList& l );
	void setList( const QStringList& l, const QStringList& s );
	QStringList getList( void );
	QStringList getScales( void );
	inline const QString& pwd( void ) const;
	const QString& set_pwd( const QString& pwd );

signals:

	void changed( void );
	void movingUp( int );
	void movingDown( int );
	void added( void );
	void clearing( void );
//	void deleting( int );

private slots:
	void openFile( void );
	void deleteFile( void );
	void moveUp( void );
	void moveDown( void );
	void changeScale( Q3ListViewItem*, const QPoint&, int );

public slots:
	void languageChange( void );
	void clear( void );

private:
	// data:
	QString _pwd;
	bool    _scales;

	// widgets:
	Q3ListView* _box;
	QPushButton* _addBtn;
	QPushButton* _deleteBtn;
	QPushButton* _upBtn;
	QPushButton* _downBtn;

	// layouts:
	Q3VBoxLayout* _buttonLay;
	Q3HBoxLayout* _layout;

};

inline const QString& fileListBox::pwd( void ) const
{
	return _pwd;
}

#endif // __FILELISTBOX_H__
