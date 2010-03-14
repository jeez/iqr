<xsl:stylesheet version="1.0"   xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="text"  version="3.2" encoding="iso-8859-1"/>

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
   <xsl:variable name="Items" select="AERExportSystem/Process/Group/Topology/TopologyRect"/>
   
   <xsl:variable name="TotalValue">
     <xsl:call-template name="Total">
       <xsl:with-param name="Items" select="$Items"/>
       <xsl:with-param name="RunningTotal"    select="0"/>
     </xsl:call-template>
   </xsl:variable>

\begin{table}[!h]
  \centering
  \caption{statistics for system ``<xsl:value-of select="AERExportSystem/@name" />''}
  \small
  \begin{tabular}{ll|l}
    element type &amp;&amp; count\\\hline\hline
    Processes &amp;&amp; <xsl:value-of select="count(AERExportSystem/Process)"/>\\\hline
    Groups    &amp;&amp; <xsl:value-of select="count(AERExportSystem/Process/Group)"/>\\\hline
    Neurons   &amp;&amp; <xsl:value-of select="$TotalValue"/>\\\hline
\multicolumn{2}{l}{\quad Number of groups with neuron of type}\\
<xsl:for-each select="//*[generate-id(.)=generate-id(key('keyNeuron',@name)[1])]">
  <xsl:sort select="@name"/> &amp; <xsl:value-of select="@name"/> &amp; <xsl:value-of select="count(key('keyNeuron',@name))"/>\\
</xsl:for-each>\hline
    Connections &amp;&amp; <xsl:value-of select="count(AERExportSystem/AERConnection)"/>\\\hline
    Synapses    &amp;&amp; <xsl:value-of select="count(AERExportSystem/AERConnection/pre)"/>\\\hline
    \multicolumn{2}{l}{\quad Synapse types used in connections}\\
<xsl:for-each select="//*[generate-id(.)=generate-id(key('keySynapse',@name)[1])]">
  <xsl:sort select="@name"/> &amp; <xsl:value-of select="@name"/> &amp; <xsl:value-of select="count(key('keySynapse',@name))"/>\\
</xsl:for-each>\hline
    Modules &amp;&amp; <xsl:value-of select="count(AERExportSystem/Process/Module)"/>\\
  \end{tabular}
  \label{tab:<xsl:value-of select="AERExportSystem/@id" />}
\end{table}





</xsl:template>

</xsl:stylesheet>