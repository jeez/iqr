#!/bin/bash

distro=""
version=""


if test -e /etc/lsb-release;
    then 
    source /etc/lsb-release
    distro=$DISTRIB_ID
    version=$DISTRIB_RELEASE;
fi

# if /etc/lsb-release the does not contain the desired information (e.g. SuSE 10.3), 
# we have to get the information from somewhere else...

if test -z $DISTRIB_ID
    then
    file=`ls /etc/*release* | /bin/grep -i -e fedora  -e suse`
    if grep -i -h -c "suse" $file 1>/dev/null;
	then 
	distro=SuSE;
    elif grep -i -h -c "red hat" $file 1>/dev/null;
	then 
	distro=rh;
    elif grep -i -h -c "fedora" $file 1>/dev/null;
	then 
	distro=fc;
    fi
    version=`rpm -qf --queryformat '%{VERSION}\n' $file`
fi


distro=$distro$version

echo $distro





