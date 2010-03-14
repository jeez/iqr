#!/bin/bash

INPUT=$1;

if test ! -r $INPUT
    then echo "error: invalid input file"
    exit
fi

cp $INPUT .temp
xmllint --noout --postvalid  .temp 

