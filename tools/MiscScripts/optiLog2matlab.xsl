<xsl:stylesheet version="1.0"   xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="text"  version="3.2" encoding="iso-8859-1" indent="yes"/>
<xsl:strip-space elements="*"/>


<xsl:template match="ParamSet">
  <xsl:variable name="ppp"><xsl:value-of select="position()"/></xsl:variable>

 
  <xsl:for-each select="./Parameter">
    <xsl:variable name="oldName"><xsl:value-of select="@name"/></xsl:variable>
    <xsl:variable name="newName"><xsl:value-of select="translate($oldName, ' -|', '___')"/></xsl:variable>

    <xsl:if test="$ppp=1">parameters(<xsl:value-of select="position()"/>,1) = {'<xsl:value-of select="$newName"/>'};
  </xsl:if>
    

    <xsl:value-of select="$newName"/><xsl:text>(</xsl:text><xsl:value-of select="$ppp"/><xsl:text>,1)=</xsl:text><xsl:value-of select="@value"/><xsl:text>;
  </xsl:text>
  </xsl:for-each>
  <xsl:text>error(</xsl:text><xsl:value-of select="$ppp"/><xsl:text>,1)=</xsl:text><xsl:value-of select="@error"/><xsl:text>;
  </xsl:text>

</xsl:template>




</xsl:stylesheet>


