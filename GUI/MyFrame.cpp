
#include <utility>
#include <thread>
#include <curl/curl.h>
#include <fstream>



#include "myframe.h"

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t written = fwrite(contents, size, nmemb, static_cast<FILE*>(userp));
    return written;
}

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
                EVT_BUTTON(wxID_HIGHEST+1, MyFrame::OnAddButtonClick)
                EVT_BUTTON(wxID_HIGHEST+2, MyFrame::OnFetchData)
                EVT_GRID_CELL_LEFT_CLICK(MyFrame::OnGridCellLeftClick)
wxEND_EVENT_TABLE()


MyFrame::MyFrame(const wxString& title, const std::string& token)
        : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)), ah(token) {
    sizer = new wxBoxSizer(wxVERTICAL);
    sizer->SetMinSize(wxSize(800,600));
    SetSizeHints(wxSize(800, 600), wxDefaultSize);
    wxBoxSizer* topHorizontalBox = new wxBoxSizer(wxHORIZONTAL);

    wxWindow::Refresh();
    wxWindow::Update();

    topHorizontalBox->AddSpacer(10);
    //wxStaticText* label = new wxStaticText(this, wxID_ANY, "Item Name: ");
    //sizer->Add(label, 0, wxALL, 10);

    addButton = new wxButton(this, wxID_HIGHEST+1, "Search");
    //sizer->Add(addButton, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);
    //addButton->Bind(wxEVT_BUTTON, &MyFrame::OnAddButtonClick, this);
    addButton->Disable();

    fetchButton = new wxButton(this, wxID_HIGHEST+2, "Fetch/Update Data");
    //sizer->Add(fetchButton, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);
    //fetchButton->Bind(wxEVT_BUTTON, &MyFrame::OnFetchData, this);




    textBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    sizer->Add(textBox, 0, wxEXPAND | wxUP |  wxLEFT | wxRIGHT, 10);
    textBox->WriteText("Item Name");

    topHorizontalBox->Add(addButton);
    topHorizontalBox->Add(fetchButton);

    sizer->Add(topHorizontalBox);

    grid = new wxGrid(this, wxID_ANY);
    grid->CreateGrid(0, 5);
    grid->SetColLabelValue(0, "Item ID");
    grid->SetColLabelValue(1, "Name");
    grid->SetColLabelValue(2, "Quantity");
    grid->SetColLabelValue(3, "Unit Price");
    grid->SetColLabelValue(4, "Time Left");

    grid->SetColSize(0,122);
    grid->SetColSize(1,252);
    grid->SetColSize(3, 122);
    grid->SetColSize(3,122);
    grid->SetColSize(4,122);


    wxStaticBoxSizer* horizontalBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Selected auction");
    sizer->Add(horizontalBoxSizer, 0, wxEXPAND | wxALL, 10);

    wxImage image(56, 56);
    bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(image));
    horizontalBoxSizer->Add(bitmap, 0, wxALL, 5);

    wxBoxSizer* verticalBoxSizer = new wxBoxSizer(wxVERTICAL);

    string1Label = new wxStaticText(this, wxID_ANY, "");
    verticalBoxSizer->Add(string1Label, 0, wxLEFT | wxRIGHT, 5);
    string1Text = new wxStaticText(this, wxID_ANY, "");
    verticalBoxSizer->Add(string1Text, 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);

    horizontalBoxSizer->Add(verticalBoxSizer);
    sizer->Add(grid, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    SetSizerAndFit(sizer);
}

void MyFrame::OnAddButtonClick(wxCommandEvent& event) {
    std::thread t([this]() {
        addButton->Disable();

        wxString text = textBox->GetValue();
        std::string name = std::string(text);
        auto qres = ah.query(name);

        if(grid->GetNumberRows() != 0){
            grid->DeleteRows(0, grid->GetNumberRows());
        }
        grid->AppendRows(qres.size());

        for (size_t i = 0; i < qres.size(); i++) {
            const AuctionEntry& ae = qres[i];
            grid->SetCellValue(i, 0, wxString::Format("%d", ae.itemID));
            grid->SetCellValue(i, 1, wxString::FromUTF8(ah.idToItem(ae.itemID).getName()));
            grid->SetCellValue(i, 2, wxString::Format("%d", ae.quantity));
            grid->SetCellValue(i, 3, wxString::Format("%s", ae.getPriceString()));
            grid->SetCellValue(i, 4, wxString::FromUTF8(ae.timeLeft));
        }

        textBox->Clear();
        addButton->Enable();

        SetSizeHints(wxSize(800, 600), wxDefaultSize);
        wxWindow::Refresh();
        wxWindow::Update();
    });

    t.detach();
    event.Skip();
}

void MyFrame::OnFetchData(wxCommandEvent& event) {
    fetchButton->Disable();
    addButton->Disable();
    textBox->Clear();
    textBox->WriteText("Fetching data...");
    textBox->SetEditable(false);

    std::thread fetchThread([this]() {


        ah.fetchData();
        wxCommandEvent updateEvent(wxEVT_COMMAND_TEXT_UPDATED);
        wxPostEvent(this, updateEvent);

        addButton->Enable();
        textBox->SetFocus();
        textBox->SetEditable(true);
        textBox->Clear();

    });

    fetchThread.detach();
    event.Skip();
}


void MyFrame::OnGridCellLeftClick(wxGridEvent& event) {
    int selectedRow = event.GetRow();
    wxString itemID = grid->GetCellValue(selectedRow, 0);

    std::string id = std::string(itemID.mb_str());

    std::string imageURL = ah.getIMGbyID(std::stoi(id));
    downloadImage(imageURL);

    wxImage image("image.jpg");
    if (image.IsOk()) {
        Item i = ah.idToItem(std::stoi(id));
        image.Rescale(56, 56);
        wxBitmap newBitmap(image);
        bitmap->SetBitmap(newBitmap);

        bitmap->Refresh();

        string1Label->SetLabel(i.getName());
        string1Text->SetLabel(i.getDesc());
    }

}

void MyFrame::downloadImage(std::string url) {
    CURL* curl;
    FILE* fp;
    CURLcode res;
    std::string file_path = "image.jpg";

    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen(file_path.c_str(), "wb");
        if (fp)
        {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                //std::cout << "Error: " << curl_easy_strerror(res) << std::endl;
            }
            fclose(fp);
        }
        curl_easy_cleanup(curl);
    }
}

