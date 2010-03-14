<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/XSL/Transform/1.0"  xsl:version="1.0">
<xsl:output method="text" standalone="yes" indent="yes"/> 
<xsl:strip-space elements="*"/>
<xsl:template match="/">
\begin{compacttreelist}
\item System
\begin{compacttreelist}
    \item name: <xsl:value-of select="/ProcessSimulation/@name"/>
    \item id: <xsl:value-of select="ProcessSimulation/@id"/>
\end{compacttreelist}
<xsl:for-each select="/ProcessSimulation/Group">
\begin{compacttreelist}
\item Group
    \begin{compacttreelist}
    \item name: <xsl:value-of select="@name"/>
    \item id: <xsl:value-of select="@id"/>
<xsl:if test="Geometry/Sparse">
    \item Geometry: Sparse
    \begin{compacttreelist}
    <xsl:for-each select="/ProcessSimulation/Group/Geometry/Sparse/Point">
        \item point: (<xsl:value-of select="@x"/>, <xsl:value-of select="@y"/>)
    </xsl:for-each>
    \end{compacttreelist}
</xsl:if>
<xsl:if test="Geometry/Hex">
    \item Geometry: Hex
    \begin{compacttreelist}
        \item height: <xsl:value-of select="Geometry/Hex/@height"/>
        \item width: <xsl:value-of select="Geometry/Rect/@width"/>
        \item offset: <xsl:value-of select="Geometry/Hex/@offset"/>
        \item orientation: <xsl:value-of select="Geometry/Hex/@orientation"/>
     \end{compacttreelist}
</xsl:if>
<xsl:if test="Geometry/Rect">
    \item Geometry: Rect
    \begin{compacttreelist}
        \item height: <xsl:value-of select="Geometry/Rect/@height"/>
        \item width: <xsl:value-of select="Geometry/Rect/@width"/>
    \end{compacttreelist}
</xsl:if>
<xsl:for-each select="/ProcessSimulation/Group/ConnectionTo">
    \item ConnectionTo
    \begin{compacttreelist}
        \item id: <xsl:value-of select="@id"/>
        \item target: <xsl:value-of select="@target"/>
    \end{compacttreelist}
</xsl:for-each>
<!-- Neuron: <xsl:value-of select="NeuronTanH/@test"/>
--> 
<xsl:if test="NeuronLinearThreshold">
    \item Neuron Type: NeuronLinearThreshold
</xsl:if>
<xsl:if test="NeuronTanH">
    \item Neuron Type: NeuronTanH
</xsl:if>
<xsl:if test="NeuronIntegFire">
    \item Neuron Type: NeuronIntegFire
</xsl:if>
<xsl:if test="NeuronSigmoid">
    \item Neuron Type: NeuronSigmoid
</xsl:if>
<xsl:if test="NeuronRandomSpike">
    \item Neuron Type: NeuronRandomSpike
</xsl:if>
<xsl:if test="NeuronUserDef">
    \item Neuron Type: NeuronUserDef
</xsl:if>
\end{compacttreelist}
\end{compacttreelist}
</xsl:for-each>
\end{compacttreelist}
</xsl:template>
</xsl:stylesheet>