<?xml version='1.0'?>
<!-- <xsl:stylesheet xmlns:xsl="http://www.w3.org/XSL/Transform/1.0"  xsl:version="1.0">-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="text" standalone="yes" indent="yes"/>
<xsl:strip-space elements="*"/>



<xsl:template match="/"><!--
-->digraph <xsl:value-of select="/System/@name"/> {
	orientation=portrait;
	center=true;
	color=black;
	label = "<xsl:value-of select="/System/@name"/>";
        fontname=Helvetica;
	ratio=auto;
<xsl:for-each select="/System/ProcessSimulation|/System/ProcessModule">
     subgraph cluster<xsl:value-of select="./@name"/> {
         node [style=filled,shape=box];
         fontsize=16;
         fontname=Helvetica;
         label ="<xsl:value-of select="./@name"/> [<xsl:value-of select="./@id"/>]";
<xsl:for-each select="./Group">
<xsl:text>         </xsl:text><xsl:value-of select="@name"/>[label="<xsl:value-of select="@name"/>\n[<xsl:value-of select="@id"/>]",fontname=Helvetica];
<!-- --></xsl:for-each><!--
-->    }
</xsl:for-each>


<xsl:for-each select="/System/ProcessSimulation|/System/ProcessModule">

<xsl:for-each select="./Group">
<xsl:variable name="myname" select="@name"/>
<xsl:variable name="myid" select="@id"/><!--
--><xsl:if test="ConnectionTo"><!--
--><xsl:for-each select="ConnectionTo"><!--
--><xsl:variable name="ConnID" select="@id"/><!--
--><xsl:variable name="thistarget" select="@target"/><!--
--><xsl:for-each select="/System/ProcessSimulation/Group|/System/ProcessModule/Group"><!--
--><xsl:if test="$thistarget=@id">
<!-- <xsl:text>         </xsl:text><xsl:value-of select="$myname"/>-><xsl:value-of select="@name"/>[label="<xsl:value-of select="$ConnID"/> ",fontname=Helvetica,fontsize=8];-->
<xsl:text>         </xsl:text><xsl:value-of select="$myname"/>-><xsl:value-of select="@name"/>[label="<xsl:value-of select="$ConnID"/> (<xsl:value-of select="$myid"/>-><xsl:value-of select="@id"/>)",fontname=Helvetica,fontsize=10,minlen=3];
<!--
--></xsl:if></xsl:for-each></xsl:for-each><!--
--></xsl:if><!--
--></xsl:for-each><!--
-->
</xsl:for-each>
}


</xsl:template>


</xsl:stylesheet>