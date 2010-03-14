<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html"  version="3.2" encoding="iso-8859-1"/>

<xsl:template match="/">

  <h2><xsl:value-of select="/System/@name"/></h2>
  <hr/>
    
  <xsl:value-of select="/System/@name"/><br/>
  <xsl:for-each select="//Process|//Group">
    
    <xsl:if test="count(ancestor::*) = 1"> 
    <img src="child1.png"/><xsl:value-of select="@name"/><br/>
  </xsl:if> 
  
  <xsl:if test="count(ancestor::*) = 2"> 
  <img src="child2.png"/><xsl:value-of select="@name"/><br/>
</xsl:if> 
</xsl:for-each>


<hr/>

<xsl:for-each select="//Connection">
  <img src="child1.png"/><xsl:value-of select="@name"/> (
  <xsl:variable name="target" select="@target"/>
  <xsl:variable name="source" select="@source"/>

  <xsl:for-each select="//Group">
    <xsl:if test="@id=$source"> 
    <xsl:value-of select="@name"/><img src="connect.png"/>
  </xsl:if>
</xsl:for-each>

<xsl:for-each select="//Group">
  <xsl:if test="@id=$target"> 
  <xsl:value-of select="@name"/>)<br/>
</xsl:if>
</xsl:for-each>

</xsl:for-each>


</xsl:template> 


</xsl:stylesheet>