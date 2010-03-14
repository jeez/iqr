EXEC_PROGRAM(xsltproc 
ARGS "-o ${CMAKE_INSTALL_PREFIX}/news.html --stringparam cdate \"`date`\" doc/tux2news.xsl doc/iqrNotes.tux"
)


#EXEC_PROGRAM(bash ../distro/scripts
#	ARGS "prepareUserDefFramework.sh")


#FILE(MAKE_DIRECTORY /opt/iqr)


