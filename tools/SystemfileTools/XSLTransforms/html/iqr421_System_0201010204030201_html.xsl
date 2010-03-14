<?xml version='1.0'?>
<!-- <xsl:stylesheet xmlns:xsl="http://www.w3.org/XSL/Transform/1.0"  xsl:version="1.0">-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html"  version="4.0"/>
<xsl:strip-space elements="*"/>

<xsl:template match="/">

<html>
<head>
<TITLE>System <xsl:value-of select="/System/@name"/></TITLE>
</head>
<body>

<h1>System <xsl:value-of select="/System/@name"/></h1>
<hr/>
<center><img src="{/System/@name}.gif"/></center>
<hr/>
<h1>Processes</h1>
<xsl:for-each select="/System/ProcessSimulation|/System/ProcessModule">
<h2><xsl:number count="node()" format="1."/> Process <xsl:value-of select="./@name"/></h2>
<ul>
<xsl:if test="Module">
<h3>Module <xsl:value-of select="Module/@name"/></h3>
Process <xsl:value-of select="./@name"/> has a UserModule <em><xsl:value-of select="Module/@name"/></em> which resides in
<tt><xsl:value-of select="Module/@filename"/></tt>.
<xsl:if test="./Module/Parameter">
<p/>
<table border="0" cellpadding="5">
<tr>
<td></td><td><em>Name</em></td><td><em>Type</em></td><td><em>Value</em></td>
</tr>
<xsl:for-each select="./Module/Parameter">
<tr>
<td></td><td><xsl:value-of select="@name"/></td><td><xsl:value-of select="@type"/></td><td><xsl:value-of select="@value"/></td>
</tr>
</xsl:for-each>
</table>
</xsl:if>
</xsl:if>

<xsl:for-each select="./Group">
<h3><xsl:number format="1."/><a name="group_{@id}"> Group <xsl:value-of select="@name"/></a></h3>
<h4>Geometry</h4>
<xsl:if test="Geometry/Sparse">
 The geometry of group <xsl:value-of select="./@name"/> is <em>sparse</em>, and defined
by the following points:
<ul><!--
--><xsl:for-each select="./Geometry/Sparse/Point">
<li> point: (<xsl:value-of select="@x"/>, <xsl:value-of select="@y"/>)</li><!--
--></xsl:for-each>
</ul>
</xsl:if>
<xsl:if test="Geometry/Hex">
  The geometry of group <xsl:value-of select="./@name"/> is <em>hexagonal</em>, with
the following parameters
<ul>
  <li> height: <xsl:value-of select="Geometry/Hex/@height"/></li>
  <li> width: <xsl:value-of select="Geometry/Hex/@width"/></li>
  <li> offset: <xsl:value-of select="Geometry/Hex/@offset"/></li>
  <li> orientation: <xsl:value-of select="Geometry/Hex/@orientation"/></li>
</ul>
</xsl:if>
<xsl:if test="Geometry/Rect">
  The geometry of group <xsl:value-of select="./@name"/> is a <em>rectangle</em> with
height= <xsl:value-of select="Geometry/Rect/@height"/>, and width= <xsl:value-of select="Geometry/Rect/@width"/>.
</xsl:if>
<h4>Neuron Type</h4>
<xsl:if test="NeuronType/NeuronLinearThreshold">
The cell type of the group is <em>"Linear Threshold"</em> with:
<xsl:for-each select="NeuronType/NeuronLinearThreshold/@*">
  <xsl:value-of select="name()"/>: <xsl:value-of select="string()"/> <xsl:text> </xsl:text>
</xsl:for-each>
</xsl:if>
<xsl:if test="NeuronType/NeuronTanH">
The cell type of the group is <em>"TanH"</em> with:
<xsl:for-each select="NeuronType/NeuronTanH/@*">
  <xsl:value-of select="name()"/>: <xsl:value-of select="string()"/> <xsl:text> </xsl:text>
</xsl:for-each>
</xsl:if>
<xsl:if test="NeuronType/NeuronIntegFire">
The cell type of the group is <em>"Integrate and Fire"</em> with:
<xsl:for-each select="NeuronType/NeuronIntegFire/@*">
  <xsl:value-of select="name()"/>: <xsl:value-of select="string()"/> <xsl:text> </xsl:text>
</xsl:for-each>

</xsl:if>
<xsl:if test="NeuronType/NeuronSigmoid">
The cell type of the group is <em>"Sigmoid"</em> with:
<xsl:for-each select="NeuronType/NeuronSigmoid/@*">
  <xsl:value-of select="name()"/>: <xsl:value-of select="string()"/> <xsl:text> </xsl:text>
</xsl:for-each>

</xsl:if>
<xsl:if test="NeuronType/NeuronRandomSpike">
The cell type of the group is <em>"Random Spike"</em> with:
<xsl:for-each select="NeuronType/NeuronRandomSpike/@*">
  <xsl:value-of select="name()"/>: <xsl:value-of select="string()"/> <xsl:text> </xsl:text>
</xsl:for-each>

</xsl:if>
<xsl:if test="NeuronType/NeuronUserDef">
The cell type of the group is <em>"User Defined"</em> called <em><xsl:value-of select="./NeuronUserDef/@name"/></em>
and is loaded from <tt><xsl:value-of select="./NeuronUserDef/@filename"/></tt>.

<xsl:if test="./NeuronUserDef/Parameter">
<br/>The neuron has the following parameters:
<p/>
<table border="0" cellpadding="5">
<tr>
<td></td><td><em>Name</em></td><td><em>Type</em></td><td><em>Value</em></td>
</tr>
<xsl:for-each select="./NeuronUserDef/Parameter">
<tr>
<td></td><td><xsl:value-of select="@name"/></td><td><xsl:value-of select="@type"/></td><td><xsl:value-of select="@value"/></td>
</tr>
</xsl:for-each>
</table>
</xsl:if>
</xsl:if>
</xsl:for-each>
</ul>
</xsl:for-each>

<h1>Connections</h1>

<xsl:for-each select="/System/Connection">
  <h2>Connection <xsl:value-of select="./@id"/> from <xsl:value-of select="./@source"/> to <xsl:value-of select="./@target"/></h2>
  <ul>
  <xsl:for-each select="*">
    <li/><xsl:value-of select="name(.)"/><br/>
    <ul>
    <xsl:for-each select="*">
      <li/><xsl:value-of select="name(.)"/>
      <xsl:if test="@*"> ( <xsl:for-each select="@*">
      <xsl:value-of select="name()"/>: <xsl:value-of select="string()"/> <xsl:text> </xsl:text></xsl:for-each>)
      </xsl:if>
      <ul>
      <xsl:for-each select="*">
        <li/><xsl:value-of select="name(.)"/>
        <xsl:if test="@*"> ( <xsl:for-each select="@*">
        <xsl:value-of select="name()"/>: <xsl:value-of select="string()"/> <xsl:text> </xsl:text></xsl:for-each>)
        </xsl:if>
      </xsl:for-each>
      </ul>
    </xsl:for-each>
    </ul>
  </xsl:for-each>
  </ul>
</xsl:for-each>

</body>
</html>


</xsl:template>

</xsl:stylesheet>