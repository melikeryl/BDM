/*
 Copyright (C) 2009 Frédéric Zubler, Rodney J. Douglas,
 Dennis Göhlsdorf, Toby Weston, Andreas Hauri, Roman Bauer,
 Sabina Pfister, Adrian M. Whatley & Lukas Breitwieser.

 This file is part of CX3D.

 CX3D is free software: you can redistribute it and/or modify
 it under the terms of the GNU General virtual License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 CX3D is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General virtual License for more details.

 You should have received a copy of the GNU General virtual License
 along with CX3D.  If not, see <http://www.gnu.org/licenses/>.
*/

%module(directors="1") spatialOrganization

%{
#include <stdint.h>
#include "spatial_organization/rational.h"
using namespace cx3d::spatial_organization;
%}

// import depending modules
%import "cx3d.i"

// transparently load native library - convenient for user
%include "load_library.i"
JAVA_LOAD_NATIVE_LIBRARY(cx3d_spatialOrganization);

// typemap definitions, code modifications / additions
%include "typemaps.i"
%include "cx3d_shared_ptr.i"
%include "big_integer_typemap.i"

// modifications for class Rational
%apply long long { int64_t };
%typemap(javainterfaces) cx3d::spatial_organization::Rational "ini.cx3d.spatialOrganization.interfaces.Rational"
%typemap(jstype) cx3d::spatial_organization::Rational "ini.cx3d.spatialOrganization.interfaces.Rational"
%shared_ptr(cx3d::spatial_organization::Rational);

// add the original header files here
%include "spatial_organization/rational.h"
