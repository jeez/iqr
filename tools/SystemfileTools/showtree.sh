#!/bin/bash

INPUT=$1;

if test ! -r $INPUT
    then echo "error: invalid input file"
    exit
fi

xsltproc --novalid tree.xsl $INPUT