//
// Copyright (c) 2008-2018 the Urho3D project.
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

#include "../../Graphics/Direct3D11/D3D11ShaderProgram.h"

namespace Urho3D
{

ShaderProgram::ShaderProgram(Graphics* graphics, ShaderVariation* vertexShader, ShaderVariation* pixelShader, ShaderVariation* geometryShader, ShaderVariation* tcsShader, ShaderVariation* tesShader)
{
    // Create needed constant buffers
    const unsigned* vsBufferSizes = vertexShader->GetConstantBufferSizes();
    for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
    {
        if (vsBufferSizes[i])
            vsConstantBuffers_[i] = graphics->GetOrCreateConstantBuffer(VS, i, vsBufferSizes[i]);
    }

    if (tcsShader)
    {
        const unsigned* tcsBufferSizes = tcsShader->GetConstantBufferSizes();
        for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
        {
            if (tcsBufferSizes[i] != 0 && tcsBufferSizes[i] != vsBufferSizes[i])
            {
                URHO3D_LOGERRORF("Hull shader and vertex shader constant buffer mismatch: HS size '%u', VS size '%u' at index %u", tcsBufferSizes[i], vsBufferSizes[i], i);
                URHO3D_LOGINFO("Hull and vertex shaders must use matching constant buffers");
            }
        }
    }

    if (tesShader)
    {
        const unsigned* tesBufferSizes = tesShader->GetConstantBufferSizes();
        for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
        {
            if (tesBufferSizes[i] != 0 && tesBufferSizes[i] != vsBufferSizes[i])
            {
                URHO3D_LOGERRORF("Domain shader and vertex shader constant buffer mismatch: DS size '%u', VS size '%u' at index %u", tesBufferSizes[i], vsBufferSizes[i], i);
                URHO3D_LOGINFO("Domain and vertex shaders must use matching constant buffers");
            }
        }
    }

    if (geometryShader)
    {
        const unsigned* gsBufferSizes = geometryShader->GetConstantBufferSizes();
        for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
        {
            // buffer size can be zero if it unused
            if (gsBufferSizes[i] != vsBufferSizes[i] && gsBufferSizes[i] != 0)
            {
                URHO3D_LOGERRORF("Geometry shader and vertex shader constant buffer mismatch: GS size '%u', VS size '%u' at index %u", gsBufferSizes[i], vsBufferSizes[i], i);
                URHO3D_LOGINFO("Geometry and vertex shaders must use matching constant buffers");
            }
        }
    }

    const unsigned* psBufferSizes = pixelShader->GetConstantBufferSizes();
    for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
    {
        if (psBufferSizes[i])
            psConstantBuffers_[i] = graphics->GetOrCreateConstantBuffer(PS, i, psBufferSizes[i]);
    }

    // Copy parameters, add direct links to constant buffers
    const HashMap<StringHash, ShaderParameter>& vsParams = vertexShader->GetParameters();
    for (HashMap<StringHash, ShaderParameter>::ConstIterator i = vsParams.Begin(); i != vsParams.End(); ++i)
    {
        parameters_[i->first_] = i->second_;
        parameters_[i->first_].bufferPtr_ = vsConstantBuffers_[i->second_.buffer_].Get();
    }

    const HashMap<StringHash, ShaderParameter>& psParams = pixelShader->GetParameters();
    for (HashMap<StringHash, ShaderParameter>::ConstIterator i = psParams.Begin(); i != psParams.End(); ++i)
    {
        parameters_[i->first_] = i->second_;
        parameters_[i->first_].bufferPtr_ = psConstantBuffers_[i->second_.buffer_].Get();
    }

    // Optimize shader parameter lookup by rehashing to next power of two
    parameters_.Rehash(NextPowerOfTwo(parameters_.Size()));

}

}