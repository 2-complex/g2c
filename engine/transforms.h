#include "lin/lin.h"

namespace cello
{


/*! Computes a perspective transformation matrix given the angular height
    of the frustum, the aspect ratio, and the near and far clipping planes.*/
Mat4 perspective(double angle, double aspect, double near, double far);

/*! Computes an orthographic projection matrix given the coordinates of the
    planes defining the viewing volume.*/
Mat4 orthographic(double left, double right, double bottom, double top, double near, double far);

/*! Computes a perspective transformation matrix given the left, right,
    top, bottom, near and far clipping planes.*/
Mat4 frustum(double left, double right, double bottom, double top, double near, double far);

/*! Computes a look-at transformation.*/
Mat4 lookAt(const Vec3& eye, const Vec3& target, const Vec3& up);

/*! Creates a matrix which translates by the given vector v.*/
Mat4 translate(const Vec3& v);

/*! Creates a matrix which scales in each dimension by an amount given by
    the corresponding entry in the given vector.*/
Mat4 scale(const Vec3& v);

/*! Creates a matrix which rotates around the x-axis by the given angle.*/
Mat4 rotateX(double theta);

/*! Creates a matrix which rotates around the y-axis by the given angle.*/
Mat4 rotateY(double theta);

/*! Creates a matrix which rotates around the z-axis by the given angle.*/
Mat4 rotateZ(double theta);

/*! Creates a matrix which rotates around the given axis by the given.*/
Mat4 axisRotate(const Vec3& axis, double theta);

/*! Retrieves the upper 3x3 matrix of a 4x4 transformation matrix.*/
Mat3 upper3x3(const Mat4& m);

} // end namespace


