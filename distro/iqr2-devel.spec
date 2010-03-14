Summary: sources and documentation to write iqr usedefined types
Name: iqr-devel
Version: 2.0
Release: 0
License: GPL
Group: X11/KDE/Applications
BuildRoot: /tmp/rpmbuild
Autoreq:       No



%description
iqr is a simulation software to design and control large-scale neuronal models with a graphical
interface. The modelled systems can be connected to various hardware devices, e.g. to
control the behavior of a robot or read input from a camera. 

%install
mkdir -p -m 0755 $RPM_BUILD_ROOT/usr/include/iqr/Common/Helper/
mkdir -p -m 0755 $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
mkdir -p -m 0755 $RPM_BUILD_ROOT/usr/share/doc/%{name}/
mkdir -p -m 0755 $RPM_BUILD_ROOT/usr/share/doc/%{name}/examples/Modules
mkdir -p -m 0755 $RPM_BUILD_ROOT/usr/share/doc/%{name}/examples/Neurons

install -m 0644 ../src-qt4/Common/Helper/iqrUtils.h             $RPM_BUILD_ROOT/usr/include/iqr/Common/Helper/
install -m 0644 ../src-qt4/Common/Item/boolParameter.hpp        $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/colorParameter.hpp       $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/doubleParameter.hpp      $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/indirectPtr.hpp          $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/intParameter.hpp         $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/item.hpp                 $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/module.hpp               $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/moduleIcon.hpp           $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/namedPtr.hpp             $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/neuron.hpp               $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/optionsParameter.hpp     $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/parameter.hpp            $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/parameterList.hpp        $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/pathParameter.hpp        $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/pattern.hpp              $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/ringBuffer.hpp           $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/stateArray.hpp           $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/stateVariable.hpp        $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/stateVariableHolder.hpp  $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/stateVariableList.hpp    $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/stringParameter.hpp      $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/synapse.hpp              $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/threadModule.hpp         $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/xRef.hpp                 $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/xRefHolder.hpp           $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../src-qt4/Common/Item/xRefList.hpp             $RPM_BUILD_ROOT/usr/include/iqr/Common/Item/
install -m 0644 ../doc/iqrUserdefinedTypes.pdf              $RPM_BUILD_ROOT/usr/share/doc/%{name}/

#doxygen ../resources/UserDefFramework/Doxyfile-UserDefFramework

install -m 0644 ../distro/resources/UserDefFramework/examples/Modules/* $RPM_BUILD_ROOT/usr/share/doc/%{name}/examples/Modules
install -m 0644 ../distro/resources/UserDefFramework/examples/Neurons/* $RPM_BUILD_ROOT/usr/share/doc/%{name}/examples/Neurons


%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%clean
rm -rf $RPM_BUILD_ROOT

%files
/usr/include/iqr/*
/usr/share/doc/%{name}/*
/usr/share/doc/%{name}/examples/Modules/*
/usr/share/doc/%{name}/examples/Neurons/*
