#!/bin/bash

mkdir -p                                          /tmp/UserDefFramework/Common/Helper
mkdir -p                                          /tmp/UserDefFramework/Common/Item
cp ../../src/Common/Helper/iqrUtils.h             /tmp/UserDefFramework/Common/Helper/

cp ../../src/Common/Item/boolParameter.hpp        /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/doubleParameter.hpp      /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/indirectPtr.hpp          /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/intParameter.hpp	  /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/item.hpp		  /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/module.hpp	          /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/moduleIcon.hpp	          /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/namedPtr.hpp	          /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/neuron.hpp               /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/optionsParameter.hpp     /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/parameter.hpp            /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/parameterList.hpp        /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/pattern.hpp              /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/ringBuffer.hpp           /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/stateArray.hpp           /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/stateVariableHolder.hpp  /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/stateVariable.hpp        /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/stateVariableList.hpp    /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/stringParameter.hpp      /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/synapse.hpp              /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/threadModule.hpp	  /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/xRefHolder.hpp	          /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/xRef.hpp		  /tmp/UserDefFramework/Common/Item/
cp ../../src/Common/Item/xRefList.hpp             /tmp/UserDefFramework/Common/Item/

cp -r ../resources/UserDefFramework/examples/     /tmp/UserDefFramework/

#doxygen ../resources/UserDefFramework/Doxyfile-UserDefFramework

tar cfvz /tmp/UserDefFramework.tgz  -C /tmp UserDefFramework/ 

rm -r /tmp/UserDefFramework/

#doxygen
      

