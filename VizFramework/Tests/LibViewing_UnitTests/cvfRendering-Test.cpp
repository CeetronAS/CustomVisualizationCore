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


#include "cvfBase.h"
#include "cvfRendering.h"
#include "cvfVector3.h"
#include "cvfArray.h"
#include "cvfScene.h"
#include "cvfModelBasicList.h"
#include "cvfPart.h"
#include "cvfDrawableGeo.h"
#include "cvfCamera.h"
#include "cvfPartRenderHintCollection.h"
#include "cvfOverlayColorLegend.h"
#include "cvfOverlayTextBox.h"
#include "cvfFixedAtlasFont.h"

#include "gtest/gtest.h"

using namespace cvf;



//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
TEST(RenderingTest, BasicConstruction)
{
    Rendering r("Test rendering");
    ASSERT_EQ(0, r.refCount());
    EXPECT_EQ(Viewport::CLEAR_COLOR_DEPTH, r.clearMode());
    EXPECT_STREQ("Test rendering", r.renderingName().toAscii().ptr());
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
TEST(RenderingTest, name)
{
    Rendering r1;
    Rendering r2("Test rendering");

    EXPECT_STREQ("", r1.renderingName().toAscii().ptr());
    EXPECT_STREQ("Test rendering", r2.renderingName().toAscii().ptr());

    r1.setRenderingName("Main rendering");
    EXPECT_STREQ("Main rendering", r1.renderingName().toAscii().ptr());
}



//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
TEST(RenderingTest, SetGetClearMode)
{
    Rendering r;
    r.setClearMode(Viewport::CLEAR_DEPTH);
    EXPECT_EQ(Viewport::CLEAR_DEPTH, r.clearMode());
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
TEST(RenderingTest, MinimalLifeCycle)
{
    ref<Scene> myScene = new Scene;
    EXPECT_EQ(1, myScene->refCount());

    {
        ref<Rendering> myRendering = new Rendering;
        EXPECT_EQ(NULL, myRendering->scene());

        myRendering->setScene(myScene.p());
        EXPECT_EQ(2, myScene->refCount());

        EXPECT_EQ(myScene.p(), myRendering->scene());
    }

    EXPECT_EQ(1, myScene->refCount());
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
TEST(RenderingTest, SetupSimpleSceneAndFindVisible)
{
    Vec3fArray* verts = new Vec3fArray;
    verts->reserve(3);
    verts->add(Vec3f(0, 0, 0));
    verts->add(Vec3f(1, 0, 0));
    verts->add(Vec3f(1, 1, 0));

    Vec3fArray* norms = new Vec3fArray;
    norms->resize(3);
    norms->set(0, Vec3f::Z_AXIS);
    norms->set(1, Vec3f::Z_AXIS);
    norms->set(2, Vec3f::Z_AXIS);

    DrawableGeo* myGeo = new DrawableGeo;
    myGeo->setFromTriangleVertexArray(verts);
    myGeo->setNormalArray(norms);

    Part* myPart = new Part;
    myPart->setDrawable(myGeo);
    myPart->updateBoundingBox();

    ref<ModelBasicList> myModel = new ModelBasicList;
    myModel->addPart(myPart);
    myModel->updateBoundingBoxesRecursive();

    ref<Scene> myScene = new Scene;
    myScene->addModel(myModel.p());

    Rendering myRendering;
    myRendering.setScene(myScene.p());

    PartRenderHintCollection visibleParts;
    Camera camera;
    CullSettings cullSettings;
    cullSettings.enableViewFrustumCulling(false);
    myScene->findVisibleParts(&visibleParts, camera, cullSettings, 0xffffffff);

    ASSERT_EQ(1u, visibleParts.count());

    Part* visiblePart = visibleParts.part(0);
    ASSERT_EQ(myPart, visiblePart);
}


//--------------------------------------------------------------------------------------------------
/// 
//--------------------------------------------------------------------------------------------------
TEST(RenderingTest, OverlayItemsAddRemove)
{
    Rendering myRendering;

    EXPECT_EQ(0, myRendering.overlayItemCount());

    ref<Font> myFont = new FixedAtlasFont(FixedAtlasFont::STANDARD);
    ref<OverlayColorLegend> col = new OverlayColorLegend(myFont.p());

    myRendering.addOverlayItem(col.p(), OverlayItem::TOP_RIGHT, OverlayItem::HORIZONTAL);
    EXPECT_EQ(1, myRendering.overlayItemCount());
    myRendering.addOverlayItem(col.p(), OverlayItem::TOP_LEFT, OverlayItem::VERTICAL);
    EXPECT_EQ(2, myRendering.overlayItemCount());

    OverlayItem* item = myRendering.overlayItem(0, NULL, NULL);
    EXPECT_EQ(item, col.p());
    EXPECT_EQ(item, myRendering.overlayItem(1, NULL, NULL));

    {
        OverlayItem::LayoutCorner corner;
        OverlayItem::LayoutDirection dir;

        OverlayItem* item = myRendering.overlayItem(0, &corner, &dir);

        EXPECT_EQ(item, col.p());
        EXPECT_EQ(OverlayItem::TOP_RIGHT, corner);
        EXPECT_EQ(OverlayItem::HORIZONTAL, dir);
    }

    {
        OverlayItem::LayoutCorner corner;
        OverlayItem::LayoutDirection dir;

        OverlayItem* item = myRendering.overlayItem(1, &corner, &dir);

        EXPECT_EQ(item, col.p());
        EXPECT_EQ(OverlayItem::TOP_LEFT, corner);
        EXPECT_EQ(OverlayItem::VERTICAL, dir);
    }

    {
        OverlayItem::LayoutCorner corner;
        OverlayItem::LayoutDirection dir;

        const Rendering* constRendering = &myRendering;
        const OverlayItem* item = constRendering->overlayItem(1, &corner, &dir);

        EXPECT_EQ(item, col.p());
        EXPECT_EQ(OverlayItem::TOP_LEFT, corner);
        EXPECT_EQ(OverlayItem::VERTICAL, dir);
    }

    myRendering.removeAllOverlayItems();
    EXPECT_EQ(0, myRendering.overlayItemCount());
}

