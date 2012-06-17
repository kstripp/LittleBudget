#ifndef __CHOOSEFILE_H__
#define __CHOOSEFILE_H__
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

/* choosefile.h
 * g. m. beddingfield
 * august 2005
 *
 * This is the dialog that the user uses to set the budget and report
 * files to collate, as well as the scales to use.
 *
 */

#define QT3_SUPPORT

#include <qdialog.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3GridLayout>
#include <Q3HBoxLayout>
#include <QLabel>

class QWidget;
class Q3VBoxLayout;
class Q3HBoxLayout;
class Q3GridLayout;
class QSpacerItem;
class QLabel;
class Q3ListBox;
class Q3ListBoxItem;
class QPushButton;
class QCheckBox;
class fileListBox;
class chooseBudgets;

class chooseFile : public QDialog
{
	Q_OBJECT

public:
	chooseFile( const QStringList& cmdln = QStringList(),
		    QWidget* parent = 0,
		    const char* name = 0,
		    bool modal = true,
		    Qt::WFlags fl = 0 );
	~chooseFile();

	static QStringList get_lb_cmds( const QStringList& result,
					QWidget* parent = 0,
					const char* name = 0,
					bool modal = true,
					Qt::WFlags fl = 0 );

public slots:

	void languageChange(void);
	void okClicked(void);
	void debugDump(void);

private slots:

	void refresh(void);

private:
	QStringList* _result; // points to external object.

	fileListBox* _budgets;
	fileListBox* _reports;
	QCheckBox*   _quiet;
	QPushButton* _okBtn;
	QPushButton* _cxBtn;
	QSpacerItem* _spacer;
	Q3VBoxLayout* _vLay;
	Q3HBoxLayout* _hLay;
	QLabel* _budgetLabel;
	QLabel* _reportLabel;
};

#endif // __CHOOSEFILE_H__
