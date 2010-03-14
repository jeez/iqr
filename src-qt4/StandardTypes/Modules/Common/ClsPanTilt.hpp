#ifndef CLSPANTILE_HPP
#define CLSPANTILE_HPP

#include <string>

using namespace std;

namespace iqrcommon {

    template <class TParent> class ClsPanTilt  {
	
    public:
	ClsPanTilt(TParent *_tParent, string _strParentLabel, int _iUnitID=0, string _strLabel ="") : 
	    tParent(_tParent), 
	    strParentLabel(_strParentLabel),
	    iUnitID(_iUnitID), 
	    strLabel(_strLabel) {
	    bAbsolutePosition = true;
	    fPanRelFactor = 10.;
	    fTiltRelFactor = 10.;

	    fMinPan = fMaxPan = fMinTilt = fMaxTilt = 0;
	    


	    stateVariablePanTiltIn = tParent->addInputFromGroup("ptInput" + strLabel, "Pant/Tilt Input" + strLabel);
	    
	    string strPaddedLabel;
	    strPaddedLabel = " " + strLabel;

	    clsOptionsParameterPanTiltMode = 
		tParent->addOptionsParameter("pantiltMode" + strPaddedLabel,  /* _strName */
					     "Pant/Tilt Mode" + strPaddedLabel,  /* _strLabel */
					     false /* _bReadOnly */,
					     "Pant/Tilt Mode" + strPaddedLabel,  /* _strDescription */
					     "Pan/Tilt" /* _strCategory */);
	    clsOptionsParameterPanTiltMode->addOption(PanTiltModeRelative());
	    clsOptionsParameterPanTiltMode->addOption(PanTiltModeAbsolute());
	    clsOptionsParameterPanTiltMode->setValue(PanTiltModeRelative());


	    clsIntParameterPanNrSteps = 
		tParent->addIntParameter("PanNrSteps", /* _strName */
					 "Nr. Steps Pan",  /* _strLabel */
					 20, /* _iValue */
					 1, /* _iMinimum */
					 1000, /* _iMaximum */ 
					 "Nr. Steps Pan" /* _strDescription */,
					 "Pan/Tilt" /* _strCategory */);
	    
	    clsIntParameterTiltNrSteps = 
		tParent->addIntParameter("TiltNrSteps", /* _strName */
					 "Nr. Steps Tilt",  /* _strLabel */
					 20, /* _iValue */
					 1, /* _iMinimum */
					 1000, /* _iMaximum */ 
					 "Nr. Steps Tilt" /* _strDescription */,
					 "Pan/Tilt" /* _strCategory */);
	};
	
	void init(){
//	    cout << "ClsPanTilt::init" << endl;
	    string strPTMode = clsOptionsParameterPanTiltMode->getValue();
	    if(!strPTMode.compare(PanTiltModeAbsolute())){
		bAbsolutePosition = true;
//		cout << "abs" << endl;
	    } else {
		bAbsolutePosition = false;
//		cout << "rel" << endl;
	    }

	    if(stateVariablePanTiltIn->getTarget()->getStateArray().getWidth()<4){
		throw ModuleError(string("Module \"") + strParentLabel + "\": needs 4 cells for output");
	    }

	    iPanNrSteps = clsIntParameterPanNrSteps->getValue(); 
	    TiltNrSteps = clsIntParameterTiltNrSteps->getValue(); 
	    

	    fMinPan = tParent->getMinPan();
	    fMaxPan = tParent->getMaxPan();
	    fMinTilt= tParent->getMinTilt();
	    fMaxTilt= tParent->getMaxTilt();

/*
	    cout << "fMinPan : " << fMinPan  << endl;  
	    cout << "fMaxPan : " << fMaxPan	 << endl; 
	    cout << "fMinTilt: " << fMinTilt << endl; 
	    cout << "fMaxTilt: " << fMaxTilt << endl;
*/
	    
	    fPanRelFactor = fabs(fMaxPan - fMinPan) / (double)iPanNrSteps;
	    fTiltRelFactor = fabs(fMaxTilt - fMinTilt) / (double)TiltNrSteps;




	}


	pair<double,double> calculatePanTilt(int iCurrentPan, int iCurrentTilt){

	    pair<double,double> pairTemp;

	    StateArray &clsStateArrayInput = stateVariablePanTiltIn->getTarget()->getStateArray();
	    double fPanLeft = clsStateArrayInput[0][0];
	    double fPanRight = clsStateArrayInput[0][1];
	    double fTiltUp = clsStateArrayInput[0][2];
	    double fTiltDown = clsStateArrayInput[0][3];

/*
	    cout << "fPanLeft :" << fPanLeft  << endl;
	    cout << "fPanRight:" << fPanRight << endl;
	    cout << "fTiltUp  :" << fTiltUp   << endl;
	    cout << "fTiltDown:" << fTiltDown << endl;

	    cout << "fMinPan : " << fMinPan  << endl;  
	    cout << "fMaxPan : " << fMaxPan	 << endl; 
	    cout << "fMinTilt: " << fMinTilt << endl; 
	    cout << "fMaxTilt: " << fMaxTilt << endl;
*/

	    if(bAbsolutePosition){
		/* assuming values are < 1.0 */
//		double fPan = abs(fPanLeft-fPanRight) * (fMaxPan-fMinPan) + fMinPan;
//		double fTilt = abs(fTiltUp-fTiltDown) * (fMaxTilt-fMinTilt) + fMinTilt;

		double fPan  = (fMaxPan-fMinPan)/2.   * (fPanRight-fPanLeft) + (fMaxPan-fMinPan)/2.   + fMinPan;
		double fTilt = (fMaxTilt-fMinTilt)/2. * (fTiltUp-fTiltDown)  + (fMaxTilt-fMinTilt)/2. + fMinTilt;
		
		int iPan = (fPan < fMinPan ? (int)fMinPan : (int)fPan);
		iPan = (iPan > fMaxPan ? (int)fMaxPan : iPan);
	    
		int iTilt = (fTilt < fMinTilt ? (int)fMinTilt : (int)fTilt);
		iTilt = (iTilt > fMaxTilt ? (int)fMaxTilt : iTilt);
		pairTemp.first = iPan;
		pairTemp.second = iTilt;
	    } else {
		int iPanIncr = (int)((fPanLeft-fPanRight)*fPanRelFactor);
		int iTiltIncr = (int)((fTiltUp-fTiltDown)*fTiltRelFactor);

/*
		cout << "fPanRelFactor: " << fPanRelFactor << endl;
		cout << "fTiltRelFactor: " << fTiltRelFactor << endl;
		cout << "iCurrentPan:" << iCurrentPan << endl;
		cout << "iPanIncr:" << iPanIncr << endl;
		cout << "fMinPan:" << fMinPan << endl;
		cout << "fMaxPan:" << fMaxPan << endl;
*/


		

		if((iCurrentPan+iPanIncr) < fMinPan || 
		   (iCurrentPan+iPanIncr) > fMaxPan){
		    iPanIncr = 0;	
//		    cout << "reset iPanIncr" << endl;
		}

		if((iCurrentTilt+iTiltIncr) < fMinTilt || 
		   (iCurrentTilt+iTiltIncr) > fMaxTilt){
		    iTiltIncr = 0;
//		    cout << "reset iTiltIncr" << endl;
		}
		pairTemp.first = iCurrentPan+iPanIncr;
		pairTemp.second = iCurrentTilt+iTiltIncr;
	    }
	    return pairTemp;
	}


	void doPanTilt(int iCurrentPan, int iCurrentTilt){

	    pair<double,double> pairTemp = calculatePanTilt(iCurrentPan, iCurrentTilt);
	    if(bAbsolutePosition){
		tParent->setPanTiltAbs((int)pairTemp.first, (int)pairTemp.second, iUnitID);
	    } else {
		tParent->setPanTiltRel((int)pairTemp.first, (int)pairTemp.second, iUnitID);
	    }
	}

	bool absolutePosition(){return bAbsolutePosition;};

    private:
	TParent *tParent;
	string strParentLabel;
	/* parameter */
	bool bAbsolutePosition;
	double fPanRelFactor;
	double fTiltRelFactor;
	int iUnitID;
	string strLabel;

	double fMinPan, fMaxPan;
	double fMinTilt, fMaxTilt;


	StateVariablePtr* stateVariablePanTiltIn;
	ClsOptionsParameter* clsOptionsParameterPanTiltMode;
	ClsIntParameter* clsIntParameterPanNrSteps;
	ClsIntParameter* clsIntParameterTiltNrSteps;

	int iPanNrSteps;
	int TiltNrSteps;


	static const char* PanTiltModeRelative()  { return "relative"; }
	static const char* PanTiltModeAbsolute()  { return "absolute"; }



    };
}


#endif
