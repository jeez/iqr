#!/usr/bin/perl

# Ulysses Bernardet 2005/08/07
# This script converts data sampled from iqr to a pattern to be used
# with the StateManipulation panel.
 
# cmdl arguments:
if(@ARGV!=3){
    $c = @ARGV;
    die "ERROR: Wrong number of arguments ($c).\nUsage: data2pattern.pl <file> <width> <height>\n\n";
}


$infile = @ARGV[0];
$width = @ARGV[1];
$height = @ARGV[2];

if(! -e $infile){
    die "ERROR: File $infile not found.\n\n";
}

#------

$cont = `tail -1 $infile`;
@data = split /;/, $cont;

#print @data . "\n";
$nr_entries = @data;

#first value is the cycle number...
$counter = 1;


for($iy = 0; $iy<=$height; $iy++){
    print "0 \t";
}
print "\n";

for($ix = 0; $ix<$width; $ix++){
    $lines[$ix] = "0";
}

for($iy = 0; $iy<$height; $iy++){
    for($ix = 0; $ix<$width; $ix++){
	$lines[$ix] = $lines[$ix] . " \t" . $data[$counter];
	$counter++;
    }
}


foreach $line(@lines){
    print $line . "\n";
}

