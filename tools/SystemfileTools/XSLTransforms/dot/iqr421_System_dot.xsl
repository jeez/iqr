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
     subgraph cluster<xsl:value-of select="translate(./@name,' ','_')"/>{
         node [style=filled,shape=box];
         fontsize=16;
         fontname=Helvetica;
         label ="<xsl:value-of select="./@name"/>";
<xsl:for-each select="./Group">
<xsl:text>         </xsl:text><xsl:value-of select="translate(@name,' ','_')"/> [label="<xsl:value-of select="@name"/>",fontname=Helvetica];
<!-- --></xsl:for-each><!--
-->    }
</xsl:for-each>

<xsl:for-each select="/System/Connection">
      <xsl:variable name="thissource" select="@source"/>
      <xsl:variable name="thistarget" select="@target"/>
<xsl:for-each select="/System/Process/Group">
<xsl:if test="$thissource=@id">
<xsl:value-of select="translate(@name,' ','_')"/>
</xsl:if>
</xsl:for-each><!--
-->-><!--
--><xsl:for-each select="/System/Process/Group">
<xsl:if test="$thistarget=@id">
<xsl:value-of select="translate(@name,' ','_')"/>
</xsl:if>
</xsl:for-each>;
</xsl:for-each>
}

</xsl:template>


</xsl:stylesheet>
