#!/usr/bin/perl

$outfile = "iqr421_system.dtd";


($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst) = localtime();
$year+=1900;
$mon++;
$datestring = "$year" . ($mon < 10 ? "-0": "-") . $mon . ($mday < 10 ? "-0": "-") . $mday . " $hour:$min:$sec\n";


print "\nThis script will generate a DTD file for iqr421 by assembling it from components\n\n"; 

$iqr_System =      `cat DTDComponents/iqr_System.dtd`; 
$iqr_System =~ m/(VERSION:)(\d*)/gi;
$iqr_System_Verson = $2;

$Prc_Process =     `cat DTDComponents/Prc_Process.dtd`; 
$Prc_Process =~ m/(VERSION:)(\d*)/gi;
$Prc_Process_Verson = $2;

$Prc_Module =      `cat DTDComponents/Prc_Module.dtd`; 
$Prc_Module =~ m/(VERSION:)(\d*)/gi;
$Prc_Module_Version = $2;

$Prc_Group =       `cat DTDComponents/Prc_Group.dtd`; 
$Prc_Group  =~ m/(VERSION:)(\d*)/gi;
$Prc_Group_Version = $2;

$Prc_Connections = `cat DTDComponents/Prc_Connections.dtd`; 
$Prc_Connections =~ m/(VERSION:)(\d*)/gi;
$Prc_Connections_Version = $2;

$Prc_Neurons =     `cat DTDComponents/Prc_Neurons.dtd`; 
$Prc_Neurons =~ m/(VERSION:)(\d*)/gi;
$Prc_Neurons_Version = $2;

$Prc_Synapses =     `cat DTDComponents/Prc_Synapses.dtd`;
$Prc_Synapses =~ m/(VERSION:)(\d*)/gi;
$Prc_Synapses_Version = $2;


$FE_Elements =      `cat DTDComponents/FE_Elements.dtd`;               
$FE_Elements =~ m/(VERSION:)(\d*)/gi;
$FE_Elements_Version = $2;

$Helpers =                `cat DTDComponents/Helpers.dtd`;               
$Helpers =~ m/(VERSION:)(\d*)/gi;
$Helpers_Version = $2;


print "iqr_System_Verson            : " . $iqr_System_Verson       . "\n";
print "Prc_Process_Verson           : " . $Prc_Process_Verson      . "\n";
print "Prc_Module_Version           : " . $Prc_Module_Version      . "\n";
print "Prc_Group_Version            : " . $Prc_Group_Version       . "\n";
print "Prc_Connections_Version      : " . $Prc_Connections_Version . "\n";
print "Prc_Neurons_Version          : " . $Prc_Neurons_Version     . "\n";
print "Prc_Synapses_Version         : " . $Prc_Synapses_Version    . "\n";
print "FE_Elements_Version          : " . $FE_Elements_Version     . "\n";
print "Helpers_Version              : " . $Helpers_Version         . "\n";

$versionstring = $iqr_System_Verson       .
                 $Prc_Process_Verson      . 
                 $Prc_Module_Version      . 
                 $Prc_Group_Version       . 
                 $Prc_Connections_Version . 
                 $Prc_Neurons_Version     . 
                 $Prc_Synapses_Version    . 
                 $FE_Elements_Version     . 
                 $Helpers_Version         ;

print "\nBased on the above listed version number, the DTD version will be\n\t $versionstring\n";

$outfile = "iqr421_System_" . $versionstring . ".dtd";

open(FHO, ">$outfile");

print FHO "<!-- XML iqr421_System DTD
        DTD for iqr421 systems
        AUTHOR:  Ulysses Bernardet & J. Mark Blanchard
        VERSION: $versionstring 
        DATE: $datestring -->\n";
print FHO $iqr_System            . "\n"; 
print FHO $Prc_Process           . "\n";
print FHO $Prc_Module            . "\n";
print FHO $Prc_Group             . "\n";
print FHO $Prc_Connections       . "\n";
print FHO $Prc_Neurons           . "\n";
print FHO $Prc_Synapses          . "\n";
print FHO $FE_Elements           . "\n";
print FHO $Helpers               . "\n";

close FHO;




