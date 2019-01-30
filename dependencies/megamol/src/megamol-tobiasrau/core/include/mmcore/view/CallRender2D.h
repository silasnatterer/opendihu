/*
 * CallRender2D.h
 *
 * Copyright (C) 2009 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef MEGAMOLCORE_CALLRENDER2D_H_INCLUDED
#define MEGAMOLCORE_CALLRENDER2D_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "mmcore/api/MegaMolCore.std.h"
#include "mmcore/factories/CallAutoDescription.h"
#include "vislib/math/Rectangle.h"
#include "vislib/types.h"
#include "mmcore/view/AbstractCallRender.h"
#include "mmcore/view/MouseFlags.h"
#include "mmcore/view/RenderOutput.h"


namespace megamol {
namespace core {
namespace view {


#ifdef _WIN32
#pragma warning(disable: 4250)  // I know what I am doing ...
#endif /* _WIN32 */
    /**
     * Call for rendering 2d images
     *
     * Function "Render" tells the callee to render itself into the currently
     * active opengl context (TODO: Later on it could also be a FBO).
     * The bounding box member will be set to the world space rectangle
     * containing the visible part.
     *
     * Function "GetExtents" asks the callee to fill the extents member of the
     * call (bounding boxes).
     * The renderer should not draw anything outside the bounding box
     */
    class MEGAMOLCORE_API CallRender2D : public AbstractCallRender, public RenderOutput {
    public:

        /**
         * Answer the name of the objects of this description.
         *
         * @return The name of the objects of this description.
         */
        static const char *ClassName(void) {
            return "CallRender2D";
        }

        /**
         * Gets a human readable description of the module.
         *
         * @return A human readable description of the module.
         */
        static const char *Description(void) {
            return "Call for rendering a frame";
        }

        /**
         * Answer the number of functions used for this call.
         *
         * @return The number of functions used for this call.
         */
        static unsigned int FunctionCount(void) {
            return 3;
        }

        /**
         * Answer the name of the function used for this call.
         *
         * @param idx The index of the function to return it's name.
         *
         * @return The name of the requested function.
         */
        static const char * FunctionName(unsigned int idx) {
            switch (idx) {
                case 0: return "Render";
                case 1: return "GetExtents";
                case 2: return "MouseEvent";
                default: return NULL;
            }
        }

        /** Ctor. */
        CallRender2D(void);

        /** Dtor. */
        virtual ~CallRender2D(void);

        /**
         * Gets the bounding box. As an answer to an 'GetExtents' call this
         * member holds the bounding rectangle of all visible elements created
         * by the renderer in world space. When called for 'Render' this
         * member holds the rectangle in world space which is visible on the
         * viewport.
         *
         * @return The bounding box
         */
        inline const vislib::math::Rectangle<float>& GetBoundingBox(void) const {
            return this->bbox;
        }

        /**
         * Answer the background colour
         *
         * @return The background colour as three bytes [0..255] RGB
         */
        inline const unsigned char * GetBackgroundColour(void) const {
            return this->bkgndCol;
        }

        /**
         * Answer the viewport height in pixel
         *
         * @return The viewport height in pixel
         */
        inline unsigned int GetHeight(void) const {
            return this->GetViewport().Height();
        }

        /**
         * Answer the mouse flags
         *
         * @return The mouse flags
         */
        inline MouseFlags GetMouseFlags(void) const {
            return this->mouseFlags;
        }

        /**
         * Answer the mouse x coordinate in world space
         *
         * @return The mouse x coordinate in world space
         */
        inline float GetMouseX(void) const {
            return this->mouseX;
        }

        /**
         * Answer the mouse y coordinate in world space
         *
         * @return The mouse y coordinate in world space
         */
        inline float GetMouseY(void) const {
            return this->mouseY;
        }

        /**
         * Answer the viewport width in pixel
         *
         * @return The viewport width in pixel
         */
        inline unsigned int GetWidth(void) const {
            return this->GetViewport().Width();
        }

        /**
         * Sets the bounding box member
         *
         * @param minX The minimum value along the x axis
         * @param minY The minimum value along the y axis
         * @param maxX The maximum value along the x axis
         * @param maxY The maximum value along the y axis
         */
        inline void SetBoundingBox(float minX, float minY, float maxX, float maxY) {
            this->bbox.Set(minX, minY, maxX, maxY);
        }

        /**
         * Sets the bounding box member
         *
         * @param box The bounding box rectangle
         */
        inline void SetBoundingBox(const vislib::math::Rectangle<float>& box) {
            this->bbox = box;
        }

        /**
         * Sets the background colour
         *
         * @param col Pointer to an array of three bytes holding the RGB background
         *            colour.
         */
        inline void SetBackgroundColour(const unsigned char *col) {
            this->bkgndCol[0] = col[0];
            this->bkgndCol[1] = col[1];
            this->bkgndCol[2] = col[2];
        }

        /**
         * Sets the background colour
         *
         * @param r The red colour component for the background colour [0..255]
         * @param g The green colour component for the background colour [0..255]
         * @param b The blue colour component for the background colour [0..255]
         */
        inline void SetBackgroundColour(unsigned char r, unsigned char g, unsigned char b) {
            this->bkgndCol[0] = r;
            this->bkgndCol[1] = g;
            this->bkgndCol[2] = b;
        }

        /**
         * Sets the mouse informations.
         *
         * @param x The mouse x coordinate in world space
         * @param y The mouse y coordinate in world space
         * @param flags The mouse flags
         */
        inline void SetMouseInfo(float x, float y, MouseFlags flags) {
            this->mouseX = x;
            this->mouseY = y;
            this->mouseFlags = flags;
        }

        /**
         * Assignment operator
         *
         * @param rhs The right hand side operand
         *
         * @return A reference to this
         */
        CallRender2D& operator=(const CallRender2D& rhs);

    private:

#ifdef _WIN32
#pragma warning (disable: 4251)
#endif /* _WIN32 */
        /** The bounding box */
        vislib::math::Rectangle<float> bbox;
#ifdef _WIN32
#pragma warning (default: 4251)
#endif /* _WIN32 */

        /** The background colour in RGB (bytes) */
        unsigned char bkgndCol[3];

        /** The mouse coordinates for the mouse event */
        float mouseX, mouseY;

        /** The mouse flags for the mouse event */
        MouseFlags mouseFlags;

    };
#ifdef _WIN32
#pragma warning(default: 4250)
#endif /* _WIN32 */


    /** Description class typedef */
    typedef factories::CallAutoDescription<CallRender2D> CallRender2DDescription;


} /* end namespace view */
} /* end namespace core */
} /* end namespace megamol */

#endif /* MEGAMOLCORE_CALLRENDER2D_H_INCLUDED */
