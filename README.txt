When I started to create this engine, I was not that expierenced. Someparts of this engine are not well-constructed.
I prepare to redo all my work if I have enough time
Gears Render Engine
Free to use for any purpose, open source, and great for learning shader.
By Kemi Peng <pengsoftware@gmail.com>

For detailed tutorial, please check my blog, http://pengkemi.blog.com/

Now, the demo is only focus on visual effect.
Use 1,2,3 to switch between different stage. 2 is for test of depth of field, 3 is for test of deferred rendering.
Use 4 and 5 to turn full screen bloom effect on or off.
Use 6 and 7 to turn shininess bloom effect on or off.
Use 8 and 9 to turn depth of field effect on or off.
Use Q and E to move camera.
Use W, A, S, D to move objects.
Use I, J, K, L to tilt objects.

Math library and GMA model loading library are found online, modified by Kemi Peng, original author unknown. GMA model loading library will be rewritten soon.

Version -- 0.5.0 08/05/2015

Only need to modify StageUtility class to create your own scence or game.

Features:
Support opaque objects
3 diffuse textures
Specular mapping
Normal mapping
Emission mapping
Percentage Close Filter Lerp soft-shadow
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
