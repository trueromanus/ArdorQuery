/*
    ArdorQuery http tester
    Copyright (C) 2022 Roman Vladimirov
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "backendviewmodel.h"

BackendViewModel::BackendViewModel(QObject *parent)
    : QObject{parent}
{
    addNewRequest("New Query");
}

void BackendViewModel::addNewRequest(const QString &name)
{
    auto model = new HttpRequestModel(this);
    model->setTitle(name);

    auto request = model->requestModel();
    request->setTextAdvisor(m_textAdviser);

    m_requests->addItem(model);
}

bool BackendViewModel::keysHandler(int key, quint32 nativeCode, bool control, bool shift, bool alt) noexcept
{
    auto request = m_requests->selectedItem()->requestModel();

    // Ctrl-Enter
    if ((key == Qt::Key_Enter || key == Qt::Key_Return) && control) {
        request->addItem(request->selectedItem() + 1);
        return true;
    }

    // Alt-Enter
    if ((key == Qt::Key_Enter || key == Qt::Key_Return) && alt) {
        if (request->selectedItem() > 0) request->addItem(request->selectedItem() - 1);
        return true;
    }

    // Shift-Enter
    if ((key == Qt::Key_Enter || key == Qt::Key_Return) && alt) {
        request->addItem(-1);
        return true;
    }

    // Ctrl-S or F5
    if ((nativeCode == 31 && control) || nativeCode == 63) {
        request->addItem(-1);
        return true;
    }

    // Ctrl-L or F3
    if ((nativeCode == 38 && control) || nativeCode == 61) {
        m_requestExternal->appendFromClipboard();
        return true;
    }

    // Shift-L
    if (nativeCode == 38 && shift) {
        m_requestExternal->replaceFromClipboard();
        return true;
    }

    // Ctrl-R
    if (nativeCode == 19 && control) {
        request->clearFields();
        return true;
    }

    // PgUp/Numpad PgUp or Ctrl-PgUp/Ctrl-Numpad PgUp
    if (nativeCode == 329 || nativeCode == 73) {
        if (control) {
            request->selectFirstField();
        } else {
            request->selectUpField();
        }
        return true;
    }

    // PgDown/Numpad PgDown or Ctrl-PgDown/Ctrl-Numpad PgDown
    if (nativeCode == 337 || nativeCode == 81) {
        if (control) {
            request->selectLastField();
        } else {
            request->selectDownField();
        }
        return true;
    }

    return false;
}

void BackendViewModel::setHelpVisible(const bool helpVisible) noexcept
{
    if (m_helpVisible == helpVisible) return;

    m_helpVisible = helpVisible;
    emit helpVisibleChanged();
}
