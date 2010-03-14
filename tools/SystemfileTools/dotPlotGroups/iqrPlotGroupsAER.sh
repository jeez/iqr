#!/bin/bash

INPUT=$1;

if test ! -r $INPUT
    then echo "error: invalid input file"
    exit
fi



image=`echo $INPUT | sed 's/\..*/.ps/'`

xsltproc --novalid dotPlotGroups-AER.xsl $INPUT > .temp
dot -Tps .temp > $image

