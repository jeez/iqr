<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="xml" indent="yes"/>
<xsl:output doctype-system="iqr421_System_0303030204030201.dtd"/>
<!-- <xsl:output omit-xml-declaration="no"/> -->


<xsl:variable name="indent" select="3" />
  <xsl:variable name="spaces" select="'                 '" />
    
    <xsl:template match="*">
      <xsl:copy>	
      <xsl:copy-of select="@*" />
        <xsl:apply-templates />
          <xsl:variable name="varspaces" select="substring($spaces,1,count(ancestor::node()) *$indent)"/>
          <xsl:value-of select="$varspaces" />
          </xsl:copy>
          
          

<!--
<xsl:copy>
<xsl:apply-templates select="@*|node()"/>
</xsl:copy>
-->

<!--
<xsl:template match="*">
<xsl:copy>
<xsl:copy-of select="@*" />
<xsl:text>QQ</xsl:text>
<xsl:apply-templates/>
<xsl:text>ZZ</xsl:text>
</xsl:copy>
-->


                   <!--
<xsl:template match="/">

<xsl:for-each select="*"><xsl:text>&#9;</xsl:text><xsl:element name="{name()}">
<xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="string()"/></xsl:attribute></xsl:for-each>

<xsl:for-each select="*"><xsl:text>&#9;</xsl:text><xsl:element name="{name()}">
<xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="string()"/></xsl:attribute></xsl:for-each>

<xsl:for-each select="*"><xsl:text>&#9;</xsl:text><xsl:element name="{name()}">
<xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="string()"/></xsl:attribute></xsl:for-each>

<xsl:for-each select="*"><xsl:text>&#9;</xsl:text><xsl:element name="{name()}">
<xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="string()"/></xsl:attribute></xsl:for-each>

<xsl:for-each select="*"><xsl:text>&#9;</xsl:text><xsl:element name="{name()}">
<xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="string()"/></xsl:attribute></xsl:for-each>

<xsl:for-each select="*"><xsl:text>&#9;</xsl:text><xsl:element name="{name()}">
<xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="string()"/></xsl:attribute></xsl:for-each>

</xsl:element></xsl:for-each>
</xsl:element></xsl:for-each>
</xsl:element></xsl:for-each>
</xsl:element></xsl:for-each>
</xsl:element></xsl:for-each>
</xsl:element></xsl:for-each>

-->
<!--
<xsl:for-each select="*"><xsl:text>&#10;</xsl:text>
<xsl:if test="not(@*)"><xsl:element name="{name()}"/></xsl:if>
<xsl:if test="@*"><xsl:element name="{name()}">
<xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="string()"/></xsl:attribute></xsl:for-each>


    <xsl:for-each select="*"><xsl:text>&#10;&#9;</xsl:text>
    <xsl:if test="not(@*)"><xsl:element name="{name()}"/></xsl:if>
    <xsl:if test="@*"><xsl:element name="{name()}">
    <xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="string()"/></xsl:attribute></xsl:for-each>


        <xsl:for-each select="*"><xsl:text>&#10;&#9;&#9;</xsl:text>
        <xsl:if test="not(@*)"><xsl:element name="{name()}"/></xsl:if>
	<xsl:if test="@*"><xsl:element name="{name()}">
        <xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="string()"/></xsl:attribute></xsl:for-each>


            <xsl:for-each select="*"><xsl:text>&#10;&#9;&#9;&#9;</xsl:text>
	    <xsl:if test="not(@*)"><xsl:element name="{name()}"/></xsl:if>
	    <xsl:if test="@*"><xsl:element name="{name()}">
            <xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="string()"/></xsl:attribute></xsl:for-each>


                <xsl:for-each select="*"><xsl:text>&#10;&#9;&#9;&#9;&#9;</xsl:text>
	        <xsl:if test="not(@*)"><xsl:element name="{name()}"/></xsl:if>
		<xsl:if test="@*"><xsl:element name="{name()}">
  	        <xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="string()"/></xsl:attribute></xsl:for-each>

                    <xsl:for-each select="*"><xsl:text>&#10;&#9;&#9;&#9;&#9;&#9;</xsl:text>
		    <xsl:if test="not(@*)"><xsl:element name="{name()}"/></xsl:if>
		    <xsl:if test="@*"><xsl:element name="{name()}">
  	            <xsl:for-each select="@*"><xsl:attribute name="{name()}"><xsl:value-of select="string()"/></xsl:attribute></xsl:for-each>



                    <xsl:text>&#10;&#9;&#9;&#9;&#9;&#9;</xsl:text></xsl:element></xsl:if></xsl:for-each>
                <xsl:text>&#10;&#9;&#9;&#9;&#9;</xsl:text></xsl:element></xsl:if></xsl:for-each>
	    <xsl:text>&#10;&#9;&#9;&#9;</xsl:text></xsl:element></xsl:if></xsl:for-each>
	<xsl:text>&#10;&#9;&#9;</xsl:text></xsl:element></xsl:if></xsl:for-each>
    <xsl:text>&#10;&#9;</xsl:text></xsl:element></xsl:if></xsl:for-each>
<xsl:text>&#10;</xsl:text></xsl:element></xsl:if></xsl:for-each>
-->

</xsl:template>
</xsl:stylesheet>