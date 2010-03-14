<xsl:stylesheet version="1.0"   xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html"  version="3.2" encoding="iso-8859-1"/>

<xsl:key name="keyProcess" match="Process" use="@type"/>
<xsl:key name="keyGroup" match="Process" use="@type"/>
<xsl:key name="keyNeuron" match="Neuron" use="@name"/>
<xsl:key name="keySynapse" match="Synapse" use="@name"/>
<xsl:key name="keyCPU" match="CPU" use="@model"/>

<xsl:template name="Total">
  <xsl:param name="Items"/>
  <xsl:param name="RunningTotal"/>
  <xsl:choose>
   <xsl:when test="not($Items)">
    <xsl:copy-of select="$RunningTotal"/>
   </xsl:when>
   <xsl:otherwise>
  <xsl:variable name="CurrentTotal"
         select="$RunningTotal + ($Items[1]/@hcount * $Items[1]/@vcount)"/>

   <xsl:call-template name="Total">
    <xsl:with-param name="Items" select="$Items[position()>1]"/>
    <xsl:with-param name="RunningTotal" select="$CurrentTotal"/>
   </xsl:call-template>
   </xsl:otherwise>
  </xsl:choose>
 </xsl:template>


<xsl:template match="/">






<h2>statistics for system "<xsl:value-of select="AERExportSystem/@name" />"</h2>  
<hr/>



  <xsl:variable name="Items" select="AERExportSystem/Process/Group/Topology/TopologyRect"/>

<xsl:variable name="TotalValue">
  <xsl:call-template name="Total">
    <xsl:with-param name="Items" select="$Items"/>
    <xsl:with-param name="RunningTotal"    select="0"/>
  </xsl:call-template>
</xsl:variable>



<!-- 
<xsl:number format="01" grouping-separator="," grouping-size="3"/>
<xsl:apply-templates/> -->

<ul>
  <li>Processes: <xsl:value-of select="count(AERExportSystem/Process)"/></li>
  <ul>
    <li>Groups: <xsl:value-of select="count(AERExportSystem/Process/Group)"/></li>
    <ul>
      <li>Neurons: <xsl:value-of select="$TotalValue"/></li>
      <li>Neuron types:</li>
      <ul>
      <xsl:for-each select="//*[generate-id(.)=generate-id(key('keyNeuron',@name)[1])]">
        <xsl:sort select="@name"/>
        <xsl:value-of select="@name"/>: <xsl:value-of select="count(key('keyNeuron',@name))"/><br/><xsl:text>
      </xsl:text>
      </xsl:for-each>
</ul>
    </ul>
  </ul>
  <li>Synapses: <xsl:value-of select="count(AERExportSystem/AERConnection/pre)"/></li>
  <li>Connection: <xsl:value-of select="count(AERExportSystem/AERConnection)"/></li>
  <ul>
      <ul>
        <li>Synapse types:</li>
        <ul>
          <xsl:for-each select="//*[generate-id(.)=generate-id(key('keySynapse',@name)[1])]">
            <xsl:sort select="@name"/>
            <xsl:value-of select="@name"/>: <xsl:value-of select="count(key('keySynapse',@name))"/><br/><xsl:text>
      </xsl:text>
          </xsl:for-each>
      </ul>
    </ul>
  </ul>
</ul>




</xsl:template>

</xsl:stylesheet>