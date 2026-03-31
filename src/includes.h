#ifndef INCLUDES_H
#define INCLUDES_H


//Include Windows.
#ifdef __WIN32
#include <Windows.h> //Only for windows systems (obviously). Only needed for console-specific functions, which are minimal.
#endif


//Include GLEW.
#include <GL/glew.h>


//Include OpenGL.
#include <GL/gl.h>
#include <GL/glu.h>


//Include GLM.
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//Include GLFW.
#include <GLFW/glfw3.h>


//Include std subheaders.
#include <bits/stdc++.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cmath>
#include <ctime>
#include <vector>
#include <array>
#include <set>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <filesystem>

#endif