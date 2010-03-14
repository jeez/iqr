<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="text" standalone="yes" indent="yes"/>
<xsl:strip-space elements="*"/>



<xsl:template match="/System">

  <xsl:text>     </xsl:text><xsl:value-of select="Note"/><xsl:text>
</xsl:text>


</xsl:template>


</xsl:stylesheet>
