Gears Render Engine
Free to use for any purpose, open source, and great for learning shader.
By Kemi Peng <pengsoftware@gmail.com>

For detailed tutorial, please check my blog, http://pengkemi.blog.com/

Math library and GMA model loading library are found online, modified by Kemi Peng, original author unknown. GMA model loading library will be rewritten soon.

Version -- 0.5.0 08/05/2015

Only need to modify StageUtility class to create your own scence or game.

Features:
Support opaque objects
3 diffuse textures
Specular mapping
Normal mapping
Emission mapping
Lerp soft-shadow
Deferred rendering
Full screen bloom
Shininess bloom
Depth of field

Enviroment:
Based on C++, openGL 3.3, Windows 7 and Visual Studio 2008.

It requires for glew-1.12.0, which is included.
The dds file loading library(GutDDS.cpp, GutDDS.h, GutTexture.cpp) requires for DirectX's DDSURFACEDESC2 structure definition.
The input library(StandardInput.cpp, StatardInput.h) requires for DirectInput.
If you did not have D3D9.0, please modify those files.