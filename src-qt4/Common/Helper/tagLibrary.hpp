/****************************************************************************
 ** $Filename: tagLibrary.hpp
 **
 ** $Author: Mark Blanchard
 **
 ** $Description: Common string constants for handling system files.
 **               These constants must match the DTD.
 **
 *****************************************************************************/

#ifndef TAGLIBRARY_HPP
#define TAGLIBRARY_HPP

namespace iqrcommon {

    class ClsTagLibrary
    {
    public:
	static const char* SystemTag()  {
	    return "System";
	}
	static const char* ProcessTag() {
	    return "Process";
	}
	static const char* ModuleTag() {
	    return "Module";
	}
	static const char* GroupTag() {
	    return "Group";
	}

	static const char* ModuleInputFromGroup() {
	    return "InputFromGroup";
	}

	static const char* ModuleOutputToGroup() {
	    return "OutputToGroup";
	}

	static const char* GeometryTag() {
	    return "Geometry";
	}

	static const char* FeedbackTag() {
	    return "Freedback";
	}

	static const char* AERSystemTag()  {
	    return "AERExportSystem";
	}

	static const char* RegularGeometryTag() {
	    return "GeometryRegular";
	}
	static const char* CustomGeometryTag() {
	    return "GeometryCustom";
	}

	static const char* ParameterTag() {
	    return "Parameter";
	}

	static const char* XRefHolderTag() {
	    return "XRefHolder";
	}

	static const char* XRefTag() {
	    return "XRef";
	}

	

// Transistion to Topology ----------

	static const char* TopologyTag() {
	    return "Topology";
	}

	static const char* RectTopologyTag() {
	    return "TopologyRect";
	}

	static const char* SparseTopologyTag() {
	    return "TopologySparse";
	}

	static const char* HexTopologyTag() {
	    return "TopologyHex";
	}

	static const char* CustomTopologyTag() {
	    return "TopologyCustom";
	}

	static const char* TopologyWidthTag() {
	    return "hcount";
	}
	static const char* TopologyHeightTag() {
	    return "vcount";
	}
	static const char* TopologyOrientationTag() {
	    return "orientation";
	}

	static const char* TopologyOrientationTagHorizontal() {
	    return "horizontal";
	}

	static const char* TopologyOrientationTagVertical() {
	    return "vertical";
	}

	static const char* TopologyOffsetTag() {
	    return "offset";
	}

	static const char* TopologyCylinderVertical() {
	    return "cylinderVert";
	}

	static const char* TopologyCylinderHorizontal() {
	    return "cylinderHor";
	}

//-----------------------------------

	static const char* NeuronTag() {
	    return "Neuron";
	}

	static const char* UserDefinedNeuronTag() {
	    return "NeuronUserDefined";
	}

	static const char* ConnectionTag() {
	    return "Connection";
	}

	static const char* GroupsWithConnectionsTag() {
	    return "GroupsWithConnections";
	}

	static const char* AERConnectionTag() {
	    return "AERConnection";
	}

	static const char* NotesTag() {
	    return "Note";
	}

	static const char* DiagramIconTag() {
	    return "DiagramIcon";
	}
	static const char* DiagramLineTag() {
	    return "DiagramLine";
	}

	static const char* DiagramNoteTag() {
	    return "DiagramNote";
	}

	static const char* DiagramLinePointTag() {
	    return "Point";
	}

	static const char* DiagramLineAPSourceTag() {
	    return "APSource";
	}

	static const char* DiagramLineAPTargetTag() {
	    return "APTarget";
	}

	// Get attribute tags.
	static const char* IDTag() {
	    return "id";
	}
	static const char* NameTag() {
	    return "name";
	}

	static const char* ValueTag() {
	    return "value";
	}

	static const char* TargetTag() {
	    return "target";
	}

	static const char* AuthorTag() {
	    return "author";
	}
	static const char* DateTag() {
	    return "date";
	}
	static const char* PortTag() {
	    return "port";
	}

	static const char* EnableModuleTag() {
	    return "enableModule";
	}

	static const char* PathTag() {
	    return "path";
	}

	static const char* CyclesPerSecondTag() {
	    return "cylesPerSecond";
	}

	static const char* SyncPlotsTag() {
	    return "syncPlots";
	}

	static const char* HostnameTag() {
	    return "hostname";
	}

	static const char* ColorTag() {
	    return "color";
	}

	static const char* ProcessEnabledTag() {
	    return "enabled";
	}

	static const char* GeometryWidthTag() {
	    return "width";
	}
	static const char* GeometryHeightTag() {
	    return "height";
	}
	static const char* GeometryOrientationTag() {
	    return "orientation";
	}
	static const char* GeometryOffsetTag() {
	    return "offset";
	}

	static const char* NeuronExcitatoryGainTag() {
	    return "excGain";
	}
	static const char* NeuronInhibitoryGainTag() {
	    return "inhGain";
	}
	static const char* NeuronModulatoryGainTag() {
	    return "modGain";
	}
	static const char* NeuronMembranePersistenceTag() {
	    return "vmPrs";
	}
	static const char* NeuronThresholdTag() {
	    return "threshold";
	}
	static const char* NeuronSpikeAmplitudeTag() {
	    return "spkAmpl";
	}
	static const char* NeuronClipPotentialTag() {
	    return "clipVm";
	}
	static const char* NeuronMinimumPotentialTag() {
	    return "vmMin";
	}
	static const char* NeuronMaximumPotentialTag() {
	    return "vmMax";
	}
	static const char* NeuronSlopeTag() {
	    return "slope";
	}
	static const char* NeuronProbabilityTag() {
	    return "probability";
	}
	static const char* NeuronMembraneResetTag() {
	    return "vmReset";
	}
	static const char* NeuronAdaptThresholdTag() {
	    return "adaptTh";
	}
	static const char* NeuronThresholdIncrementTag() {
	    return "thInc";
	}
	static const char* NeuronThresholdPersistenceTag() {
	    return "thPrs";
	}
	static const char* NeuronMidpointPotentialTag() {
	    return "vmMid";
	}

	static const char* DiagramIconXTag() {
	    return "x";
	}
	static const char* DiagramIconYTag() {
	    return "y";
	}
	static const char* DiagramIconWidthTag() {
	    return "width";
	}
	static const char* DiagramIconHeightTag() {
	    return "height";
	}

	static const char* DiagramLinePointXTag() {
	    return "x";
	}
	static const char* DiagramLinePointYTag() {
	    return "y";
	}

	static const char* ConnectionSourceTag() {
	    return "source";
	}
	static const char* ConnectionTargetTag() {
	    return "target";
	}

/* OBSOLETE: */
	static const char* ConnectionSourcePattern() {
	    return "SourcePattern";
	}
	static const char* ConnectionTargetPattern() {
	    return "TargetPattern";
	}
/* ----------------- */

	static const char* ConnectionPattern() {
	    return "Pattern";
	}

	static const char* ConnectionPatternMapped() {
	    return "PatternMapped";
	}

	static const char* ConnectionPatternForeach() {
	    return "PatternForeach";
	}

	static const char* ConnectionPatternTuples() {
	    return "PatternTuples";
	}

	static const char* SelectorAll() {
	    return "All";
	}

	static const char* SelectorRegion() {
	    return "Region";
	}


	static const char* RegionXStart() {
	    return "xstart";
	}

	static const char* RegionYStart() {
	    return "ystart";
	}

	static const char* RegionWidth() {
	    return "width";
	}

	static const char* RegionHeight() {
	    return "height";
	}

	static const char* Tuple() {
	    return "Tuple";
	}

	static const char* SourceList() {
	    return "SourceList";
	}

	static const char* TargetList() {
	    return "TargetList";
	}

	static const char* Point() {
	    return "Point";
	}

	static const char* PointX() {
	    return "x";
	}
	static const char* PointY() {
	    return "y";
	}

	static const char* pre() {
	    return "pre";
	}

	static const char* post() {
	    return "post";
	}

	static const char* index() {
	    return "index";
	}


	static const char* synapse_index() {
	    return "synapse_index";
	}

	static const char* delay() {
	    return "delay";
	}

	static const char* SelectorList() {
	    return "List";
	}


	static const char* SelectorTuples() {
	    return "Tuples";
	}


	static const char* ConnectionPatternMappedType() {
	    return "type";
	}

	static const char* ConnectionPatternMappedTypeAll() {
	    return "all";
	}

	static const char* ConnectionPatternMappedTypeCenter() {
	    return "center";
	}

/* ----------------------- */

	static const char* SynapseTag() {
	    return "Synapse";
	}

	static const char* ConnectionArborization() {
	    return "Arborization";
	}

	static const char* ConnectionArborizationRFTag() {
	    return "RF";
	}

	static const char* ConnectionArborizationPFTag() {
	    return "PF";
	}

	static const char* ConnectionArborizationEllipse() {
	    return "ArbEllipse";
	}

	static const char* ConnectionArborizationRectangular() {
	    return "ArbRect";
	}
	static const char* ConnectionArborizationEllipseWindow() {
	    return "ArbEllipseWindow";
	}
	static const char* ConnectionArborizationRectangularWindow() {
	    return "ArbRectWindow";
	}
	static const char* ConnectionArborizationRandom() {
	    return "ArbRandom";
	}
	static const char* ConnectionArborizationAll() {
	    return "ArbAll";
	}

	static const char* ArborizationInitializationProbabilityTag() {
	    return "initialization_probability";
	}

	static const char* ArborizationSelfConnectTag() {
	    return "self_connect";
	}

	static const char* ArborizationDirectionTag() {
	    return "direction";
	}


	static const char* ArborizationWidthTag() {
	    return "width";
	}
	static const char* ArborizationHeightTag() {
	    return "height";
	}

	static const char* ArborizationInnerWidthTag() {
	    return "innerwidth";
	}
	static const char* ArborizationInnerHeightTag() {
	    return "innerheight";
	}

	static const char* ArborizationOuterWidthTag() {
	    return "outerwidth";
	}

	static const char* ArborizationOuterHeightTag() {
	    return "outerheight";
	}

	static const char* ArborizationCountTag() {
	    return "count";
	}




	static const char* ConnectionAttenuationFunction() {
	    return "AttenuationFunction";
	}

	static const char* ConnectionDelayFunction() {
	    return "DelayFunction";
	}

	static const char* ConnectionFunctionGaussian() {
	    return "FunGaussian";
	}

	static const char* ConnectionFunctionLinear() {
	    return "FunLinear";
	}
	static const char* ConnectionFunctionBlock() {
	    return "FunBlock";
	}
	static const char* ConnectionFunctionRandom() {
	    return "FunRandom";
	}
	static const char* ConnectionFunctionUniform() {
	    return "FunUniform";
	}
	static const char* ConnectionFunctionKernel() {
	    return "FunKernel";
	}


	static const char* ConnectionFunctionArgMin() {
	    return "min";
	}

	static const char* ConnectionFunctionArgMax() {
	    return "max";
	}

	static const char* ConnectionFunctionArgSigma() {
	    return "sigma";
	}

	static const char* ConnectionFunctionArgWidth() {
	    return "width";
	}

	static const char* ConnectionFunctionArgHeight() {
	    return "height";
	}

	static const char* ConnectionFunctionArgProbability() {
	    return "probability";
	}

	static const char* ConnectionFunctionArgValue() {
	    return "value";
	}

	static const char* ConnectionTypeTag() {
	    return "type";
	}
	static const char* ExcitatoryConnectionTag() {
	    return "excitatory";
	}
	static const char* InhibitoryConnectionTag() {
	    return "inhibitory";
	}
	static const char* ModulatoryConnectionTag() {
	    return "modulatory";
	}

	/* new stuff from Dannys code */
	static const char* ForgettingRateTag() {
	    return "forgettingRate";
	}

	static const char* LearningRateTag() {
	    return "learningRate";
	}

	static const char* MaxWeightTag() {
	    return "maxWeight";
	}

	static const char* MinWeightTag() {
	    return "minWeight";
	}

	static const char* TimeWindowSizeTag() {
	    return "timeWindowsSize";
	}

	static const char* FeedbackThresholdTag() {
	    return "feedbackThreshold";
	}
	/* -------------------------- */

	static const char* iqrSettings() {
	    return "iqrSettings";
	}

	static const char* lastFilesOpen() {
	    return "lastFilesOpen";
	}

	static const char* FileTag() {
	    return "File";
	}


	static const char* LocalModulePath() {
	    return "LocalModulePath";
	}

	static const char* LocalNeuronPath() {
	    return "LocalNeuronPath";
	}

	static const char* LocalSynapsePath() {
	    return "LocalSynapsePath";
	}

	static const char* StandardModulePath() {
	    return "StandardModulePath";
	}

	static const char* StandardNeuronPath() {
	    return "StandardNeuronPath";
	}

	static const char* StandardSynapsePath() {
	    return "StandardSynapsePath";
	}

	static const char* UserModulePath() {
	    return "UserModulePath";
	}

	static const char* UserNeuronPath() {
	    return "UserNeuronPath";
	}

	static const char* UserSynapsePath() {
	    return "UserSynapsePath";
	}


	static const char* UseLocalNeurons(){
	    return "UseLocalNeurons";
	}


	static const char* UseUserNeurons(){
	    return "UseUserNeurons";
	}

	static const char* UseLocalSynapses(){
	    return "UseLocalSynapses";
	}

	static const char* UseUserSynapses(){
	    return "UseUserSynapses";
	}

	static const char* UseLocalModules(){
	    return "UseLocalModules";
	}

	static const char* UseUserModules(){
	    return "UseUserModules";
	}

	static const char* iqrEntityTag(){
	    return "iqrEntity";
	}

	static const char* ClipboardContentsType(){
	    return "type";
	}

	static const char* DataManagerConfiguration() {
	    return "iqrDataManagerConfiguration";
	}

	static const char* PlotTypeGroup(){
	    return "PlotTypeGroup";
	}

	static const char* PlotTypeTime(){
	    return "PlotTypeTime";
	}

	static const char* PlotTypeCorrelation(){
	    return "PlotTypeCorrelation";
	}

	static const char* DataSampler(){
	    return "DataSampler";
	}

	static const char* AutoSaveInterval(){
	    return "AutoSaveInterval";
	}

	static const char* BlockDiagramFontName(){
	    return "BlockDiagramFontName";
	}

	static const char* BlockDiagramFontSize(){
	    return "BlockDiagramFontSize";
	}


    };
}

#endif
