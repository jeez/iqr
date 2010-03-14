Summary: tools for working with iqr system and data files
Name: iqr-tools
Version: 1.0
Release: 2
License: GPL
Group: X11/KDE/Applications
BuildRoot: /tmp/rpmbuild
Autoreq:       No
Requires: libxslt perl


%description
iqr is a simulation software to design and control large-scale neuronal models with a graphical
interface. This package contains tools for working iqr system and data files.

%install

mkdir -p -m 0755 $RPM_BUILD_ROOT/usr/lib/iqr/xsl
mkdir -p -m 0755 $RPM_BUILD_ROOT/usr/local/bin

#transforms
install -m 0644 ../tools/SystemfileTools/dotPlotGroups/dotPlotGroups.xsl         $RPM_BUILD_ROOT/usr/lib/iqr/xsl
install -m 0644 ../tools/SystemfileTools/dotPlotGroups/dotPlotGroups-AER.xsl     $RPM_BUILD_ROOT/usr/lib/iqr/xsl
install -m 0644 ../tools/SystemfileTools/dotPlotSingleCell/dotPlotSingleCell.xsl $RPM_BUILD_ROOT/usr/lib/iqr/xsl
install -m 0644 ../tools/SystemfileTools/SystemStatistics/statsAERexportHTML.xsl $RPM_BUILD_ROOT/usr/lib/iqr/xsl
install -m 0644 ../tools/SystemfileTools/SystemStatistics/stats.xsl              $RPM_BUILD_ROOT/usr/lib/iqr/xsl

#perl scripts
install -m 0755 ../tools/DatafileTools/iqrData2Pattern.pl       $RPM_BUILD_ROOT/usr/local/bin
install -m 0755 ../tools/DatafileTools/iqrData2matlabImport.pl  $RPM_BUILD_ROOT/usr/local/bin
install -m 0755 ../tools/SystemfileTools/iqrSysTransform        $RPM_BUILD_ROOT/usr/local/bin

%clean
rm -rf $RPM_BUILD_ROOT

%files
/usr/lib/iqr/xsl
/usr/local/bin/iqrData2Pattern.pl
/usr/local/bin/iqrData2matlabImport.pl
/usr/local/bin/iqrSysTransform 
