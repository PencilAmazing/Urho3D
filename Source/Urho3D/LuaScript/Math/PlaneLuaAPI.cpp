//
// Copyright (c) 2008-2016 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "../../Precompiled.h"

#include "../../Math/Plane.h"
#include "../../LuaScript/LuaScriptUtils.h"

#include <kaguya.hpp>

namespace Urho3D
{

void RegisterPlane(kaguya::State& lua)
{
    using namespace kaguya;

    // [Class] Plane
    lua["Plane"].setClass(UserdataMetatable<Plane>()
        .setConstructors<Plane(),
        Plane(const Plane&),
        Plane(const Vector3&, const Vector3&, const Vector3&),
        Plane(const Vector3&, const Vector3&),
        Plane(const Vector4&)>()

        .addOverloadedFunctions("Define",
            static_cast<void(Plane::*)(const Vector3&, const Vector3&, const Vector3&)>(&Plane::Define),
            static_cast<void(Plane::*)(const Vector3&, const Vector3&)>(&Plane::Define),
            static_cast<void(Plane::*)(const Vector4&)>(&Plane::Define))

        .addOverloadedFunctions("Transform",
            static_cast<void(Plane::*)(const Matrix3&)>(&Plane::Transform),
            static_cast<void(Plane::*)(const Matrix3x4&)>(&Plane::Transform),
            static_cast<void(Plane::*)(const Matrix4&)>(&Plane::Transform))

        // [Method] Vector3 Project(const Vector3& point) const
        .addFunction("Project", &Plane::Project)
        // [Method] float Distance(const Vector3& point) const
        .addFunction("Distance", &Plane::Distance)
        // [Method] Vector3 Reflect(const Vector3& direction) const
        .addFunction("Reflect", &Plane::Reflect)
        // [Method] Matrix3x4 ReflectionMatrix() const
        .addFunction("ReflectionMatrix", &Plane::ReflectionMatrix)

        .addOverloadedFunctions("Transformed",
            static_cast<Plane(Plane::*)(const Matrix3&) const>(&Plane::Transformed),
            static_cast<Plane(Plane::*)(const Matrix3x4&) const>(&Plane::Transformed),
            static_cast<Plane(Plane::*)(const Matrix4&) const>(&Plane::Transformed))

        // [Method] Vector4 ToVector4() const
        .addFunction("ToVector4", &Plane::ToVector4)

        .addProperty("normal", &Plane::normal_)
        .addProperty("absNormal", &Plane::absNormal_)
        .addProperty("d", &Plane::d_)

        .addStaticField("UP", &Plane::UP)
        );
}
}
