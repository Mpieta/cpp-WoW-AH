#include <fstream>

#include "App.h"


bool App::OnInit()
{
    std::string token;
    if(!std::filesystem::exists("API_token.txt")){
        std::cout<<"file not found\n";
        throw std::invalid_argument("Could not find API_token.txt");
    }
    else{
        std::ifstream f("API_token.txt");
        f>>token;
        f.close();
    }

    wxInitAllImageHandlers();
    MyFrame *mainFrame = new MyFrame("WoW Auction House", token);
    wxIcon icon;
    icon.LoadFile("icon.png", wxBITMAP_TYPE_PNG);
    mainFrame->SetIcon(icon);
    mainFrame->Show();
    
    return true;
}


wxIMPLEMENT_APP(App);