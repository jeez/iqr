#!/bin/bash


for file in `ls *.iqr` ;
  do
  ls -og $file & xsltproc --novalid  ~/Code/iqr421_Redesign/iqrUtils/systemNote.xsl $file ;
done
