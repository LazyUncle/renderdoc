/******************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2019 Baldur Karlsson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

// enable these extensions if possible
//#extension GL_ARB_texture_cube_map_array : enable
//#extension GL_ARB_texture_multisample : enable

#if UINT_TEX

// these bindings are defined based on the RESTYPE_ defines in debuguniforms.h

layout(binding = 1) uniform usampler1D texUInt1D;
layout(binding = 2) uniform usampler2D texUInt2D;
layout(binding = 3) uniform usampler3D texUInt3D;
// cube = 4
layout(binding = 5) uniform usampler1DArray texUInt1DArray;
layout(binding = 6) uniform usampler2DArray texUInt2DArray;
// cube array = 7
layout(binding = 8) uniform usampler2DRect texUInt2DRect;
layout(binding = 9) uniform usamplerBuffer texUIntBuffer;
#ifdef GL_ARB_texture_multisample
layout(binding = 10) uniform usampler2DMS texUInt2DMS;
#endif

vec4 SampleTextureFloat4(int type, vec2 pos, float slice, int mipLevel, int sampleIdx, vec3 texRes,
                         uvec4 YUVDownsampleRate, uvec4 YUVAChannels)
{
  return vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

uvec4 SampleTextureUInt4(int type, vec2 pos, float slice, int mipLevel, int sampleIdx, vec3 texRes)
{
  uvec4 col;
  if(type == RESTYPE_TEX1D)
  {
    col = texelFetch(texUInt1D, int(pos.x * texRes.x), mipLevel);
  }
  else if(type == RESTYPE_TEX1DARRAY)
  {
    col = texelFetch(texUInt1DArray, ivec2(pos.x * texRes.x, slice), mipLevel);
  }
  else if(type == RESTYPE_TEX2D)
  {
    col = texelFetch(texUInt2D, ivec2(pos * texRes.xy), mipLevel);
  }
  else if(type == RESTYPE_TEXRECT)
  {
    col = texelFetch(texUInt2DRect, ivec2(pos * texRes.xy));
  }
  else if(type == RESTYPE_TEXBUFFER)
  {
    col = texelFetch(texUIntBuffer, int(pos.x * texRes.x));
  }
  else if(type == RESTYPE_TEX2DMS)
  {
#ifdef GL_ARB_texture_multisample
    if(sampleIdx < 0)
      sampleIdx = 0;

    col = texelFetch(texUInt2DMS, ivec2(pos * texRes.xy), sampleIdx);
#else
    col = uvec4(0u, 0u, 0u, 0u);
#endif
  }
  else if(type == RESTYPE_TEX2DARRAY)
  {
    col = texelFetch(texUInt2DArray, ivec3(pos * texRes.xy, slice), mipLevel);
  }
  else    // if (type == RESTYPE_TEX3D)
  {
    col = texelFetch(texUInt3D, ivec3(pos * texRes.xy, slice + 0.001f), mipLevel);
  }

  return col;
}

ivec4 SampleTextureSInt4(int type, vec2 pos, float slice, int mipLevel, int sampleIdx, vec3 texRes)
{
  return ivec4(0, 0, 0, 0);
}

#elif SINT_TEX

// these bindings are defined based on the RESTYPE_ defines in debuguniforms.h

layout(binding = 1) uniform isampler1D texSInt1D;
layout(binding = 2) uniform isampler2D texSInt2D;
layout(binding = 3) uniform isampler3D texSInt3D;
// cube = 4
layout(binding = 5) uniform isampler1DArray texSInt1DArray;
layout(binding = 6) uniform isampler2DArray texSInt2DArray;
// cube array = 7
layout(binding = 8) uniform isampler2DRect texSInt2DRect;
layout(binding = 9) uniform isamplerBuffer texSIntBuffer;
#ifdef GL_ARB_texture_multisample
layout(binding = 10) uniform isampler2DMS texSInt2DMS;
#endif

vec4 SampleTextureFloat4(int type, vec2 pos, float slice, int mipLevel, int sampleIdx, vec3 texRes,
                         uvec4 YUVDownsampleRate, uvec4 YUVAChannels)
{
  return vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

uvec4 SampleTextureUInt4(int type, vec2 pos, float slice, int mipLevel, int sampleIdx, vec3 texRes)
{
  return uvec4(0u, 0u, 0u, 0u);
}

ivec4 SampleTextureSInt4(int type, vec2 pos, float slice, int mipLevel, int sampleIdx, vec3 texRes)
{
  ivec4 col;
  if(type == RESTYPE_TEX1D)
  {
    col = texelFetch(texSInt1D, int(pos.x * texRes.x), mipLevel);
  }
  else if(type == RESTYPE_TEX1DARRAY)
  {
    col = texelFetch(texSInt1DArray, ivec2(pos.x * texRes.x, slice), mipLevel);
  }
  else if(type == RESTYPE_TEX2D)
  {
    col = texelFetch(texSInt2D, ivec2(pos * texRes.xy), mipLevel);
  }
  else if(type == RESTYPE_TEXRECT)
  {
    col = texelFetch(texSInt2DRect, ivec2(pos * texRes.xy));
  }
  else if(type == RESTYPE_TEXBUFFER)
  {
    col = texelFetch(texSIntBuffer, int(pos.x * texRes.x));
  }
  else if(type == RESTYPE_TEX2DMS)
  {
#ifdef GL_ARB_texture_multisample
    if(sampleIdx < 0)
      sampleIdx = 0;

    col = texelFetch(texSInt2DMS, ivec2(pos * texRes.xy), sampleIdx);
#else
    col = ivec4(0, 0, 0, 0);
#endif
  }
  else if(type == RESTYPE_TEX2DARRAY)
  {
    col = texelFetch(texSInt2DArray, ivec3(pos * texRes.xy, slice), mipLevel);
  }
  else    // if (type == RESTYPE_TEX3D)
  {
    col = texelFetch(texSInt3D, ivec3(pos * texRes.xy, slice + 0.001f), mipLevel);
  }

  return col;
}

#else

// these bindings are defined based on the RESTYPE_ defines in debuguniforms.h

layout(binding = 1) uniform sampler1D tex1D;
layout(binding = 2) uniform sampler2D tex2D;
layout(binding = 3) uniform sampler3D tex3D;
layout(binding = 4) uniform samplerCube texCube;
layout(binding = 5) uniform sampler1DArray tex1DArray;
layout(binding = 6) uniform sampler2DArray tex2DArray;
#ifdef GL_ARB_texture_cube_map_array
layout(binding = 7) uniform samplerCubeArray texCubeArray;
#endif
layout(binding = 8) uniform sampler2DRect tex2DRect;
layout(binding = 9) uniform samplerBuffer texBuffer;
#ifdef GL_ARB_texture_multisample
layout(binding = 10) uniform sampler2DMS tex2DMS;
#endif

vec4 SampleTextureFloat4(int type, vec2 pos, float slice, int mipLevel, int sampleIdx, vec3 texRes,
                         uvec4 YUVDownsampleRate, uvec4 YUVAChannels)
{
  vec4 col;
  if(type == RESTYPE_TEX1D)
  {
    col = textureLod(tex1D, pos.x, float(mipLevel));
  }
  else if(type == RESTYPE_TEX1DARRAY)
  {
    col = textureLod(tex1DArray, vec2(pos.x, slice), float(mipLevel));
  }
  else if(type == RESTYPE_TEX2D)
  {
    col = textureLod(tex2D, pos, float(mipLevel));
  }
  else if(type == RESTYPE_TEXRECT)
  {
    col = texelFetch(tex2DRect, ivec2(pos * texRes.xy));
  }
  else if(type == RESTYPE_TEXBUFFER)
  {
    col = texelFetch(texBuffer, int(pos.x * texRes.x));
  }
  else if(type == RESTYPE_TEX2DMS)
  {
#ifdef GL_ARB_texture_multisample
    if(sampleIdx < 0)
    {
      int sampleCount = -sampleIdx;

      // worst resolve you've seen in your life
      // it's manually unrolled because doing it as a dynamic loop on
      // sampleCount seems to produce crazy artifacts on nvidia - probably a compiler bug
      if(sampleCount == 2)
      {
        col += 0.5f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 0);
        col += 0.5f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 1);
      }
      else if(sampleCount == 4)
      {
        col += 0.25f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 0);
        col += 0.25f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 1);
        col += 0.25f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 2);
        col += 0.25f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 3);
      }
      else if(sampleCount == 8)
      {
        col += 0.125f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 0);
        col += 0.125f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 1);
        col += 0.125f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 2);
        col += 0.125f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 3);
        col += 0.125f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 4);
        col += 0.125f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 5);
        col += 0.125f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 6);
        col += 0.125f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 7);
      }
      else if(sampleCount == 16)
      {
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 0);
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 1);
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 2);
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 3);
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 4);
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 5);
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 6);
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 7);
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 8);
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 9);
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 10);
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 11);
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 12);
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 13);
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 14);
        col += 0.0625f * texelFetch(tex2DMS, ivec2(pos * texRes.xy), 15);
      }
    }
    else
    {
      col = texelFetch(tex2DMS, ivec2(pos * texRes.xy), sampleIdx);
    }
#else
    col = vec4(0.0f, 0.0f, 0.0f, 0.0f);
#endif
  }
  else if(type == RESTYPE_TEX2DARRAY)
  {
    col = textureLod(tex2DArray, vec3(pos, slice), float(mipLevel));
  }
  else if(type == RESTYPE_TEX3D)
  {
    col = textureLod(tex3D, vec3(pos, (slice + 0.001f) / texRes.z), float(mipLevel));
  }
  else if(type == RESTYPE_TEXCUBE)
  {
    vec3 cubeCoord = CalcCubeCoord(pos, int(slice));

    col = textureLod(texCube, cubeCoord, float(mipLevel));
  }
  else    // type == RESTYPE_TEXCUBEARRAY
  {
#ifdef GL_ARB_texture_cube_map_array
    vec3 cubeCoord = CalcCubeCoord(pos, int(slice) % 6);
    vec4 arrayCoord = vec4(cubeCoord, int(slice) / 6);

    col = textureLod(texCubeArray, arrayCoord, float(mipLevel));
#else
    col = vec4(0.0f, 0.0f, 0.0f, 0.0f);
#endif
  }

  return col;
}

uvec4 SampleTextureUInt4(int type, vec2 pos, float slice, int mipLevel, int sampleIdx, vec3 texRes)
{
  return uvec4(0u, 0u, 0u, 0u);
}

ivec4 SampleTextureSInt4(int type, vec2 pos, float slice, int mipLevel, int sampleIdx, vec3 texRes)
{
  return ivec4(0, 0, 0, 0);
}

#endif
