/*****************************************************************************
 * seekstyle_barcode.cpp : Seek slider style
 ****************************************************************************
 * Copyright (C) 2011-2012 VLC authors and VideoLAN
 *
 * Authors: Ludovic Fauvet <etix@videolan.org>
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

#include "styles/seekstyle_barcode.hpp"
#include "util/input_slider.hpp"
#include "adapters/seekpoints.hpp"

#include <QWindowsStyle>
#include <QStyleOptionSlider>
#include <QPainter>
#include <QDebug>

#define RADIUS 3
#define CHAPTERSSPOTSIZE 3

int SeekStyleBarcode::pixelMetric( PixelMetric metric, const QStyleOption *option, const QWidget *widget ) const
{
    const QStyleOptionSlider *slider;

    if ( metric == PM_SliderLength )
        return 20;
    else
        return QWindowsStyle::pixelMetric( metric, option, widget );
}

void SeekStyleBarcode::drawComplexControl( ComplexControl cc, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget ) const
{
    if( cc == CC_Slider )
    {
        painter->setRenderHints( QPainter::Antialiasing );

        if ( const QStyleOptionSlider *slider = qstyleoption_cast<const QStyleOptionSlider *>( option ) )
        {
            const SeekSlider *seekSlider = qobject_cast<const SeekSlider*>( widget );
            qreal sliderPos = -1;

            /* Get the needed subcontrols to draw the slider */
            QRect groove = subControlRect(CC_Slider, slider, SC_SliderGroove, widget);
            QRect handle = subControlRect(CC_Slider, slider, SC_SliderHandle, widget);

            /* Adjust the size of the groove so the handle stays centered */
            groove.adjust( handle.width() / 2, 0, -handle.width() / 2, 0 );

            /* Reduce the height of the groove */
            // Note: in the original 2.0.0 code, the groove had a height of 9px and to
            // comply with the original style (but still allow the widget to expand) I
            // had to remove 1 from the rect bottom.
            groove.adjust( 0, 10, 0, (qreal)-10 - 1 );
            handle.adjust( 0, (handle.height()-groove.height())/2, 0, -(handle.height()-groove.height())/2);

            if ( ( slider->subControls & SC_SliderGroove ) && groove.isValid() )
            {
                sliderPos = ( ( (qreal)groove.width() ) / (qreal)slider->maximum )
                        * (qreal)slider->sliderPosition;

                /* set the background color */
                QColor backgroundBase( slider->palette.window().color() );

                /* draw background */
                painter->drawPixmap(groove, seekSlider->barcode);

                /* draw buffering overlay */
                if ( seekSlider && seekSlider->f_buffering < 1.0 )
                {
                    QRect innerRect = groove.adjusted( 1, 1,
                                        groove.width() * ( -1.0 + seekSlider->f_buffering ) - 1, 0 );
                    QColor overlayColor = QColor( "Orange" );
                    overlayColor.setAlpha( 128 );
                    painter->setBrush( overlayColor );
                    painter->drawRoundedRect( innerRect, RADIUS, RADIUS );
                }
            }

            if ( slider->subControls & SC_SliderTickmarks ) {
                QStyleOptionSlider tmpSlider = *slider;
                tmpSlider.subControls = SC_SliderTickmarks;
                QWindowsStyle::drawComplexControl(cc, &tmpSlider, painter, widget);
            }

            if ( slider->subControls & SC_SliderHandle && handle.isValid() )
            {
                QPalette p = slider->palette;
                QColor foreground = p.color( QPalette::Active, QPalette::WindowText );
                painter->setBrush( foreground );

                QPoint base;

                QPoint leftDiff(-10,-10);
                QPoint rightDiff(10,-10);

                base = QPoint(handle.left()+handle.width()/2, handle.top());
                QPolygon upperTriangle;
                upperTriangle << base << base+leftDiff << base+rightDiff;

                base = QPoint(handle.left()+handle.width()/2, handle.bottom());
                QPolygon lowerTriangle;
                lowerTriangle << base << base-leftDiff << base-rightDiff;

                painter->drawPolygon(upperTriangle);
                painter->drawPolygon(lowerTriangle);

                if ( option->state & QStyle::State_MouseOver || (seekSlider && seekSlider->isAnimationRunning() ) )
                {
                    QPalette p = slider->palette;

                    /* draw chapters tickpoints */
                    if ( seekSlider->chapters && seekSlider->inputLength && groove.width() )
                    {
                        QColor background = p.color( QPalette::Active, QPalette::Window );
                        QColor foreground = p.color( QPalette::Active, QPalette::WindowText );
                        foreground.setHsv( foreground.hue(),
                                        ( background.saturation() + foreground.saturation() ) / 2,
                                        ( background.value() + foreground.value() ) / 2 );
                        if ( slider->orientation == Qt::Horizontal ) /* TODO: vertical */
                        {
                            QList<SeekPoint> points = seekSlider->chapters->getPoints();
                            foreach( SeekPoint point, points )
                            {
                                int x = groove.x() + point.time / 1000000.0 / seekSlider->inputLength * groove.width();
                                painter->setPen( foreground );
                                painter->setBrush( Qt::NoBrush );
                                painter->drawLine( x, slider->rect.height(), x, slider->rect.height() - CHAPTERSSPOTSIZE );
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        qWarning() << "SeekStyleBarcode: Drawing an unmanaged control";
        QWindowsStyle::drawComplexControl( cc, option, painter, widget );
    }
}
