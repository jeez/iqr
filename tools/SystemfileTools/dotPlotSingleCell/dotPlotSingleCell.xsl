<?xml version='1.0'?>
<!-- <xsl:stylesheet xmlns:xsl="http://www.w3.org/XSL/Transform/1.0"  xsl:version="1.0">-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="text" standalone="yes" indent="no"/>
<!-- <xsl:strip-space elements="*"/>
<xsl:preserve-space elements="*"/> -->

<xsl:template name="rowloop">
 <xsl:param name="rowcounter">0</xsl:param>
 <xsl:param name="height">0</xsl:param>
 <xsl:param name="offset">0</xsl:param>
 <!-- <xsl:value-of select="$rowcounter"/> -->
 <xsl:if test="number($rowcounter) &lt; $height"> &lt;cell<xsl:value-of select="$rowcounter+$offset"/>&gt; <!--<xsl:value-of select="$rowcounter+$offset"/>--><xsl:if test="number($rowcounter)  &lt; $height -1">|</xsl:if> <xsl:call-template name="rowloop">
   <xsl:with-param name="rowcounter" select="$rowcounter + 1"/>
   <xsl:with-param name="offset" select="$offset"/>
   <xsl:with-param name="height" select="$height"/>
  </xsl:call-template>
 </xsl:if>
</xsl:template>


<xsl:template name="colloop">
  <xsl:param name="height">0</xsl:param>
  <xsl:param name="colcounter">0</xsl:param>
  <xsl:param name="width">0</xsl:param>
  <!--   <xsl:value-of select="$colcounter"/> -->
  <xsl:if test="number($colcounter) &lt; $width"><!--
    -->{<xsl:call-template name="rowloop">
    <xsl:with-param name="height" select="$height"/>
    <xsl:with-param name="offset" select="$colcounter * $height"/>
  </xsl:call-template>}<xsl:if test="number($colcounter)  &lt; $width -1">|</xsl:if>
  <xsl:call-template name="colloop"><xsl:with-param name="colcounter" select="$colcounter + 1"/><xsl:with-param name="height" select="$height"/> <xsl:with-param name="width" select="$width"/> </xsl:call-template>
</xsl:if>
</xsl:template>

<xsl:template match="/"><!--
-->digraph <xsl:value-of select="translate(/AERExportSystem/@name,' ','_')"/> {
	orientation=portrait;
	center=true;
	color=black;
	label = "<xsl:value-of select="/AERExportSystem/@name"/>";
        fontname=Helvetica;
        fontsize="10";
	ratio=auto;
<xsl:for-each select="/AERExportSystem/Process">
     subgraph cluster<xsl:value-of select="translate(./@id,'-','')"/>{
         node [style=filled,shape=box];
         fontsize=10;
         fontname=Helvetica;
         label ="<xsl:value-of select="./@name"/>";

<xsl:for-each select="./Group">

             subgraph cluster<xsl:value-of select="translate(./@id,'-','')"/>{
<xsl:text>             </xsl:text>label="<xsl:value-of select="@name"/>";
<xsl:variable name="hcount"><xsl:value-of select="./Topology/TopologyRect/@hcount"/></xsl:variable>
<xsl:variable name="vcount"><xsl:value-of select="./Topology/TopologyRect/@vcount"/></xsl:variable>
             node<xsl:value-of select="translate(@id,'-','')"/>[shape=record,label =&quot;{<xsl:call-template name="colloop">
<xsl:with-param name="height" select="$hcount"/> 
<xsl:with-param name="colcounter" select="0"/> 
<xsl:with-param name="width" select="$vcount"/> 
</xsl:call-template>}&quot;, fontsize=8, fontname=Helvetica, width="<xsl:value-of select="$hcount*.2"/>", height="<xsl:value-of select="$vcount*.2"/>"];
             }</xsl:for-each>
    }
    
</xsl:for-each>



<xsl:for-each select="/AERExportSystem/AERConnection">

      <xsl:variable name="thissource" select="translate(./@source,'-','')"/>
      <xsl:variable name="thistarget" select="translate(./@target,'-','')"/>



      <xsl:variable name="thistype" select="@type"/>
      <xsl:for-each select="./pre">
        <xsl:variable name="preindex" select="./@index"/>
      <xsl:for-each select="./post">
        <xsl:variable name="postindex" select="./@index"/>
        "node<xsl:value-of select="$thissource"/>":cell<xsl:value-of select="$preindex"/>->"node<xsl:value-of select="$thistarget"/>":cell<xsl:value-of select="$postindex"/><xsl:if test="$thistype='excitatory'"> [arrowsize=".5", arrowhead="odot", color=red, style="solid"];</xsl:if>
<xsl:if test="$thistype='inhibitory'"> [arrowsize=".5", arrowhead="dot", color=blue, style="solid"];</xsl:if>
<xsl:if test="$thistype='modulatory'"> [arrowsize=".5", arrowhead="inv", color=green, style="solid"];</xsl:if>
      </xsl:for-each>
      </xsl:for-each>
</xsl:for-each>
}

</xsl:template>


</xsl:stylesheet>
