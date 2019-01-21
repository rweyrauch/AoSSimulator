/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <cmath>
#include <cassert>
#include <stdexcept>

namespace Math {

// coordinate names
enum 
{
    COORD_X = 0,
    COORD_Y = 1,
    COORD_Z = 2,
    COORD_W = 3,
    
    COORD_U = 0,
    COORD_V = 1,
    
    COORD_S = 0,
    COORD_T = 1,
};

// How close to zero is to considered zero?
const float EPSILON = 1.0e-6f;
const float INFINITE = 1.0e38f;

// Pi
const float PI = (float)M_PI;

template<class T> inline T Clamp(T val, T min, T max)
{
    return ((val > max) ? max : ((val < min) ? min : val));
}

template<class T> inline bool RangeClosed(T val, T min, T max)
{
    return ((val <= max) && (val >= min));
}
template<class T> inline bool RangeOpen(T val, T min, T max)
{
    return ((val < max) && (val > min));
}

template<class T> inline T Min(T val1, T val2)
{
  return ((val1 < val2) ? val1 : val2);
}

template<class T> inline T Max(T val1, T val2)
{
  return ((val1 > val2) ? val1 : val2);
}

template<class T> inline T Abs(T val)
{
  return (val < static_cast<T>(0) ? -val : val);
}

template<class T> inline bool Equal(T val1, T val2, T tol)
{
    return (val1 >= (val2 - tol)) && (val1 <= (val2 + tol));
}

template<class T> inline T Sqr(T val)
{
    return (val * val);
}

template<class T> inline T DegToRad(T deg)
{
    return (deg * static_cast<T>(M_PI) / static_cast<T>(180));
}

template<class T> inline T RadToDeg(T rad)
{   
    return (rad * static_cast<T>(180) / static_cast<T>(M_PI));
}

inline bool IsPowOfTwo(unsigned int size)
{
    return !((size - 1) & size);
}

inline unsigned int NextPowOfTwo(unsigned int val)
{
     val -= 1;

     val |= val >> 16;
     val |= val >> 8;
     val |= val >> 4;
     val |= val >> 2;
     val |= val >> 1;

     return val + 1;
}

template<class T> T Lerp(T t, T a, T b)
{
    return (a + t * (b - a));
}

template<class T> T SCurve(T t)
{
    return (t * t * (static_cast<T>(3.0) - static_cast<T>(2.0) * t));
}

template<class T> void Swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

inline float Trunc(float val)
{
    if (val >= 0.0f)
        return floorf(val);
    return ceilf(val);
}

inline float Rand()
{
    return (float)rand()/(float)RAND_MAX;
}

inline float RandRange(float rangeMin, float rangeMax)
{
    return Rand() * (rangeMax - rangeMin) + rangeMin;
}

float RandNormalDist(float variance);

///////////////////////////////////////////////////////////////////////////////
//
// Point 3
//
///////////////////////////////////////////////////////////////////////////////

class  Point3
{
public:

    Point3() {}

    Point3(const Point3& v) :
        m_x(v.m_x),
        m_y(v.m_y),
        m_z(v.m_z) {}

    Point3(float x, float y, float z) :
        m_x(x),
        m_y(y),
        m_z(z) {}

    inline Point3& operator = (const Point3& v)
    {
        m_x = v.m_x;
        m_y = v.m_y;
        m_z = v.m_z;
        return *this;
    }

    inline Point3& operator += (const Point3& v)
    {
        m_x += v.m_x;
        m_y += v.m_y;
        m_z += v.m_z;
        return *this;
    }

    inline Point3& operator -= (const Point3& v)
    {
        m_x -= v.m_x;
        m_y -= v.m_y;
        m_z -= v.m_z;
        return *this;
    }

    inline Point3& operator *= (float scale)
    {
        m_x *= scale;
        m_y *= scale;
        m_z *= scale;
        return *this;
    }

    inline Point3 operator + (const Point3& v) const
    {
        return Point3(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
    }

    inline Point3 operator - (const Point3& v) const
    {
        return Point3(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
    }

    inline Point3 operator * (float scale) const
    {
        return Point3(m_x * scale, m_y * scale, m_z * scale);
    }

    friend Point3 operator * (float scale, const Point3& v)
    {
        return Point3(v.x() * scale, v.y() * scale, v.z() * scale);
    }

    inline bool operator == (const Point3& v) const
    {
        return ((m_x == v.m_x) && (m_y == v.m_y) && (m_z == v.m_z));
    }

    inline bool operator != (const Point3& v) const
    {
        return !(*this == v);
    }

    inline bool equal(const Point3& v, float e = EPSILON) const
    {
        return (Equal(m_x, v.m_x, e) &&
                Equal(m_y, v.m_y, e) &&
                Equal(m_z, v.m_z, e));
    }

    inline float distanceSquare(const Point3& v) const
    {
        return (Sqr(m_x - v.m_x) + Sqr(m_y - v.m_y) + Sqr(m_z - v.m_z));
    }

    inline float distance(const Point3& v) const
    {
        return sqrtf(distanceSquare(v));
    }

    inline void set(const Point3& v)
    {
        m_x = v.m_x;
        m_y = v.m_y;
        m_z = v.m_z;
    }

    inline void set(float x, float y, float z)
    {
        m_x = x;
        m_y = y;
        m_z = z;
    }

    inline operator float* ()
    {
        return (float*)&m_x;
    }

    inline operator const float* () const
    {
        return (const float*)&m_x;
    }

    inline const float& at(size_t i) const
    {
        if (i == 0) return m_x;
        else if (i == 1) return m_y;
        else if (i == 2) return m_z;
        else assert(0);
    }

    inline float& x() { return m_x; }
    inline const float& x() const { return m_x; }
    inline float& y() { return m_y; }
    inline const float& y() const { return m_y; }
    inline float& z() { return m_z; }
    inline const float& z() const { return m_z; }

    inline float& at(int index) { switch (index) { case 0: return m_x; case 1: return m_y; case 2: return m_z; }; throw std::out_of_range("Invalid index.");}
    inline const float& at(int index) const { switch (index) { case 0: return m_x; case 1: return m_y; case 2: return m_z; }; throw std::out_of_range("Invalid index.");}

protected:

    float m_x, m_y, m_z;

};

///////////////////////////////////////////////////////////////////////////////
//
// Vector 3
//
///////////////////////////////////////////////////////////////////////////////

class  Vector3
{
public:

    Vector3() {}
    
    Vector3(const Vector3& v) :
        m_x(v.m_x),
        m_y(v.m_y),
        m_z(v.m_z) {}

    Vector3(const float* v) :
        m_x(*(v+0)),
        m_y(*(v+1)),
        m_z(*(v+2)) {}

    Vector3(float x, float y, float z) :
        m_x(x),
        m_y(y),
        m_z(z) {}

    Vector3(const Point3& origin, const Point3& end) :
        m_x(end.x() - origin.x()),
        m_y(end.y() - origin.y()),
        m_z(end.z() - origin.z())
    {}

    inline Vector3& operator = (const Vector3& v)
    {
        m_x = v.m_x;
        m_y = v.m_y;
        m_z = v.m_z;
        return *this;
    }

    inline Vector3& operator += (const Vector3& v)
    {
        m_x += v.m_x;
        m_y += v.m_y;
        m_z += v.m_z;
        return *this;
    }

    inline Vector3& operator -= (const Vector3& v)
    {
        m_x -= v.m_x;
        m_y -= v.m_y;
        m_z -= v.m_z;
        return *this;
    }

    inline Vector3& operator *= (float scale)
    {
        m_x *= scale;
        m_y *= scale;
        m_z *= scale;
        return *this;
    }

    inline Vector3& operator /= (float scale)
    {
        const float invScale = 1.0f / scale;
        m_x *= invScale;
        m_y *= invScale;
        m_z *= invScale;
        return *this;
    }

    inline Vector3 operator - () const
    {
        return Vector3(-m_x, -m_y, -m_z);
    }

    inline Vector3 operator + () const
    {
        return Vector3(+m_x, +m_y, +m_z);
    }

    inline Vector3 operator + (const Vector3& v) const
    {
        return Vector3(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
    }

    inline Vector3 operator - (const Vector3& v) const
    {
        return Vector3(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
    }

    inline Vector3 operator * (float scale) const
    {
        return Vector3(m_x * scale, m_y * scale, m_z * scale);
    }

    inline Vector3 operator / (float scale) const
    {
        const float invScale = 1.0f / scale;
        return Vector3(m_x * invScale, m_y * invScale, m_z * invScale);
    }

    friend Vector3 operator * (float scale, const Vector3& v)
    {
        return Vector3(v.x() * scale, v.y() * scale, v.z() * scale);
    }

    inline bool operator == (const Vector3& v) const
    {
        return ((m_x == v.m_x) && (m_y == v.m_y) && (m_z == v.m_z));
    }

    inline bool operator != (const Vector3& v) const
    {
        return !(*this == v);
    }

    inline bool equal(const Vector3& v, float e = EPSILON) const
    {
        return (Equal(m_x, v.m_x, e) &&
                 Equal(m_y, v.m_y, e) &&
                 Equal(m_z, v.m_z, e));
    }

    float normalize();

    inline float length() const
    {
        return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z);
    }

    inline float lengthSquare() const
    {
        return (m_x * m_x + m_y * m_y + m_z * m_z);
    }

    inline float dot(const Vector3& v) const
    {
        return (m_x * v.m_x + m_y * v.m_y + m_z * v.m_z);
    }
    inline float dot(const Point3& p) const
    {
        return (m_x * p.x() + m_y * p.y() + m_z * p.z());
    }

    inline float distanceSquare(const Vector3& v) const
    {
        return (Sqr(m_x - v.m_x) + Sqr(m_y - v.m_y) + Sqr(m_z - v.m_z));
    }

    inline float distance(const Vector3& v) const
    {
        return sqrtf(distanceSquare(v));
    }

    inline void set(const Vector3& v)
    {
        m_x = v.m_x;
        m_y = v.m_y;
        m_z = v.m_z;
    }

    inline void set(float x, float y, float z)
    {
        m_x = x;
        m_y = y;
        m_z = z;
    }

    inline const float& at(size_t i) const
    {
        if (i == 0) return m_x;
        else if (i == 1) return m_y;
        else if (i == 2) return m_z;
        else assert(0);
    }

    inline float& x() { return m_x; }
    inline const float& x() const { return m_x; }
    inline float& y() { return m_y; }
    inline const float& y() const { return m_y; }
    inline float& z() { return m_z; }
    inline const float& z() const { return m_z; }

    inline float& at(int index) { switch (index) { case 0: return m_x; case 1: return m_y; case 2: return m_z; }; throw std::out_of_range("Invalid index.");}
    inline const float& at(int index) const { switch (index) { case 0: return m_x; case 1: return m_y; case 2: return m_z; }; throw std::out_of_range("Invalid index.");}

    static float Dot(const Vector3& v1, const Vector3& v2);
    static float Dot(const Vector3& v1, const Point3& p2);

    static Vector3 Cross(const Vector3& v1, const Vector3& v2);

    // Compute reflection vector from unit incident and normal vectors.
    static Vector3 Reflect(const Vector3& inc, const Vector3& normal);

    // Compute reflection vector form non-unit incident and normal vectors.
    static Vector3 ReflectNormalize(const Vector3& inc, const Vector3& normal);

    // Compute refraction vector from unit incident and normal vectors.
    // Eta is relative index of refraction.
    static Vector3 Refract(const Vector3& inc, const Vector3& normal, float eta);

protected:

    float m_x, m_y, m_z;

};

enum EContainment
{
    CONT_NONE,          // Not contained
    CONT_PARTIAL,       // Partially contained
    CONT_ALL            // Entirely contained
};

///////////////////////////////////////////////////////////////////////////////
//
// Sphere
//
///////////////////////////////////////////////////////////////////////////////

class Sphere
{
public:

    Sphere() :
        m_center(0.0f, 0.0f, 0.0f),
        m_radius(1.0f)
    {}

    Sphere(const Point3& center, const float radius = 1.0f) :
        m_center(center),
        m_radius(radius)
    {}

    bool makeAroundPnts(const Point3 *points, unsigned int num);

    inline void setCenter(const Point3& center)
    {
        m_center = center;
    }

    inline const Point3& getCenter() const
    {
        return m_center;
    }

    void setRadius(const float radius)
    {
        m_radius = radius;
    }

    inline float getRadius() const
    {
        return m_radius;
    }

    inline float getRadiusSqr() const
    {
        return (m_radius * m_radius);
    }

    inline float getInvRadius() const
    {
        return (1.0f / m_radius);
    }

    EContainment contains(const Point3& point) const;

protected:

    Point3 m_center;
    float m_radius;
};

///////////////////////////////////////////////////////////////////////////////
//
// Plane
//
///////////////////////////////////////////////////////////////////////////////

class  Plane
{
public:

    Plane() :
        m_normal(0.0f, 0.0f, 1.0f),
        m_offset(0.0f)
    {}

    Plane(const Vector3& normal, float offset = 0.0f) :
        m_normal(normal),
        m_offset(offset)
    {}

    Plane(const Point3& p0, const Point3& p1, const Point3& p2);

    Point3 nearestPointOnPlane(const Point3& point) const;
    float  distance(const Point3& point) const;

    EContainment contains(const Point3& point) const;

public:

  Vector3 m_normal;
  float m_offset;

};

///////////////////////////////////////////////////////////////////////////////
//
// Axis-aligned box
//
///////////////////////////////////////////////////////////////////////////////

class Box3
{
public:
    enum EBoxIndex
    {
        BI_MAX_X = 0,
        BI_MAX_Y,
        BI_MAX_Z,
        BI_MIN_X,
        BI_MIN_Y,
        BI_MIN_Z,
        BI_NUM_NDX
    };

    enum
    {
        NUM_CORNERS = 8,
        NUM_EDGES   = 12,
        NUM_FACES   = 6
    };

    Box3()
    {
        m_maxMin[ BI_MAX_X ] = 0.0f;
        m_maxMin[ BI_MAX_Y ] = 0.0f;
        m_maxMin[ BI_MAX_Z ] = 0.0f;
        m_maxMin[ BI_MIN_X ] = 0.0f;
        m_maxMin[ BI_MIN_Y ] = 0.0f;
        m_maxMin[ BI_MIN_Z ] = 0.0f;
    }

    Box3(const Point3& max, const Point3& min)
    {
        m_maxMin[ BI_MAX_X ] = max.x();
        m_maxMin[ BI_MAX_Y ] = max.y();
        m_maxMin[ BI_MAX_Z ] = max.z();
        m_maxMin[ BI_MIN_X ] = min.x();
        m_maxMin[ BI_MIN_Y ] = min.y();
        m_maxMin[ BI_MIN_Z ] = min.z();
    }

    Box3(float maxX, float maxY, float maxZ, float minX, float minY, float minZ)
    {
        m_maxMin[ BI_MAX_X ] = maxX;
        m_maxMin[ BI_MAX_Y ] = maxY;
        m_maxMin[ BI_MAX_Z ] = maxZ;
        m_maxMin[ BI_MIN_X ] = minX;
        m_maxMin[ BI_MIN_Y ] = minY;
        m_maxMin[ BI_MIN_Z ] = minZ;
    }

    void empty()
    {
        m_maxMin[ BI_MAX_X ] = -INFINITE;
        m_maxMin[ BI_MAX_Y ] = -INFINITE;
        m_maxMin[ BI_MAX_Z ] = -INFINITE;
        m_maxMin[ BI_MIN_X ] = INFINITE;
        m_maxMin[ BI_MIN_Y ] = INFINITE;
        m_maxMin[ BI_MIN_Z ] = INFINITE;
    }

    bool split(float plane, unsigned int axis, Box3& boxL, Box3& boxR) const;

    void getCorner(unsigned int which, Point3& corner) const;
    void getEdge(unsigned int which, int& c0, int& c1) const;
    void getFace(unsigned int which, int& c0, int& c1, int& c2, int& c3) const;

    void getCenter(Point3& center) const
    {
        center.x() = (m_maxMin[BI_MAX_X] + m_maxMin[BI_MIN_X]) * 0.5f;
        center.y() = (m_maxMin[BI_MAX_Y] + m_maxMin[BI_MIN_Y]) * 0.5f;
        center.z() = (m_maxMin[BI_MAX_Z] + m_maxMin[BI_MIN_Z]) * 0.5f;
    }

    void getDims(Vector3& dims) const;

    void getMax(Point3& max) const
    {
        max.set(m_maxMin[BI_MAX_X], m_maxMin[BI_MAX_Y], m_maxMin[BI_MAX_Z]);
    }
    void getMin(Point3& min) const
    {
        min.set(m_maxMin[BI_MIN_X], m_maxMin[BI_MIN_Y], m_maxMin[BI_MIN_Z]);
    }

    float getSurfaceArea() const;

    EContainment contains(const Point3& point) const;
    EContainment contains(const Box3& box) const;

    void extendAround(const Point3& point);

public:

    float m_maxMin[ BI_NUM_NDX ];

private:

    static int s_cornerIndex[ NUM_CORNERS ][ 3 ];
    static int s_edges[ NUM_EDGES ][ 2 ];
    static int s_faces[ NUM_FACES ][ 4 ];
};

///////////////////////////////////////////////////////////////////////////////
//
// Oriented box
//
///////////////////////////////////////////////////////////////////////////////

class OrientedBox
{
public:

    OrientedBox() :
        m_center(0.0f, 0.0f, 0.0f),
        m_pad(0),
        m_bu(1.0f, 0.0f, 0.0f),
        m_hu(0.0f),
        m_bv(0.0f, 1.0f, 0.0f),
        m_hv(0.0f),
        m_bw(0.0f, 0.0f, 1.0f),
        m_hw(0.0f)
    {}

    OrientedBox(const Box3& box);

    EContainment contains(const Point3& point) const;

public:

    Point3 m_center;
    int   m_pad;

    // Basis vectors (directions of box sides).
    // Note, could combine basis vector and center into a basis matrix.
    Vector3 m_bu;

    // Half length
    float m_hu;

    Vector3 m_bv;
    float m_hv;

    Vector3 m_bw;
    float m_hw;
};

///////////////////////////////////////////////////////////////////////////////
//
// Ray (parametric line)
//
///////////////////////////////////////////////////////////////////////////////

class Ray
{
public:

    Ray() :
        m_origin(0.0f, 0.0f, 0.0f),
        m_dir(0.0f, 0.0f, 1.0f)
    {}

    Ray(const Point3& origin, const Vector3& dir) :
        m_origin(origin),
        m_dir(dir)
    {}

    Ray(const Point3& origin, const Point3& end) :
        m_origin(origin),
        m_dir(origin, end)
    {
        m_dir.normalize();
    }

    Point3 point_at(const float t) const
    {
        Point3 point(m_origin);
        point.x() += m_dir.x() * t;
        point.y() += m_dir.y() * t;
        point.z() += m_dir.z() * t;
        return point;
    }

    inline const Point3& get_origin() const
    {
        return m_origin;
    }

    inline const Vector3& get_direction() const
    {
        return m_dir;
    }

public:

    Point3 m_origin;
    Vector3 m_dir;
};

///////////////////////////////////////////////////////////////////////////////
//
// Intersection methods
//
///////////////////////////////////////////////////////////////////////////////

struct RayHit
{
    float       m_t;
    Point3       m_point;
    Vector3       m_norm;
    Vector3       m_bary;
};

struct Triangle
{
    Triangle()
    {}

    Triangle(const Point3& v0, const Point3& v1, const Point3& v2) :
        m_v0(v0),
        m_v1(v1),
        m_v2(v2)
    {}

    float getArea() const;

    Point3   m_v0;
    Point3   m_v1;
    Point3   m_v2;
};

struct TriangleFast
{
    TriangleFast(const Triangle& tri);

    Point3   m_v0;
    int     pad0;

    float   m_nu, m_nv, m_nd;   // Plane coeff of projection plane
    int     m_k;                // Projection plane index (X, Y, Z)

    float   m_bnu, m_bnv;       // Projection plane edge equations
    float   m_cnu, m_cnv;
};

extern bool Intersect(const Ray& ray, const Sphere& sphere, RayHit& h0, RayHit& h1);
extern bool Intersect(const Ray& ray, const Box3& box, float& t0, float& t1);
extern bool Intersect(const Ray& ray, const Plane& plane, RayHit& h0);
extern bool Intersect(const Ray& ray, const Triangle& tri, RayHit& h0);
extern bool Intersect(const Ray& ray, const TriangleFast& tri, RayHit& h0);

extern bool Intersect(const Triangle& tri, const Box3& box);

#define MAX_INTERSECT_POINTS 4

struct IntersectInfo
{
    unsigned int m_numPoints;
    Point3 m_points[ MAX_INTERSECT_POINTS ];
};

extern bool Intersect(const Plane& plane, const Box3& box, IntersectInfo& hitInfo);
extern bool Intersect(const Plane& plane, const OrientedBox& box, IntersectInfo& hitInfo);

} // namespace Math


