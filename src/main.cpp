// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/listbox.h>
#include <wx/listctrl.h>

#include "TargetList.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};
class MyFrame : public wxFrame
{
public:
    MyFrame();
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void onButton(wxCommandEvent& event);
    void onOk(wxCommandEvent& event);
    void onSpecificButton(wxCommandEvent& event);
    void onMyListSelection(wxListEvent& event);
    void onMyListDeselection(wxListEvent& event);

    wxTextCtrl *control;
    wxListCtrl *editList;
    wxListView *lstview;
};
enum
{
    ID_Hello = 1
};
wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}
MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
	           "Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	SetMenuBar( menuBar );
	CreateStatusBar();
	SetStatusText("Welcome to wxWidgets!");
	Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
	Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);

	// testing some sizers? layout equivalents
	// wxPanel *panel = new wxPanel(this, -1); // make a panel for the frame

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *hbox2 = new wxBoxSizer(wxHORIZONTAL);

	wxButton *ok = new wxButton(this, wxID_OK, wxT("Ok"));
	wxButton *cancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));
	wxButton *random = new wxButton(this, -1, wxT("random"));

	// wxPanel *toppanel = new wxPanel(this,-1);
	// hbox1->Add(toppanel, 1, wxEXPAND | wxALL); // add a new PANEL (like another group box ) to horizontal layout

	// wxListBox *list = new wxListBox(this, wxID_ANY);
	wxString testlistitem = "first item";
	// list->InsertItems(1, &testlistitem, 0);
	// hbox1->Add(list, 1, wxEXPAND | wxALL); // expand the list to occupy this sizer
	hbox1->Add(random, 0);
	vbox->Add(hbox1, 1, wxEXPAND | wxALL);// add horizontal layout to vertical layout, expand horizontal sizer to fill up the whole pane

	hbox2->Add(ok);
	hbox2->Add(cancel);

	vbox->Add(hbox2, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10); // add the other horiz layout to vlayout
	// panel->SetSizer(vbox); // set the 'main' layout for the panel

	SetSizer(vbox);

	// // test connection (deprecated method)
	// Connect(wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::onButton));

	// new advised method
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::onButton, this, wxID_ANY);

	// use the new bind (advised)
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::onOk, this, wxID_OK); // this links it to the window ID in the last argument
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::onSpecificButton, this, random->GetId()); // but you could do this and then search in the method for the event object instead

	// let's try some edits?
	control = new wxTextCtrl(this, wxID_ANY); // by default, is single line?
	hbox1->Add(control); // can i add it here?


	// try adding my inherited class
	TargetList *myList = new TargetList(this, wxID_ANY);
	hbox1->Add(myList,1, wxEXPAND | wxALL);

	// now make a listctrl (better!) that can be editable?
	editList = new wxListCtrl(this, wxID_ANY); // by default it is style = wxLC_ICON
	editList->SetWindowStyleFlag(wxLC_LIST | wxLC_EDIT_LABELS); // this makes editable? and use this because style is very far down the default args in ctor
	editList->InsertItem(0,wxString("editable?"));
	editList->InsertItem(0,wxString("added after, on top"));
	hbox1->Add(editList,1,wxEXPAND|wxALL);
	editList->SetItemState(0, 0, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED);
	editList->SetItemState(1, 0, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED);

	Bind(wxEVT_LIST_ITEM_SELECTED, &MyFrame::onMyListSelection, this, editList->GetId());
	Bind(wxEVT_LIST_ITEM_DESELECTED, &MyFrame::onMyListDeselection, this, editList->GetId());

	// // maybe these problems are better off with listview?
	// lstview = new wxListView(this, wxID_ANY);
	// lstview->SetWindowStyleFlag(wxLC_LIST | wxLC_EDIT_LABELS);
	// lstview->InsertItem(0,wxString("this is a listview"));
	// hbox1->Add(lstview,1,wxEXPAND|wxALL);
	//
	// wxPrintf("Check if listview item in focus? %ld\n", lstview->GetFocusedItem());
}

void MyFrame::onMyListDeselection(wxListEvent& event)
{
	wxPrintf("Caught deselection event. item %ld\n", event.GetIndex());

	// check focus for deselected item
	// query mouse position?
	int flags;
	wxPoint mousePos = editList->ScreenToClient(wxGetMousePosition());
	long hititem = editList->HitTest(mousePos, flags);

	// wxPrintf("Hit item %ld, flags = %d, mousepos = %d, %d\n", hititem, flags, mousePos.x, mousePos.y);

	if (hititem < 0){
		wxPrintf("Clicked in empty space?\n");
		// query state of deselected item
		int deselectedState = editList->GetItemState(event.GetIndex(), wxLIST_STATE_FOCUSED);
		wxPrintf("Deselected item state = %d, focused state = %d\n", deselectedState, wxLIST_STATE_FOCUSED);
		// attempt to defocus it?
		wxPrintf("Return code of defocus setstate = %d\n", editList->SetItemState(event.GetIndex(), 0, wxLIST_STATE_FOCUSED));
	}

}

void MyFrame::onMyListSelection(wxListEvent& event)
{
	wxPrintf("================== Caught selection event, item %ld\n", event.GetIndex());
	// // get mouse position?
	// wxPoint mousePos = editList->ScreenToClient(wxGetMousePosition()); // make sure you use the widget's own ScreenToClient! otherwise its wrong coords!
	//
	// int flags;
	// long hititem = editList->HitTest(mousePos, flags);
	// wxPrintf("Hit item %ld, flags = %d, mousepos = %d, %d\n", hititem, flags, mousePos.x, mousePos.y);
}

void MyFrame::onSpecificButton(wxCommandEvent& event)
{
	wxPrintf("Only random button should get this. Line = %s\n", this->control->GetLineText(0));// need to use wxprintf
	event.Skip();
}

void MyFrame::onOk(wxCommandEvent& event)
{
	wxPrintf("Ok was clicked\n");
	// this->Skip(true); // how to use this to further propagate and use both handlers?
	event.Skip();
}

void MyFrame::onButton(wxCommandEvent& event)
{
	wxPrintf("Button was clicked?\n");
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}
void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}
