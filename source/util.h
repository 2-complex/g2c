
/*
  g2c Copyright (C) 2012 2-Complex

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/



#ifndef _UTIL_
#define _UTIL_

#include <string>
#include <vector>

double currentTime();
double clamp(double x, double m, double M);
double min(double x, double y);
double max(double x, double y);
double myfmod(double x, double m);
double sCurve(double x);
unsigned char floatToByte(double t);
std::string floatToString(double t);
int stringToInt(const std::string& s);
double stringToFloat(const std::string& s);
std::string intToString(int n);
std::string intToStringWithCommas(int n);
std::string stringRepr(const std::string& s);
bool charInString(char c, const char* s);
std::string flattenWhitespace(const char* s);
void seedRand();
int randInt(int m);
std::string directoryOfPath(const std::string& path);
std::string fileOfPath(const std::string& path);
std::string relativePath(const std::string& source, const std::string& target);
std::vector<std::string> components(const std::string& path);

#endif

