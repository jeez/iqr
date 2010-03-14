/****************************************************************************
 ** $Filename: ConfigTagLibrary.h
 **
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Thu Oct  2 21:21:23 2003
 ** $Date: 2003/10/04 10:07:08 $
 **
 ** $Log: ConfigTagLibrary.h,v $
 ** Revision 1.1  2003/10/04 10:07:08  ulysses
 ** Initial revision
 **
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/


#ifndef CONFIGTAGLIBRARY_H
#define CONFIGTAGLIBRARY_H    /*+ To stop multiple inclusions. +*/

class ConfigTagLibrary     {
public:
    static const char* DataManagerConfiguration() {
	return "iqrDataManagerConfiguration";
    }

    static const char* PositionTag() {
	return "Position";
    }

    static const char* PositionXTag() {
	return "x";
    }

    static const char* PositionYTag() {
	return "y";
    }

    static const char* Geometry() {
	return "Geometry";
    }

    static const char* GeometryWidthTag() {
	return "width";
    }

    static const char* GeometryHeightTag() {
	return "height";
    }

    static const char* ItemIDTag() {
	return "itemID";
    }

    static const char* GroupType() {
	return "group";
    }

    static const char* ConnectionType() {
	return "connection";
    }
    

    static const char* IDTag() {
	return "ID";
    }

    static const char* SelectedIndicesTag() {
	return "indices";
    }

    static const char* DataClientTag() {
	return "DataClient";
    }

    static const char* StateVariableDisplayTag() {
	return "StateVariableDisplay";
    }

    static const char* StateVariableDisplaysTag() {
	return "StateVariableDisplays";
    }

    static const char* TypeTag() {
	return "type";
    }

    static const char* CaptionTag() {
	return "caption";
    }

    static const char* PlotTypeGroup(){
	return "PlotTypeGroup";
    }

    static const char* PlotTypeTime(){
	return "PlotTypeTime";
    }

    static const char* PlotTypeConnection(){
	return "PlotTypeConnection";
    }

    static const char* DataSampler(){
	return "DataSampler";
    }

};

#endif /* CONFIGTAGLIBRARY_H */
