#include "TargetList.h"

TargetList::TargetList(wxWindow *parent, wxWindowID id)
	: wxListBox(parent, id)
{
	wxPrintf("Inited TargetList inherited class.\n");

	// add some standard items
	listText.push_back("All");

	// take the vector and make the list with it
	Clear();
	for (int i = 0; i < listText.size(); i++){
		Append(wxString(listText.at(i)));
	}


}
