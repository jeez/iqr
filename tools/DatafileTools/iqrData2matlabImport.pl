#!/usr/bin/perl

# the purpose of this perl script is to create a specific matlab import script for the iqr
# data file passed as an argument.
#
# Author: Ulysses Bernardet
# Create Date: 2006/01/07

if(@ARGV!=1){
    $c = @ARGV;
    die "ERROR: Wrong number of arguments ($c).\nUsage: iqrData2matlabImport.pl <infile>\n\n";
}


$infile = $ARGV[0];



@parameters = `grep -e \"\\#\" $infile`;
$range = `grep -e cycle $infile`;
$range =~ s/cycle;//;

$nrcomment_lines = @parameters + 1;

$ii=-999;
foreach $line(@parameters){
    $line =~ s/\n//;
    $line =~ s/\#\s//;
    $_ = $line;
#    ($a, $b) = split("=", $line);
    m/(.*)(=)(.*)/;
    $name = $1;
    $ID = $3;
    $name =~ s/\"//g;
    $name =~ s/\s/_/g;
    $name =~ s/\//_/g;

#    print "$name *** $ID\n";
    $id2name{$ID} = $name;
    $id2start{$ID} = $ii;
    $id2width{$ID} = 0;
    $ii++;
}


@ranges = split(";", $range);
$linecounter = 0;
foreach $line(@ranges){
    $line =~ s/\n//;
    $_ = $line;
    m/(L-\d*-\d*-\d*)(.*)/;
    $key = $1;

    if($id2start{$key}<0){
	$id2start{$key} = $linecounter +2;
    }

    $id2width{$key}++;
#    print $linecounter . "\t" . $line . "\t" . $id2start{$key} . "\t" . $id2width{$key} . "\n";
    $linecounter++;
}

$outfile = $infile;
$outfile =~ s/\.dat//g;
$outfile =~ s/\./_/g;
$dataname = $outfile;
$scriptname = $outfile . "_import";
$outfile = $outfile . "_import.m";




open (FH, ">$outfile");

print FH "function [data]=$scriptname()\n";

print FH "inputData=textread(\'$infile\', \'\', ...
		   \'delimiter\', \';\', ...
		   \'commentstyle\', \'shell\', ...
		   \'headerlines\', $nrcomment_lines);
";

print FH "data.cycles=inputData(: ,1:1);\n";


@IDarray = keys(%id2name); 
foreach $id(@IDarray){

    $name = $id2name{$id};
    $start = $id2start{$id};
    $width = $start + $id2width{$id} -1;

    print FH "data.$name=inputData(: ,$start:$width);\n";

}

print FH "data.nrrows = size(inputData,1)\n";
print FH "data.name = \'$dataname\'\n";


close(FH);



# loop script:
$outfile = "loopScript.m";
if(-e $outfile){
    print "\"$outfile\" found in directory; skipping creation\n";
} else {
    print "Creating \"$outfile\"\n";
    open (FH, ">$outfile");
    
    print FH " 
    files = dir(\'*.dat\');
    for ff=1:size(files,1)
	nn=regexp(files(ff).name, \'(\\w*)\', \'match\');
    basename = nn(1);
    script_filename = strcat(basename, \'_import.m\');
    cmd = [\'test -e \' char(script_filename)];
    if system(cmd)
	disp(\'import script does not exist: running converter:\')
	cmd = [\'iqrData2matlabImport.pl \' char(files(ff).name)]
	system(cmd)
	end
	function_name = char(strcat(basename, \'_import\'));
%        function_name=eval(function_name);
        eval(strcat(char(basename), '=', function_name));
    end
    clear cmd ff files nn function_name script_filename basename
    ";
    
    close(FH);
}

