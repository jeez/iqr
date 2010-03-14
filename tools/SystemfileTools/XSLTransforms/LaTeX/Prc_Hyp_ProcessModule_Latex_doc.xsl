<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/XSL/Transform/1.0"  xsl:version="1.0">
<xsl:output method="text" standalone="yes" indent="yes"/>
<xsl:strip-space elements="*"/>
<xsl:template match="/"><!--
-->\documentclass[a4paper, 11pt]{article}
\usepackage[T1]{fontenc}
\usepackage[english]{babel} %das Letzte ist default
\usepackage{paralist}
\usepackage{treelist}

\selectlanguage{english}


\begin{document}







\section{System <xsl:value-of select="/ProcessModule/@name"/>}

filename: <xsl:value-of select="/ProcessModule/Module/@filename"/>
name: <xsl:value-of select="/ProcessModule/Module/@name"/>

<!-- <Module 
filename="defaultCDATA22" 
name="defaultCDATA23" -->

<xsl:for-each select="/ProcessModule/Group">
\subsection{Group <xsl:value-of select="@name"/>}
\label{sec:Group_<xsl:value-of select="@id"/>}

\subsubsection*{Geometry}
<xsl:if test="Geometry/Sparse">
The geometry of group <xsl:value-of select="./@name"/> is \emph{sparse}, and defined
by the following points:
\begin{compactitem}<!--
--><xsl:for-each select="/ProcessModule/Group/Geometry/Sparse/Point">
   \item point: (<xsl:value-of select="@x"/>, <xsl:value-of select="@y"/>)<!--
--></xsl:for-each>
\end{compactitem}
</xsl:if>
<xsl:if test="Geometry/Hex">
The geometry of group <xsl:value-of select="./@name"/> is \emph{hexagonal}, with
the following parameters
\begin{compactitem}
    \item height: <xsl:value-of select="Geometry/Hex/@height"/>
    \item width: <xsl:value-of select="Geometry/Hex/@width"/>
    \item offset: <xsl:value-of select="Geometry/Hex/@offset"/>
    \item orientation: <xsl:value-of select="Geometry/Hex/@orientation"/>
\end{compactitem}
</xsl:if>
<xsl:if test="Geometry/Rect">
The geometry of group <xsl:value-of select="./@name"/> is a \emph{rectangle} with
$height= <xsl:value-of select="Geometry/Rect/@height"/>$, and $width= <xsl:value-of select="Geometry/Rect/@width"/>$.
</xsl:if><!--
 -->
\subsubsection*{Cell Type}
<xsl:if test="NeuronLinearThreshold"><!--
 -->The cell type of the group is \emph{"Linear Threshold"}.<!--
 --></xsl:if>
<xsl:if test="NeuronTanH"><!--
 -->The cell type of the group is \emph{"TanH"}.<!--
 --></xsl:if>
<xsl:if test="NeuronIntegFire"><!--
 -->The cell type of the group is \emph{"Integrate and Fire"}.<!--
 --></xsl:if>
<xsl:if test="NeuronSigmoid"><!--
 -->The cell type of the group is \emph{"Sigmoid"}.<!--
 --></xsl:if>
<xsl:if test="NeuronRandomSpike"><!--
 -->The cell type of the group is \emph{"Random Spike"}.<!--
 --></xsl:if>
<xsl:if test="NeuronUserDef"><!--
 -->The cell type of the group is \emph{"User Defined"}.<!--
 --></xsl:if>
<!--

 -->
\subsubsection*{Connections to other groups}
\begin{compactenum}<!--
 --><xsl:for-each select="/ProcessModule/Group/ConnectionTo">
<xsl:variable name="thistarget" select="@target"/>
<xsl:for-each select="/ProcessModule/Group">
<xsl:if test="$thistarget=@id">
\item to group <xsl:value-of select="@name"/> (see \ref{sec:Group_<xsl:value-of select="@id"/>})<!--
 --></xsl:if></xsl:for-each></xsl:for-each>
\end{compactenum}
</xsl:for-each>

\end{document}


%%% Local Variables:
%%% mode: latex
%%% TeX-master: t
%%% End:

</xsl:template>


</xsl:stylesheet>