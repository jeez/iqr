<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" indent="yes"/>
  <xsl:output doctype-system="iqr421_System_0303030204030201.dtd"/>
  <xsl:output omit-xml-declaration="no"/>
  
  
  <xsl:template match="*">
    <xsl:variable name="varspaces" select="substring($spaces,1,count(ancestor::node()) *$indent)"/>
    <xsl:value-of select="$varspaces" />

      <xsl:copy>
        <xsl:copy-of select="@*" />
          <xsl:choose>
            <xsl:when test="*">
              <xsl:apply-templates />
                <xsl:value-of select="concat('&#x0A;   ', $varspaces)" />
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="normalize-space()"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:copy>

          </xsl:template>
        </xsl:stylesheet>
        
