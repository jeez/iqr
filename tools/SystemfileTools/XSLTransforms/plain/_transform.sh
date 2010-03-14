#!/bin/su


testXSLT -XML  -IN ../iqr421_System_0303030204030201_2.xml -XSL iqr421_System_plain.xsl -OUT test.xml


#  [-INDENT n (Controls how many spaces to indent. {default is 0})]
#  [-VALIDATE (Controls whether validation occurs. Validation is off by default.)]
#  [-TT (Trace the templates as they are being called.)]
#  [-TG (Trace each generation event.)]
#  [-TS (Trace each selection event.)]
#  [-TTC (Trace the template children as they are being processed.)]
#  [-XML (Use XML formatter and add XML header.)]
#  [-TEXT (Use simple Text formatter.)]
#  [-HTML (Use HTML formatter.)]
#  [-DOM (Use DOM formatter.  Formats to DOM, then formats XML for output.)]
#  [-PARAM name expression (Sets a stylesheet parameter.)]
#  [-S (Display some interesting statistics.)]
