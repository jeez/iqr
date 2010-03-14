#include <iostream>
#include "ClsFESystem.h"

#include "optionsParameter.hpp"
#include "stringParameter.hpp"

#include "iqrUtils.h"
#include "tagLibrary.hpp"

ClsFESystem::ClsFESystem() {
    strSystemID = "";

    addStringParameter(ClsTagLibrary::NameTag(), "System Name",
		       "", 
		       true, false, 
		       "Name", 
		       "Properties");

    addStringParameter(ClsTagLibrary::NotesTag(), "Notes",
		       "", 
		       true, true, 
		       "", 
		       "Notes");

    addStringParameter(ClsTagLibrary::AuthorTag(), "Author",
		       "", 
		       true, false, 
		       "Author", 
		       "Properties");

    addStringParameter(ClsTagLibrary::DateTag(), "Date",
		       "", 
		       true, false, 
		       "Date", 
		       "Properties");

    addStringParameter(ClsTagLibrary::HostnameTag(), "Hostname",
		       "", 
		       true, false, 
		       "Hostname", 
		       "Properties");


    addBoolParameter(ClsTagLibrary::SyncPlotsTag(), "SyncPlots",
		       false, 
		       "Sync plots to update", 
		       "Properties");


    addIntParameter(ClsTagLibrary::PortTag() /* _strName */, "Port" /* _strLabel */,
		    54923 /* _iValue */,
		    52000 /* _iMinimum */,
		    60000 /* _iMaximum */,
		    "Port" /* _strDescription */,
		    "Properties" /* _strCategory */ );
    

    addIntParameter(ClsTagLibrary::CyclesPerSecondTag(), /* _strName */
					 "Cycles Per Second", /* _strLabel */
					 0, /* _iValue */
					 0, /* _iMinimum */
					 1000, /* _iMaximum */ 
					 "Update Cycles Per Second (0 = unconstraint)", /* _strDescription */
					 "Properties" /* _strCategory */);

};


ClsFESystem::ClsFESystem(string _strSystemID, string _strSystemName ) : strSystemID(_strSystemID) { 

    addStringParameter(ClsTagLibrary::NameTag(), "System Name",
		       _strSystemName, 
		       true, false, 
		       "Name", 
		       "Properties");

    addStringParameter(ClsTagLibrary::NotesTag(), "Notes",
		       "", 
		       true, true, 
		       "", 
		       "Notes");

    addStringParameter(ClsTagLibrary::AuthorTag(), "Author",
		       "", 
		       true, true, 
		       "", 
		       "Properties");

    addStringParameter(ClsTagLibrary::DateTag(), "Date",
		       "", 
		       true, true, 
		       "", 
		       "Properties");

    addStringParameter(ClsTagLibrary::HostnameTag(), "Hostname",
		       "", 
		       true, true, 
		       "", 
		       "Properties");

    addIntParameter(ClsTagLibrary::PortTag() /* _strName */, "Port" /* _strLabel */,
		    54923 /* _iValue */,
		    52000 /* _iMinimum */,
		    60000 /* _iMaximum */,
		    "Port" /* _strDescription */,
		    "Properties" /* _strCategory */ );
   
    addIntParameter(ClsTagLibrary::CyclesPerSecondTag(), /* _strName */
					 "Cycles Per Second", /* _strLabel */
					 0, /* _iValue */
					 0, /* _iMinimum */
					 1000, /* _iMaximum */ 
					 "Update Cycles Per Second (0 = unconstraint)", /* _strDescription */
					 "Properties" /* _strCategory */);
	

};
    

void ClsFESystem::setSystemID( string _strSystemID ){
    strSystemID = _strSystemID;
};
string ClsFESystem::getSystemID( ){ return strSystemID;};           

string ClsFESystem::getNotes() { return getParameter(ClsTagLibrary::NotesTag())->getValueAsString(); };
void ClsFESystem::setNotes(string _str){ setParameter(ClsTagLibrary::NotesTag(),  _str); };

string ClsFESystem::getSystemName(){ 
    return getParameter(ClsTagLibrary::NameTag())->getValueAsString(); 
};


string ClsFESystem::getSystemHostName() {
    return getParameter(ClsTagLibrary::HostnameTag())->getValueAsString();
}

int ClsFESystem::getSystemPort() {
    return iqrUtils::string2int(getParameter(ClsTagLibrary::PortTag())->getValueAsString());
}

int ClsFESystem::getCyclesPerSecond() {
    return iqrUtils::string2int(getParameter(ClsTagLibrary::CyclesPerSecondTag())->getValueAsString());
}

bool ClsFESystem::getSyncPlots() {
    return iqrUtils::string2bool(getParameter(ClsTagLibrary::SyncPlotsTag())->getValueAsString());
}

//// Local Variables:
//// mode: c++
//// compile-command: "cd ../.. && make -k "
//// End:
