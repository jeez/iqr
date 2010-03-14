#!/usr/bin/perl

$input = $ARGV[0];

if(!-e $input){
    print "**cannot find file \"$input\". quit\n";
    exit -1;
}


$cont = `cat $input`;

$_ = $cont;

s/APSource=\"\d*\"/APSource=\"-1\"/g;
s/APTarget=\"\d*\"/APTarget=\"-1\"/g;


print $_;

