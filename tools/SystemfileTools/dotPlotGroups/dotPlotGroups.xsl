<?xml version='1.0'?>
<!-- <xsl:stylesheet xmlns:xsl="http://www.w3.org/XSL/Transform/1.0"  xsl:version="1.0">-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="text" standalone="yes" indent="yes"/>
<xsl:strip-space elements="*"/>



<xsl:template match="/"><!--
-->digraph <xsl:value-of select="translate(/System/@name,' ','_')"/> {
	orientation=portrait;
	center=true;
	color=black;
	label = "<xsl:value-of select="/System/@name"/>";
        fontname=Helvetica;
	ratio=auto;
<xsl:for-each select="/System/Process">
     subgraph cluster<xsl:value-of select="translate(./@id,'-','')"/>{
         node [style=filled,shape=box];
         fontsize=16;
         fontname=Helvetica;
         label ="<xsl:value-of select="./@name"/>";
<xsl:for-each select="./Group">
  
  <xsl:choose>
    <xsl:when test="Topology/TopologyRect">
      <xsl:variable name="vcount">
        <xsl:value-of select="Topology/TopologyRect/@vcount"/>
      </xsl:variable>
      
      <xsl:variable name="hcount">
        <xsl:value-of select="Topology/TopologyRect/@hcount"/>
      </xsl:variable>
      <xsl:variable name="nrcells" select="$vcount * $hcount"/>
      <xsl:text>         </xsl:text><xsl:value-of select="translate(@id,'-','')"/> [label="<xsl:value-of select="@name"/> (<xsl:value-of select="$nrcells"/>)",fontname=Helvetica];
    </xsl:when>
    <xsl:otherwise>
      <xsl:text>         </xsl:text><xsl:value-of select="translate(@id,'-','')"/> [label="<xsl:value-of select="@name"/>",fontname=Helvetica];
    <!-- -->
  </xsl:otherwise>
</xsl:choose>







</xsl:for-each><!--
-->    }
</xsl:for-each>

<xsl:for-each select="/System/Connection">
      <xsl:variable name="thissource" select="@source"/>
      <xsl:variable name="thistarget" select="@target"/>
<xsl:for-each select="/System/Process/Group">
<xsl:if test="$thissource=@id">
<xsl:value-of select="translate(@id,'-','')"/>
</xsl:if>
</xsl:for-each><!--
-->-><!--
--><xsl:for-each select="/System/Process/Group">
<xsl:if test="$thistarget=@id">
<xsl:value-of select="translate(@id,'-','')"/>
</xsl:if>
</xsl:for-each>
<xsl:if test="@type='excitatory'"> [color=red, style="solid"];</xsl:if>
<xsl:if test="@type='inhibitory'"> [color=blue, style="dashed"];</xsl:if>
<xsl:if test="@type='modulatory'"> [color=green, style="dotted"];</xsl:if>
<xsl:text>
</xsl:text>
</xsl:for-each>
}

</xsl:template>


</xsl:stylesheet>
