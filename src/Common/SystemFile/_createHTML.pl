#!/usr/bin/perl
# Copyright Bernardet 2001/03/22

#TestXSLT -IN foo.xml -XSL foo.xsl -OUT foo.out 

($fname) = @ARGV;


$htmlxsl = "iqr421_System_0201010204030201_html.xsl";
$dotxsl  = "iqr421_System_0201010204030201_dot.xsl";
print "=============================================================\n";
print "this script converters a iqr_system_file into a html document\n";
print "=============================================================\n\n";

$_ = $fname;
s/\.xml//gi;
$dirname = $_;



if($fname=~m/\.xml/gi){
    $_ = $fname;
    s/\.xml//gi;
    $dirname = "_examples/$_";

    $_ = $fname;
    s/\.xml/.xsl/gi;
    $xslfile = $_;

    $_ = $fname;
    s/\.xml/.html/gi;
    $htmlfile = $_;

    $_ = $fname;
    s/\.xml/.dot/gi;
    $dotfile = $_;

    $_ = $fname;
    s/\.xml/.gif/gi;
    $giffile = $_;

#    print $xslfile. "\n" . $dotfile . "\n" . $giffile . "\n";

    #first create the output dir
    $cmd = "mkdir $dirname";
    print "\ncreating the output directory\n";
#    print $cmd . "\n";
    system $cmd;

    #first creating the dot file
    $cmd = "testXSLT -TEXT -IN $fname -XSL dot/$dotxsl -OUT $dirname/$dotfile";
    print "\ntransforming to dot\n";
#    print $cmd . "\n";
    system $cmd;

    #next the gif image
    $cmd = "dot -Tgif $dirname/$dotfile -o $dirname/$giffile";
    print "\nrunning dot\n";
#    print $cmd . "\n";
    system $cmd;

    #and last the html document
    $cmd = "testXSLT -HTML -IN $fname -XSL html/$htmlxsl -OUT $dirname/$htmlfile";
    print "\ntransforming to html\n";
#    print $cmd . "\n";
    system $cmd;


} else {
    print "The argument of the script should consists of a xml file\n";
}
