/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <http://www.natron.fr/>,
 * Copyright (C) 2016 INRIA and Alexandre Gauthier-Foichat
 *
 * Natron is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Natron is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Natron.  If not, see <http://www.gnu.org/licenses/gpl-2.0.html>
 * ***** END LICENSE BLOCK ***** */

// ***** BEGIN PYTHON BLOCK *****
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>
// ***** END PYTHON BLOCK *****

#include "PropertiesBinWrapper.h"

#include <stdexcept>

#include <QApplication>

#include "Gui/DockablePanel.h"
#include "Gui/Gui.h"
#include "Gui/KnobWidgetDnD.h"
#include "Gui/RightClickableWidget.h"

NATRON_NAMESPACE_ENTER;

PropertiesBinWrapper::PropertiesBinWrapper(const std::string& scriptName, Gui* parent)
    : QWidget(parent)
    , PanelWidget(scriptName, this, parent)
{
}

PropertiesBinWrapper::~PropertiesBinWrapper()
{
}

void
PropertiesBinWrapper::mousePressEvent(QMouseEvent* e)
{
    takeClickFocus();
    QWidget::mousePressEvent(e);
}

void
PropertiesBinWrapper::enterEvent(QEvent* e)
{
    if (enterEventBase()) {
        if (getGui()) {
            boost::shared_ptr<KnobWidgetDnD> currentKnobWidget = getGui()->getCurrentKnobWidgetFocus();
            if (currentKnobWidget) {
                QWidget* internalWidget = currentKnobWidget->getWidget();
                if (internalWidget) {
                    internalWidget->setFocus();
                }
            }
        }
    }
    QWidget::enterEvent(e);
}

void
PropertiesBinWrapper::leaveEvent(QEvent* e)
{
    leaveEventBase();
    QWidget::leaveEvent(e);
}

void
PropertiesBinWrapper::keyPressEvent(QKeyEvent* e)
{
    handleUnCaughtKeyPressEvent(e);
    QWidget::keyPressEvent(e);
}

void
PropertiesBinWrapper::keyReleaseEvent(QKeyEvent* e)
{
    handleUnCaughtKeyUpEvent(e);
    QWidget::keyReleaseEvent(e);
}

QUndoStack*
PropertiesBinWrapper::getUndoStack() const
{
    QWidget* w = qApp->widgetAt( QCursor::pos() );
    RightClickableWidget* panel = RightClickableWidget::isParentSettingsPanelRecursive(w);

    if (panel) {
        boost::shared_ptr<QUndoStack> stack = panel->getPanel()->getUndoStack();

        return stack.get();
    }

    return 0;
}

NATRON_NAMESPACE_EXIT;
