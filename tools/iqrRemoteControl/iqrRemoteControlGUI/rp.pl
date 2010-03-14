

$qstrMessage="cmd:start;";
$qstrMessage="cmd:stop;";
$qstrMessage="cmd:quit;";
#$qstrMessage="cmd:param;itemType:CONN;itemName:this is a funky name;paramName:<>;value:201.114;";
$qstrMessage="cmd:param;itemType:GROUP;itemID:L-6648-1107513181-2053999932;paramID:L-112-414;value:1.002;";


$qstrMessage="cmd:param;itemType:NEURON;itemID:L-6648-1107513181-2053999932;paramID:excGain;value:.5;";
$qstrMessage="cmd:param;itemType:PROCESS;itemID:L-6648-1107513172-1474612399;paramID:port;value:125;";
$qstrMessage="cmd:param;itemType:CONNECTION;itemID:L-6648-1107513304-943947739;paramID:type;value:inhibitory;";

cmd:param;itemType:CONNECTION;itemID:L-6648-1107513304-943947739;paramID:type;value:inhibitory;
cmd:param;itemType:CONNECTION;itemID:L-6648-1107513304-943947739;paramID:type;value:test


cmd:param;itemType:GROUP;itemName:G1;paramID:name;value:test

probability



print $qstrMessage . "\n";
$_ = $qstrMessage;
#m/(itemType:)(\w*)/;
#m/(cmd:)(start|stop|quit|param)/;

#m/(.*;)(itemType:)(.*)(;itemID:)(.*)(;paramID:)(.*)(;value:)(.*)(;)/;


m/(.*;)(itemType:)(.*)(;)(itemID|itemName)(:)(.*)(;)(paramID|paramName)(:)(.*)(;value:)(.*)(;)/;


print $3 . "\n";

exit;


#5 -> itemID or itemName
#7 -> value for itemID or itemName
#9 -> paramID or paramName
#11 -> value for paramID or paramName
#13 -> value


#@lst = split(";", $qstrMessage);
#foreach $ll(@lst){
#    print $ll . "\n";
#}
