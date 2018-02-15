// MMessagesDlg.hpp --- "String Table" Dialog
//////////////////////////////////////////////////////////////////////////////
// RisohEditor --- Another free Win32 resource editor
// Copyright (C) 2017-2018 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//////////////////////////////////////////////////////////////////////////////

#ifndef MZC4_MMESSAGESDLG_HPP_
#define MZC4_MMESSAGESDLG_HPP_

#include "MWindowBase.hpp"
#include "RisohSettings.hpp"
#include "ConstantsDB.hpp"
#include "Res.hpp"
#include "MResizable.hpp"
#include "resource.h"

#include "MessageRes.hpp"

class MAddMsgDlg;
class MModifyMsgDlg;
class MMessagesDlg;

void InitMessageComboBox(HWND hCmb, ConstantsDB& db, MString strString);
BOOL MsgDlg_GetEntry(HWND hwnd, MESSAGE_ENTRY& entry, ConstantsDB& db);
void MsgDlg_SetEntry(HWND hwnd, MESSAGE_ENTRY& entry, ConstantsDB& db);

//////////////////////////////////////////////////////////////////////////////

class MAddMsgDlg : public MDialogBase
{
public:
    MESSAGE_ENTRY& m_entry;
    ConstantsDB& m_db;
    MessageRes& m_msg_res;

    MAddMsgDlg(ConstantsDB& db, MESSAGE_ENTRY& entry, MessageRes& msg_res)
        : MDialogBase(IDD_ADDMSG), m_entry(entry), m_db(db), m_msg_res(msg_res)
    {
    }

    BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
    {
        HWND hCmb1 = GetDlgItem(hwnd, cmb1);
        InitMessageComboBox(hCmb1, m_db, L"");

        CenterWindowDx();
        return TRUE;
    }

    void OnOK(HWND hwnd)
    {
        HWND hCmb1 = GetDlgItem(hwnd, cmb1);
        if (!MsgDlg_GetEntry(hwnd, m_entry, m_db))
        {
            Edit_SetSel(hCmb1, 0, -1);
            SetFocus(hCmb1);
            ErrorBoxDx(IDS_NOSUCHID);
            return;
        }
        INT value;
        if (m_db.HasResID(m_entry.MessageID))
        {
            value = m_db.GetResIDValue(m_entry.MessageID);
        }
        else
        {
            value = wcstol(m_entry.MessageID, NULL, 0);
        }
        if (m_msg_res.map().find((WORD)value) != m_msg_res.map().end())
        {
            Edit_SetSel(hCmb1, 0, -1);
            SetFocus(hCmb1);
            ErrorBoxDx(IDS_ALREADYEXISTS);
            return;
        }

        EndDialog(IDOK);
    }

    void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
    {
        switch (id)
        {
        case IDOK:
            OnOK(hwnd);
            break;
        case IDCANCEL:
            EndDialog(IDCANCEL);
            break;
        case psh1:
            OnPsh1(hwnd);
            break;
        }
    }

    void OnPsh1(HWND hwnd)
    {
        SendMessage(GetParent(GetParent(hwnd)), WM_COMMAND, CMDID_IDLIST, 0);
    }

    virtual INT_PTR CALLBACK
    DialogProcDx(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
            HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
            HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
        }
        return 0;
    }
};

//////////////////////////////////////////////////////////////////////////////

class MModifyMsgDlg : public MDialogBase
{
public:
    MESSAGE_ENTRY& m_entry;
    ConstantsDB& m_db;
    MessageRes& m_msg_res;

    MModifyMsgDlg(ConstantsDB& db, MESSAGE_ENTRY& entry, MessageRes& msg_res)
        : MDialogBase(IDD_MODIFYMSG), m_entry(entry), m_db(db), m_msg_res(msg_res)
    {
    }

    BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
    {
        HWND hCmb1 = GetDlgItem(hwnd, cmb1);
        InitMessageComboBox(hCmb1, m_db, L"");

        MsgDlg_SetEntry(hwnd, m_entry, m_db);
        CenterWindowDx();
        return TRUE;
    }

    void OnOK(HWND hwnd)
    {
        if (!MsgDlg_GetEntry(hwnd, m_entry, m_db))
        {
            ErrorBoxDx(IDS_NOSUCHID);
            return;
        }
        EndDialog(IDOK);
    }

    void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
    {
        switch (id)
        {
        case IDOK:
            OnOK(hwnd);
            break;
        case IDCANCEL:
            EndDialog(IDCANCEL);
            break;
        case psh1:
            OnPsh1(hwnd);
            break;
        }
    }

    void OnPsh1(HWND hwnd)
    {
        SendMessage(GetParent(GetParent(hwnd)), WM_COMMAND, CMDID_IDLIST, 0);
    }

    virtual INT_PTR CALLBACK
    DialogProcDx(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
            HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
            HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
        }
        return DefaultProcDx();
    }
};

//////////////////////////////////////////////////////////////////////////////

class MMessagesDlg : public MDialogBase
{
public:
    MessageRes& m_msg_res;
    ConstantsDB& m_db;
    MResizable m_resizable;
    HICON m_hIcon;
    HICON m_hIconSm;

    MMessagesDlg(ConstantsDB& db, MessageRes& msg_res)
        : MDialogBase(IDD_MESSAGES), m_msg_res(msg_res), m_db(db)
    {
        m_hIcon = LoadIconDx(3);
        m_hIconSm = LoadSmallIconDx(3);
    }

    ~MMessagesDlg()
    {
        DestroyIcon(m_hIcon);
        DestroyIcon(m_hIconSm);
    }

    void InitCtl1(HWND hCtl1)
    {
        ListView_DeleteAllItems(hCtl1);

        typedef MessageRes::map_type map_type;
        const map_type& map = m_msg_res.map();

        INT i = 0;
        map_type::const_iterator it, end = map.end();
        for (it = map.begin(); it != end; ++it)
        {
            if (it->second.empty())
                continue;

            MStringW str = m_db.GetNameOfResID(IDTYPE_MESSAGE, it->first);

            LV_ITEM item;
            ZeroMemory(&item, sizeof(item));
            item.iItem = i;
            item.mask = LVIF_TEXT;
            item.iSubItem = 0;
            item.pszText = &str[0];
            ListView_InsertItem(hCtl1, &item);

            str = mstr_quote(it->second);

            ZeroMemory(&item, sizeof(item));
            item.iItem = i;
            item.mask = LVIF_TEXT;
            item.iSubItem = 1;
            item.pszText = &str[0];
            ListView_SetItem(hCtl1, &item);

            ++i;
        }
    }

    BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
    {
        HWND hCtl1 = GetDlgItem(hwnd, ctl1);
        ListView_SetExtendedListViewStyle(hCtl1, LVS_EX_FULLROWSELECT);

        LV_COLUMN column;
        ZeroMemory(&column, sizeof(column));

        column.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
        column.fmt = LVCFMT_LEFT;
        column.cx = 140;
        column.pszText = LoadStringDx(IDS_MESSAGEID);
        column.iSubItem = 0;
        ListView_InsertColumn(hCtl1, 0, &column);

        column.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
        column.fmt = LVCFMT_LEFT;
        column.cx = 500;
        column.pszText = LoadStringDx(IDS_MESSAGEVALUE);
        column.iSubItem = 1;
        ListView_InsertColumn(hCtl1, 1, &column);

        InitCtl1(hCtl1);

        UINT state = LVIS_SELECTED | LVIS_FOCUSED;
        ListView_SetItemState(hCtl1, 0, state, state);
        SetFocus(hCtl1);

        m_resizable.OnParentCreate(hwnd);

        m_resizable.SetLayoutAnchor(ctl1, mzcLA_TOP_LEFT, mzcLA_BOTTOM_RIGHT);
        m_resizable.SetLayoutAnchor(psh1, mzcLA_TOP_RIGHT);
        m_resizable.SetLayoutAnchor(psh2, mzcLA_TOP_RIGHT);
        m_resizable.SetLayoutAnchor(psh3, mzcLA_TOP_RIGHT);
        m_resizable.SetLayoutAnchor(IDOK, mzcLA_BOTTOM_RIGHT);
        m_resizable.SetLayoutAnchor(IDCANCEL, mzcLA_BOTTOM_RIGHT);

        SendMessageDx(WM_SETICON, ICON_BIG, (LPARAM)m_hIcon);
        SendMessageDx(WM_SETICON, ICON_SMALL, (LPARAM)m_hIconSm);

        CenterWindowDx();
        return TRUE;
    }

    void OnAdd(HWND hwnd)
    {
        HWND hCtl1 = GetDlgItem(hwnd, ctl1);

        MESSAGE_ENTRY s_entry;
        ZeroMemory(&s_entry, sizeof(s_entry));
        MAddMsgDlg dialog(m_db, s_entry, m_msg_res);
        if (dialog.DialogBoxDx(hwnd) != IDOK)
        {
            return;
        }

        INT iItem;

        LV_FINDINFO find;
        TCHAR sz[128];
        ZeroMemory(&find, sizeof(find));
        find.flags = LVFI_STRING;
        find.psz = sz;
        iItem = ListView_FindItem(hCtl1, -1, &find);
        if (iItem != -1)
        {
            ListView_DeleteItem(hCtl1, iItem);
        }

        LV_ITEM item;
        iItem = ListView_GetItemCount(hCtl1);

        ZeroMemory(&item, sizeof(item));
        item.iItem = iItem;
        item.mask = LVIF_TEXT;
        item.iSubItem = 0;
        item.pszText = s_entry.MessageID;
        ListView_InsertItem(hCtl1, &item);

        std::wstring str = s_entry.MessageValue;
        str = mstr_quote(str);

        ZeroMemory(&item, sizeof(item));
        item.iItem = iItem;
        item.mask = LVIF_TEXT;
        item.iSubItem = 1;
        item.pszText = &str[0];
        ListView_SetItem(hCtl1, &item);

        UINT state = LVIS_SELECTED | LVIS_FOCUSED;
        ListView_SetItemState(hCtl1, iItem, state, state);
    }

    void OnDelete(HWND hwnd)
    {
        HWND hCtl1 = GetDlgItem(hwnd, ctl1);

        INT iItem = ListView_GetNextItem(hCtl1, -1, LVNI_ALL | LVNI_SELECTED);
        if (iItem >= 0)
        {
            ListView_DeleteItem(hCtl1, iItem);
        }
    }

    void GetEntry(HWND hwnd, HWND hCtl1, INT iItem, MESSAGE_ENTRY& entry)
    {
        ListView_GetItemText(hCtl1, iItem, 0, entry.MessageID, _countof(entry.MessageID));
        mstr_trim(entry.MessageID);

        ListView_GetItemText(hCtl1, iItem, 1, entry.MessageValue, _countof(entry.MessageValue));
        mstr_trim(entry.MessageValue);
        if (entry.MessageValue[0] == L'"')
        {
            mstr_unquote(entry.MessageValue);
        }
    }

    void OnModify(HWND hwnd)
    {
        HWND hCtl1 = GetDlgItem(hwnd, ctl1);

        INT iItem = ListView_GetNextItem(hCtl1, -1, LVNI_ALL | LVNI_SELECTED);
        if (iItem < 0)
        {
            return;
        }

        MESSAGE_ENTRY s_entry;
        GetEntry(hwnd, hCtl1, iItem, s_entry);

        MModifyMsgDlg dialog(m_db, s_entry, m_msg_res);
        if (IDOK == dialog.DialogBoxDx(hwnd))
        {
            MString str = mstr_quote(s_entry.MessageValue);
            ListView_SetItemText(hCtl1, iItem, 1, &str[0]);
        }
    }

    void OnOK(HWND hwnd)
    {
        HWND hCtl1 = GetDlgItem(hwnd, ctl1);

        INT iItem, nCount = ListView_GetItemCount(hCtl1);
        if (nCount == 0)
        {
            ErrorBoxDx(IDS_DATAISEMPTY);
            return;
        }

        m_msg_res.map().clear();

        MESSAGE_ENTRY s_entry;
        for (iItem = 0; iItem < nCount; ++iItem)
        {
            GetEntry(hwnd, hCtl1, iItem, s_entry);

            WORD wID = (WORD)m_db.GetResIDValue(s_entry.MessageID);
            std::wstring str = s_entry.MessageValue;

            m_msg_res.map().insert(std::make_pair(wID, str));
        }

        EndDialog(IDOK);
    }

    void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
    {
        switch (id)
        {
        case psh1:
            OnAdd(hwnd);
            break;
        case psh2:
            OnModify(hwnd);
            break;
        case psh3:
            OnDelete(hwnd);
            break;
        case IDOK:
            OnOK(hwnd);
            break;
        case IDCANCEL:
            EndDialog(IDCANCEL);
            break;
        }
    }

    LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnmhdr)
    {
        if (idFrom == ctl1)
        {
            if (pnmhdr->code == LVN_KEYDOWN)
            {
                LV_KEYDOWN *KeyDown = (LV_KEYDOWN *)pnmhdr;
                if (KeyDown->wVKey == VK_DELETE)
                {
                    OnDelete(hwnd);
                    return 0;
                }
            }
            if (pnmhdr->code == NM_DBLCLK)
            {
                OnModify(hwnd);
                return 0;
            }
        }
        return 0;
    }

    void OnSize(HWND hwnd, UINT state, int cx, int cy)
    {
        m_resizable.OnSize();
    }

    virtual INT_PTR CALLBACK
    DialogProcDx(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
            HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
            HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
            HANDLE_MSG(hwnd, WM_NOTIFY, OnNotify);
            HANDLE_MSG(hwnd, WM_SIZE, OnSize);
        }
        return DefaultProcDx();
    }
};

//////////////////////////////////////////////////////////////////////////////

#endif  // ndef MZC4_MMESSAGESDLG_HPP_