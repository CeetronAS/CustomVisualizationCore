//##################################################################################################
//
//   Custom Visualization Core library
//   Copyright (C) 2011-2013 Ceetron AS
//
//   This library may be used under the terms of either the GNU General Public License or
//   the GNU Lesser General Public License as follows:
//
//   GNU General Public License Usage
//   This library is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This library is distributed in the hope that it will be useful, but WITHOUT ANY
//   WARRANTY; without even the implied warranty of MERCHANTABILITY or
//   FITNESS FOR A PARTICULAR PURPOSE.
//
//   See the GNU General Public License at <<http://www.gnu.org/licenses/gpl.html>>
//   for more details.
//
//   GNU Lesser General Public License Usage
//   This library is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Lesser General Public License as published by
//   the Free Software Foundation; either version 2.1 of the License, or
//   (at your option) any later version.
//
//   This library is distributed in the hope that it will be useful, but WITHOUT ANY
//   WARRANTY; without even the implied warranty of MERCHANTABILITY or
//   FITNESS FOR A PARTICULAR PURPOSE.
//
//   See the GNU Lesser General Public License at <<http://www.gnu.org/licenses/lgpl-2.1.html>>
//   for more details.
//
//##################################################################################################


#pragma once

#include "cvfObject.h"
#include "cvfVector2.h"
#include "cvfRect.h"

namespace cvf {



class OpenGLContext;


//==================================================================================================
//
// Interface for overlay items
//
//==================================================================================================
class OverlayItem : public Object
{
public:
    enum LayoutCorner
    {
        TOP_LEFT,       
        TOP_RIGHT,      
        BOTTOM_LEFT,    
        BOTTOM_RIGHT    
    };

    enum LayoutDirection
    {
        HORIZONTAL,    
        VERTICAL       
    };

public:
    virtual Vec2ui  sizeHint()  = 0;        // In Pixels
    virtual Vec2ui  maximumSize() = 0;      // In Pixels
    virtual Vec2ui  minimumSize() = 0;      // In Pixels

    virtual void    render(OpenGLContext* oglContext, const Vec2i& position, const Vec2ui& size) = 0;
    virtual void    renderSoftware(OpenGLContext* oglContext, const Vec2i& position, const Vec2ui& size) = 0;
    virtual bool    pick(int oglXCoord, int oglYCoord, const Vec2i& position, const Vec2ui& size);
};

}
