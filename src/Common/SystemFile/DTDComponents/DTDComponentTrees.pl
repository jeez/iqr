#!/usr/bin/perl

$treescript = "DTD2LatexTree.pl";
@files = `ls *.dtd`;

foreach $file (@files){
    $_ = $file;
    s/\n//;
    $infile = $_;
    s/\.dtd/_tree.tex/;
    $outfile = $_;
    $cmd = "perl $treescript -level 40 -dtd $infile > $outfile";
    system $cmd;
    print $cmd . "\n";


    $cont = `cat $outfile`;
    $_ = $cont;

    s/(^-)(.*\n)(\s)(.*\n)(-*\n)/\n/gm;
    

#    s/(^[A-Z]+)/\\begin{compacttreelist}\n\\item $1 |/gm;
    s/(^[A-Z]+)/\\paragraph{$1}/gm;
#    s/^\\end{compacttreelist}/\\end{compacttreelist}\n\\end{compacttreelist}/gm;
    tr/A-Z/a-z/;

    s/all/All/g;
    s/arb/Arb/g;
    s/arborization/Arborization/g;
    s/block/Block/g;
    s/connection/Connection/g;
    s/connections/Connections/g;
    s/delay/Delay/g;
    s/ellipse/Ellipse/g;
    s/field/Field/g;
    s/fire/Fire/g;
    s/fun/Fun/g;
    s/function/Function/g;
    s/group/Group/g;
    s/gaussian/Gaussian/g;
    s/geometry/Geometry/g;
    s/hebbian/Hebbian/g;
    s/hex/Hex/g;
    s/integ/Integ/g;
    s/linear/Linear/g;
    s/list/List/g;
    s/module/Module/g;
    s/neuron/Neuron/g;
    s/parameter/Parameter/g;
    s/pattern/Pattern/g;
    s/pf/PF/g;
    s/point/Point/g;
    s/process/Process/g;
    s/random/Random/g;
    s/rect/Rect/g;
    s/region/Region/g;
    s/rf/RF/g;
    s/sigmoid/Sigmoid/g;
    s/simulation/Simulation/g;
    s/single/Single/g;
    s/sparse/Sparse/g;
    s/spike/Spike/g;
    s/source/Source/g;
    s/synapse/Synapse/g;
    s/system/System/g;
    s/tanh/TanH/g;
    s/target/Target/g;
    s/template/Template/g;
    s/threshold/Threshold/g;
    s/to/To/g;
    s/uniform/Uniform/g;
    s/userdef/UserDef/g;
    s/wd/WD/g;
    s/weight/Weight/g;
    s/window/Window/g;
    s/compacttreeList/compacttreelist/g;

    s/\\begin{compacttreeList}(\n|\s)*\\item EMPTY(\n|\s)*\\end{compacttreeList}//gi;

    s/\s*\n/\n/g;
    s/\n\n/\n/gi;

    $cont = $_;


    $dtd = `cat $infile`;
    print "reading dtd from $infile\n";
#-----------------------------------------------
#building the database with all the attributes:
    while($dtd =~ m/(<\!ATTLIST)(\s*)(\w*)(.*?>)/gis){
	$key =  $3;
	$itemlist = att2list($4);
	$att{$key} = $itemlist;
#	print $key . "--\n";
    }
#-----------------------------------------------

    $_ = $cont;
    s/(\\item)(\W*)(\w*)(\W*)([*|+),])(\n)/$1$2$3$4\n$att{$3}\n/gi;

    s/(\\paragraph{)(\w*)(})(\n)/$1$2$3\n$att{$2}\n/gi;



s/_/\\_/g;
$cont = $_;
open (FH, ">$outfile");
print FH $_;

close FH;

print "-------------------------\n\n";
}

sub att2list(){
($att2) = @_;
my $itemlist = "";
$_ = $att2;
s/^\s*//gim;
s/^\n//gi;
$att2 = $_;

$itemlist = "\t\t\\begin{compactitem}\n";
@t = split /\n/, $att2;
foreach $line (@t){
    $line =~ m/^\w*/gi;
    $itemlist = $itemlist . "\t\t\t\\item " . $& . "\n";
    
}
$itemlist = $itemlist . "\t\t\\end{compactitem}\n";

return $itemlist;
}


#sub getAtt(){
#($in) = @_;
#print "\n-------\n$in\n--------------\n";
#my $back;
#$back = (length($att{$in}) > 1 ? "\n$att{$in}" : "");
#$att{$in} = "";
#return $back;
#
#}
