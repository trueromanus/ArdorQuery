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

    m_commandPaletter->setup(m_requests->getList());

    connect(m_requestPerformer, &HttpPerformerViewModel::pushErrorMessage, this, &BackendViewModel::errorNotification);
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

    // ---------
    // Perform requests

    // Ctrl-Z or F5
    if ((nativeCode == 44 && control) || nativeCode == 63) {
        m_requestPerformer->performRequest();
        return true;
    }

    // Ctrl-B or F4
    if ((nativeCode == 48 && control) || nativeCode == 62) {
        m_requestPerformer->cancelRequest();
        return true;
    }

    // ---------
    // Export

    // Ctrl-S or F10
    if ((nativeCode == 31 && control) || nativeCode == 68) {
        m_requestExternal->copyToClipboard();
        return true;
    }

    // Alt-S
    if ((nativeCode == 31 && alt)) {
        //TODO: export to file
        //request->exportToFile();
        return true;
    }

    // ---------
    // Clipboard

    // Shift-L
    if (nativeCode == 38 && shift) {
        m_requestExternal->replaceFromClipboard();
        return true;
    }

    // Ctrl-L or F3
    if ((nativeCode == 38 && control) || nativeCode == 61) {
        m_requestExternal->appendFromClipboard();
        return true;
    }

    // ---------
    // Change selection

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

    // ---------
    // Miscellanious

    // Ctrl-H or F1
    if ((nativeCode == 35 && control) || nativeCode == 59) {
        setHelpVisible(!m_helpVisible);
        return true;
    }

    // ---------
    // Fields

    // Ctrl-R
    if (nativeCode == 19 && control) {
        request->clearFields();
        return true;
    }

    // Shift-R
    if (nativeCode == 19 && shift) {
        request->clearSelectedField();
        return true;
    }

    // Ctrl-Enter
    if ((key == Qt::Key_Enter || key == Qt::Key_Return) && control) {
        request->addItem(request->selectedItem() + 1);
        return true;
    }

    // Alt-Enter
    if ((key == Qt::Key_Enter || key == Qt::Key_Return) && alt) {
        request->addItem(request->selectedItem());
        return true;
    }

    // Shift-Enter
    if ((key == Qt::Key_Enter || key == Qt::Key_Return) && shift) {
        request->addItem(-1);
        return true;
    }

    // Ctrl-{
    if ((nativeCode == 26) && control) {
        request->sortingFields(false);
        return true;
    }

    // Ctrl-}
    if ((nativeCode == 27) && control) {
        request->sortingFields(true);
        return true;
    }

    // F11
    if (key == Qt::Key_F11) {
        m_tabs->toggleTabs();
        return true;
    }

    // ---------
    // Queries

    // Ctrl-Tab
    if (key == Qt::Key_Tab && control) {
        if (!m_openedCommandPalette) {
            m_openedCommandPalette = true;
            emit openedCommandPaletteChanged();
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

void BackendViewModel::errorNotification(const QString &title, const QString &message)
{
    m_notifications->pushErrorMessage(title, message);
}
