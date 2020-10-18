
#include <wx/wx.h>
#include <wx/listbox.h>


#include <string>
#include <vector>

class TargetList : public wxListBox
{
public:
	TargetList(wxWindow *parent, wxWindowID id); // need at least these two?

private:
	std::vector<std::string> listText; // this is really just for testing, the data is already obtainable via GetStrings()?


};
