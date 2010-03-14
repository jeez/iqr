/****************************************************************************
 ** $Filename: moduleIcon.hpp
 ** $Id$
 **
 ** Copyright (C) 2003 by Ulysses Bernardet
 ** $Author: Ulysses Bernardet
 ** 
 ** $CreateDate: Wed Dec 10 17:19:43 2003
 ** $Date$
 **
 ** $Log$
 **
 ** $Keywords:
 ** $Description:
 **
 *****************************************************************************/

#ifndef MODULEICON_HPP
#define MODULEICON_HPP


struct moduleIcon {
//    const char* data;
    const unsigned char* data;
    int size;

    int x0;
    int y0;

//    moduleIcon(const char* _data, int _size, int _x0 = 0, int _y0 = 0){
    moduleIcon(const unsigned char* _data, int _size, int _x0 = 0, int _y0 = 0){
	data = _data;
	size = _size;
	x0 = _x0;
	y0 = _y0;
    }
};

#endif
