


#ifndef MYAPP_H
#define MYAPP_H

#include <wx/wx.h>
#include <filesystem>
#include <stdexcept>
#include "myframe.h"

class App : public wxApp
{
public:
    virtual bool OnInit() override;

private:
};

#endif // MYAPP_H
