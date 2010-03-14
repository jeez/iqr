#!/bin/bash

input=$1;

if test ! -r $input
    then echo "error: invalid input file"
    exit
fi



output=`echo $input | sed -e 's/+/_/' -e 's/\.logml/.m/'`

xsltproc --novalid  optiLog2matlab.xsl $input > $output




