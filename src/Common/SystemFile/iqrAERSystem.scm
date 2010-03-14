<?xml version="1.0" encoding="UTF-8" ?>

<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:element name="AERConnection">
    <xs:complexType>
      <xs:sequence>
        <xs:element ref="pre" minOccurs="1" maxOccurs="unbounded" />
      </xs:sequence>
      <xs:attribute name="type" type="xs:NMTOKEN" use="required" />
      <xs:attribute name="source" type="xs:NMTOKEN" use="required" />
      <xs:attribute name="target" type="xs:NMTOKEN" use="required" />
      <xs:attribute name="id" type="xs:NMTOKEN" use="required" />
    </xs:complexType>
  </xs:element>

  <xs:element name="AERExportSystem">
    <xs:complexType>
      <xs:sequence>
        <xs:element ref="Note" minOccurs="1" maxOccurs="1" />
        <xs:element ref="Process" minOccurs="1" maxOccurs="1" />
        <xs:element ref="AERConnection" minOccurs="1" maxOccurs="unbounded" />
      </xs:sequence>
      <xs:attribute name="name" type="xs:string" use="required" />
      <xs:attribute name="date" type="xs:string" use="required" />
      <xs:attribute name="cylesPerSecond" type="xs:NMTOKEN" use="required" />
      <xs:attribute name="author" type="xs:NMTOKEN" use="required" />
      <xs:attribute name="id" type="xs:NMTOKEN" use="required" />
    </xs:complexType>
  </xs:element>

  <xs:element name="DiagramIcon">
    <xs:complexType>
      <xs:attribute name="x" type="xs:NMTOKEN" use="required" />
      <xs:attribute name="height" type="xs:NMTOKEN" use="required" />
      <xs:attribute name="width" type="xs:NMTOKEN" use="required" />
      <xs:attribute name="y" type="xs:NMTOKEN" use="required" />
    </xs:complexType>
  </xs:element>

  <xs:element name="Group">
    <xs:complexType>
      <xs:sequence>
        <xs:element ref="Topology" minOccurs="1" maxOccurs="1" />
        <xs:element ref="Neuron" minOccurs="1" maxOccurs="1" />
        <xs:element ref="Note" minOccurs="1" maxOccurs="1" />
        <xs:element ref="DiagramIcon" minOccurs="1" maxOccurs="1" />
      </xs:sequence>
      <xs:attribute name="name" type="xs:NMTOKEN" use="required" />
      <xs:attribute name="id" type="xs:NMTOKEN" use="required" />
    </xs:complexType>
  </xs:element>

  <xs:element name="Neuron">
    <xs:complexType>
      <xs:sequence>
        <xs:element ref="Parameter" minOccurs="1" maxOccurs="unbounded" />
      </xs:sequence>
      <xs:attribute name="name" type="xs:NMTOKEN" use="required" />
    </xs:complexType>
  </xs:element>

  <xs:element name="Note">
    <xs:complexType />
  </xs:element>

  <xs:element name="Parameter">
    <xs:complexType>
      <xs:attribute name="name" type="xs:NMTOKEN" use="required" />
      <xs:attribute name="value" use="required">
        <xs:simpleType>
          <xs:restriction base="xs:NMTOKEN">
            <xs:enumeration value="0" />
            <xs:enumeration value="0.3" />
            <xs:enumeration value="1" />
            <xs:enumeration value="false" />
          </xs:restriction>
        </xs:simpleType>
      </xs:attribute>
    </xs:complexType>
  </xs:element>

  <xs:element name="post">
    <xs:complexType>
      <xs:attribute name="index" type="xs:NMTOKEN" use="required" />
      <xs:attribute name="delay" type="xs:NMTOKEN" use="required" fixed="0" />
    </xs:complexType>
  </xs:element>

  <xs:element name="pre">
    <xs:complexType>
      <xs:sequence>
        <xs:element ref="post" minOccurs="1" maxOccurs="unbounded" />
      </xs:sequence>
      <xs:attribute name="index" type="xs:NMTOKEN" use="required" />
    </xs:complexType>
  </xs:element>

  <xs:element name="Process">
    <xs:complexType>
      <xs:sequence>
        <xs:element ref="Note" minOccurs="1" maxOccurs="1" />
        <xs:element ref="DiagramIcon" minOccurs="1" maxOccurs="1" />
        <xs:element ref="Group" minOccurs="1" maxOccurs="unbounded" />
      </xs:sequence>
      <xs:attribute name="port" type="xs:NMTOKEN" use="required" />
      <xs:attribute name="hostname" type="xs:string" use="required" />
      <xs:attribute name="name" type="xs:string" use="required" />
      <xs:attribute name="id" type="xs:NMTOKEN" use="required" />
    </xs:complexType>
  </xs:element>

  <xs:element name="Topology">
    <xs:complexType>
      <xs:sequence>
        <xs:element ref="TopologyRect" minOccurs="1" maxOccurs="1" />
      </xs:sequence>
    </xs:complexType>
  </xs:element>

  <xs:element name="TopologyRect">
    <xs:complexType>
      <xs:attribute name="vcount" type="xs:NMTOKEN" use="required" />
      <xs:attribute name="hcount" type="xs:NMTOKEN" use="required" />
    </xs:complexType>
  </xs:element>

</xs:schema>