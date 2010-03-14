#!/bin/bash

#finding the distribution and release 
prefix=iqr
#file=SuSE-release
#file=redhat-release

#file=`ls /etc/*release* | head -1`
file=`ls /etc/*release* | /bin/grep -i -e fedora  -e suse`


distro=""
version=""

if grep -i -h -c "suse" $file;
    then 
    distro=SuSE;
elif grep -i -h -c "red hat" $file;
    then 
    distro=rh;
elif grep -i -h -c "fedora" $file;
    then 
    distro=fc;
fi


#version=`rpm -qf --queryformat '%{VERSION}\n' /etc/*release* | head -1`
version=`rpm -qf --queryformat '%{VERSION}\n' $file`

distro=$distro$version


echo distro: $distro
#echo $suffix > distro

package_name=$prefix

#requirements



#echo qtreq $qt_req


echo ##############

xerceslib=`ldd ../../bin/iqr  | grep -i -o -P "\/.*xerces.*\d\s"`
xercesname=`rpm -qf --queryformat '%{NAME}' $xerceslib`
xercesversion=`rpm -qf --queryformat '%{VERSION}' $xerceslib`
echo xercesname: $xercesname
echo xercesversion: $xercesversion

qtlib=`ldd ../../bin/iqr  | grep -i -o -P "\/.*qt.*\d\s"`
qtname=`rpm -qf --queryformat '%{NAME}' $qtlib`
qtversion=`rpm -qf --queryformat '%{VERSION}' $qtlib`
echo qtname: $qtname
echo qtversion: $qtversion

echo ------------------


cat ../rpm/iqr-spec-stub | sed -e s/-NAME-/$package_name/ \
    -e s/-DISTRO_NAME-/$distro/ \
    -e s/-XERCES_NAME-/$xercesname/ \
    -e s/-XERCES_VERSION-/$xercesversion/ \
    -e s/-QT_NAME-/$qtname/ \
    -e s/-QT_VERSION-/$qtversion/ > ../rpm/BUILD/iqr.spec

