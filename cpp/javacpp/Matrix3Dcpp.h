/*
 * @(#)Matrix3D.java  1.2 96/12/06
 *
 * Copyright (c) 1994-1996 Sun Microsystems, Inc. All Rights Reserved.
 *
 * Sun grants you ("Licensee") a non-exclusive, royalty free, license to use,
 * modify and redistribute this software in source and binary code form,
 * provided that i) this copyright notice and license appear on all copies of
 * the software; and ii) Licensee does not utilize the software in a manner
 * which is disparaging to Sun.
 *
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY
 * IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN AND ITS LICENSORS SHALL NOT BE
 * LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THE SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL SUN OR ITS
 * LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT,
 * INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER
 * CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF
 * OR INABILITY TO USE SOFTWARE, EVEN IF SUN HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 * This software is not designed or intended for use in on-line control of
 * aircraft, air traffic, aircraft navigation or aircraft communications; or in
 * the design, construction, operation or maintenance of any nuclear
 * facility. Licensee represents and warrants that it will not use or
 * redistribute the Software for such purposes.
 */


#include <javacpp/JMath.h>


/** A fairly conventional 3D matrix object that can transform sets of
  3D points and perform a variety of manipulations on the transform */


#define Matrix3D_pi 3.14159265


class Matrix3D {
 public:
  //
  float xx, xy, xz, xo;
  float yx, yy, yz, yo;
  float zx, zy, zz, zo;
  //
  /** Create a new unit matrix */
  Matrix3D ();
  /** Scale by f in all dimensions */
  void scale(float f);
  /** Scale along each axis independently */
  void scale(float xf, float yf, float zf);
  /** Translate the origin */
  void translate(float x, float y, float z);
  /** rotate theta degrees about the y axis */
  void yrot(double theta);
  /** rotate theta degrees about the x axis */
  void xrot(double theta);
  /** rotate theta degrees about the z axis */
  void zrot(double theta);
  /** Multiply this matrix by a second: M = M*R */
  void mult(Matrix3D *rhs);
  /** Reinitialize to the unit matrix */
  void unit();
  /** Transform nvert points from v into tv.  v contains the input
    coordinates in floating point.  Three successive entries in
    the array constitute a point.  tv ends up holding the transformed
    points as integers; three successive entries per point */
  void transform(float v[], int tv[], int nvert);
};



