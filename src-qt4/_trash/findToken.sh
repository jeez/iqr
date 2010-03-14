#!/bin/bash
grep -rI $1 --include=*.cpp * | grep -v svn | awk --field-separator : '{print $1}' | sort | uniq 
