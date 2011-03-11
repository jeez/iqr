#ifndef CLSMYENTITYRESOLVER_H
#define CLSMYENTITYRESOLVER_H


#include <QtXml>

//#if XERCES_VERSION_MAJOR >= 3
//#include <xercesc/sax/EntityResolver.hpp>
//#else
//#include <dom/DOMEntityResolver.hpp>
//#endif

//#include "iqr421_System_050501040704030101_dtd.cpp"
#include "iqr421_System_050602040805040202_dtd.cpp"

//using namespace xercesc;

namespace iqrcommon {

    class MyResolver : public QXmlEntityResolver {
    public:

        virtual QString errorString() const {
            return QString();
        }

        virtual bool resolveEntity( const QString & publicId, const QString & systemId, QXmlInputSource *& ret ){
        //InputSource* resolveEntity( const XMLCh* const publicId,
        //			    const XMLCh* const systemId){

//	    cout << "MyResolver::resolveEntity(...)" << endl;
            if (publicId == "iqrSystem.dtd" && systemId == "-//INI/iqr421") {

//		cout << "\tresolving publicId: \"iqrSystem.dtd\" and systemId \"-//INI/iqr421\" to MemBufInputSource" << endl;

                //static const char*  cpMemBufId = "iqr421_System";
                //return new MemBufInputSource ( (const XMLByte*)iqrSystemDTD_data, strlen(iqrSystemDTD_data), cpMemBufId, false);
                ret = new QXmlInputSource();
                ret->setData(QByteArray(iqrSystemDTD_data));

                return true;
/*
  with the code below, we can also use a file from disk...
  return new LocalFileInputSource ( XMLString::transcode("/home/ulysses/iqrDTD/iqrSystem.dtd"));
*/
        }
            return false;
    }
    }l;

}

#endif

/// Local Variables:
/// mode: c++
/// End:
