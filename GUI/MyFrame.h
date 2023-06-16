//
// Created by Lenovo on 15/06/2023.
//



#ifndef MYFRAME_H
#define MYFRAME_H

#include <wx/wx.h>
#include <wx/listbox.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/grid.h>
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/mstream.h>


#include "../core/AuctionHouse.h"

class MyFrame : public wxFrame
{
public:
    //MyFrame(const wxString& title, AuctionHouse ah);

    MyFrame(const wxString& title, const std::string &token);

private:

    void OnAddButtonClick(wxCommandEvent& event);
    void OnFetchData(wxCommandEvent& event);
    void OnGridCellLeftClick(wxGridEvent& event);
    void downloadImage(std::string url);

    wxTextCtrl* textBox;
    wxButton* addButton;
    wxButton* fetchButton;
    wxGrid* grid;
    wxBoxSizer* sizer;
    wxStaticBitmap* bitmap;

    wxStaticText* string1Label;
    wxStaticText* string1Text;

    AuctionHouse ah;

wxDECLARE_EVENT_TABLE();
};

#endif // MYFRAME_H
