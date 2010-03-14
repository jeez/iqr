#!/bin/bash
STARTTIME=`date +%s`
$*
ENDTIME=`date +%s`

TOTALTIME=$(($ENDTIME - $STARTTIME))


echo "######### time running #########"
echo "\"$*\"" 
echo "ran for $TOTALTIME seconds"