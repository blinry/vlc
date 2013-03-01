/*****************************************************************************
 * seekstyle_barcode.hpp : Barcode style
 ****************************************************************************
 * Copyright (C) 2013 VLC authors and VideoLAN
 *
 * Authors: Sebastian Morr <sebastian@morr.cc>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef SEEKSTYLE_BARCODE_HPP
#define SEEKSTYLE_BARCODE_HPP

#include <QWindowsStyle>

class SeekStyleBarcode : public QWindowsStyle
{
    Q_OBJECT
public:
    SeekStyleBarcode() { }
    virtual int pixelMetric(PixelMetric metric, const QStyleOption * option = 0, const QWidget * widget = 0) const;
    virtual void drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p, const QWidget *widget) const;
};

#endif // SEEKSTYLE_BARCODE_HPP
