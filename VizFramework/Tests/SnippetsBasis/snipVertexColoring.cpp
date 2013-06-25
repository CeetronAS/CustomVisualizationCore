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


#include "cvfLibCore.h"
#include "cvfLibGeometry.h"
#include "cvfLibRender.h"
#include "cvfLibViewing.h"

#include "cvfuInputEvents.h"

#include "snipVertexColoring.h"

namespace snip {


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
bool VertexColoring::onInitialize()
{
    ref<ModelBasicList> myModel = new ModelBasicList;

    // Create the effect
    {
        m_effect = new Effect;

        ref<RenderStateMaterial_FF> mat = new RenderStateMaterial_FF(Color3::BLUE);
        mat->enableColorMaterial(true);
        m_effect->setRenderState(mat.p());

        ref<RenderStateLighting_FF> lighting = new RenderStateLighting_FF;
        m_effect->setRenderState(lighting.p());
    }


    // "Normal" geometry
    {
        GeometryBuilderDrawableGeo builder;
        GeometryUtils::createBox(Vec3f(0,0,0), 2.0, 2.0, 2.0, &builder);
        ref<DrawableGeo> geo = builder.drawableGeo();
        geo->computeNormals();

        ref<Part> part = new Part;
        part->setDrawable(geo.p());
        part->setEffect(m_effect.p());

        myModel->addPart(part.p());
    }

    // Geometry with per vertex colors
    {
        GeometryBuilderDrawableGeo builder;
        GeometryUtils::createBox(Vec3f(3,0,0), 2.0, 2.0, 2.0, &builder);
        ref<DrawableGeo> geo = builder.drawableGeo();
        geo->computeNormals();

        const Color3ub colorTable[] = 
        {
            Color3ub(Color3::ORANGE),
            Color3ub(Color3::FOREST_GREEN),
            Color3ub(Color3::SKY_BLUE),
            Color3ub(Color3::ORCHID),
            Color3ub(Color3::BROWN),
            Color3ub(Color3::DARK_RED)
        };
        const int colorTableSize = sizeof(colorTable)/sizeof(Color3ub);

        size_t vertexCount = geo->vertexCount();
        ref<Color3ubArray> colorArray = new Color3ubArray;
        colorArray->resize(vertexCount);
        colorArray->setAll(Color3::WHITE);

        size_t i;
        for (i = 0; i < vertexCount; i++)
        {
            Color3ub c = colorTable[(i/4) % colorTableSize];
            colorArray->set(i, c);
        }
        geo->setColorArray(colorArray.p());

        ref<Part> part = new Part;
        part->setDrawable(geo.p());
        part->setEffect(m_effect.p());

        myModel->addPart(part.p());
    }

    // Geometry with per vertex colors (using ScalarToColorMapper)
    {
        BoxGenerator gen;
        gen.setMinMax(Vec3d(2,-1,2), Vec3d(4, 1, 4));
        gen.setSubdivisions(10, 10, 10);

        GeometryBuilderDrawableGeo builder;
        gen.generate(&builder);
        ref<DrawableGeo> geo = builder.drawableGeo();
        geo->computeNormals();

        cvf::Color3ubArray colors;
        colors.reserve(4);
        colors.add(Color3::BLUE);
        colors.add(Color3::CYAN);
        colors.add(Color3::YELLOW);
        colors.add(Color3::RED);

        ScalarMapperUniformLevels mapper;
        mapper.setColors(colors);
        mapper.setRange(0, 3);

        size_t vertexCount = geo->vertexCount();
        ref<Color3ubArray> colorArray = new Color3ubArray;
        colorArray->resize(vertexCount);
        colorArray->setAll(Color3::WHITE);

        size_t i;
        for (i = 0; i < vertexCount; i++)
        {
            Vec3f v = geo->vertexArray()->get(i);
            float dist = v.pointDistance(Vec3f(2, -1, 2));
            Color3ub c = mapper.mapToColor(dist);
            colorArray->set(i, c);
        }

        geo->setColorArray(colorArray.p());

        ref<Part> part = new Part;
        part->setDrawable(geo.p());
        part->setEffect(m_effect.p());

        myModel->addPart(part.p());
    }

    // Geometry without normals
    {
        GeometryBuilderDrawableGeo builder;
        GeometryUtils::createBox(Vec3f(0,0,3), 2.0, 2.0, 2.0, &builder);
        ref<DrawableGeo> geo = builder.drawableGeo();
        geo->setNormalArray(NULL);

        ref<Part> part = new Part;
        part->setDrawable(geo.p());
        part->setEffect(m_effect.p());

        myModel->addPart(part.p());
    }

    

    myModel->updateBoundingBoxesRecursive();

    m_renderSequence->firstRendering()->scene()->addModel(myModel.p());
    m_renderSequence->firstRendering()->addOverlayItem(new OverlayAxisCross(m_camera.p(), new FixedAtlasFont(FixedAtlasFont::STANDARD)), OverlayItem::BOTTOM_LEFT, OverlayItem::HORIZONTAL);

    BoundingBox bb = m_renderSequence->boundingBox();
    if (bb.isValid())
    {
        m_camera->fitView(bb, Vec3d::Y_AXIS, Vec3d::Z_AXIS);
    }

    addEdgesRendering();
       
    return true;
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void VertexColoring::addEdgesRendering()
{
    ref<RenderStatePolygonOffset> polyOffset = new RenderStatePolygonOffset;
    polyOffset->configureLineNegativeOffset();

    ref<Effect> lineEffect = new Effect;
    lineEffect->setRenderState(new RenderStatePolygonMode(RenderStatePolygonMode::LINE));
    lineEffect->setRenderState(polyOffset.p());
    lineEffect->setRenderState(new RenderStateDepth(true, RenderStateDepth::LEQUAL));

    ref<RenderStateMaterial_FF> lineMat = new RenderStateMaterial_FF;
    lineMat->setAmbientAndDiffuse(Color3f::WHITE);
    lineEffect->setRenderState(lineMat.p());
    lineEffect->setRenderState(new RenderStateLighting_FF(true));

    ref<Rendering> newRendering = new Rendering;
    newRendering->setCamera(m_camera.p());
    newRendering->setEffectOverride(lineEffect.p());
    newRendering->setScene(m_renderSequence->rendering(0)->scene());
    newRendering->setClearMode(Viewport::DO_NOT_CLEAR);

    m_renderSequence->addRendering(newRendering.p());
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
void VertexColoring::onKeyPressEvent(KeyEvent* keyEvent)
{
    Key key = keyEvent->key();
    char character = keyEvent->character();

    if (key == Key_L)
    {
        bool lightingOn = (character == 'l') ? true : false;

        RenderStateLighting_FF* rsLighting = dynamic_cast<RenderStateLighting_FF*>(m_effect->renderStateOfType(RenderState::LIGHTING_FF));
        rsLighting->enable(lightingOn);
    }

    if (key == Key_C)
    {
        bool colorMaterialOn = (character == 'c') ? true : false;

        RenderStateMaterial_FF* rsMaterial = dynamic_cast<RenderStateMaterial_FF*>(m_effect->renderStateOfType(RenderState::MATERIAL_FF));
        rsMaterial->enableColorMaterial(colorMaterialOn);
    }

    keyEvent->setRequestedAction(REDRAW);
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
std::vector<cvf::String> VertexColoring::helpText() const
{
    std::vector<String> help;
    help.push_back(String("l/L - to toggle lighting on/off"));
    help.push_back(String("c/C - to toggle color material on/off"));

    return help;
    
}


} // namespace snip

