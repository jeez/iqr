#!/usr/bin/perl
use Cwd;
use Term::ANSIColor;

$outfile = "index.html";
open (FH, ">$outfile");

$title = "Contents of <tt>$basedir<\/tt>";
$title = "<tt>iqrPackages<\/tt><hr\/>";




 


print FH  "
<HTML>
<HEAD>
<TITLE>Documents in subdirs of $basedir<\/TITLE>
<\/HEAD>
<BODY>
<table border =1 with=80%>";


my @mydocs = `ls -tr *.iqr`;

foreach $mydoc (@mydocs){
    $mydoc =~ s/\n//;;

    @info = stat $mydoc;
    $dateLastMod = $info[9];
    @dateInfo = localtime($dateLastMod);
    
    $day = $dateInfo[3];
    $month = $dateInfo[4] + 1;
    $year = $dateInfo[5] + 1900;
    
    $hour = $dateInfo[2];
    $min = $dateInfo[1];
    
    $fileinfo = "$year\/$month\/$day $hour:$min";

    $note = `xsltproc --novalid  ~/Code/iqr421_Redesign/iqrUtils/systemNote.xsl $mydoc`;



    $ref = "<a href=$mydoc>$mydoc<\/a>";
    print FH "<tr with=80%><td width=15%>$ref</td><td with=15%>$fileinfo</td><td with=70%>$note</td></tr>";
    print color 'bold blue';
    print "\n$mydoc ($fileinfo)\n";
    print color 'reset';
    print "$note";
    
}

print "\n";

print FH "</table>";
print FH "<\/BODY><\/HTML>";
close FH;

