#!/usr/bin/perl

#$cont = `cat  ../rpm/iqr-spec-stub`;
#$_ = $cont;
#
#m/(Version:\s)(.*)/;
#$version = $2;
#
#m/(Release:\s)(.*)(\.-.*)/;
#
#$release = $2;
#
#$Version = $version . "." . $release;

$Version = `cat  ../../src/iqr.version`;

print $Version;

open(FH, ">../rpm/BUILD/iqr.version");
print  FH $Version;
close(FH);
