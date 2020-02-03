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
#ifndef __EMSCRIPTEN__
#include <stdexcept>
#endif

namespace Math {

// coordinate names
enum 
{
    COORD_X = 0,
    COORD_Y = 1,
    COORD_Z = 2,
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

///////////////////////////////////////////////////////////////////////////////
//
// Point 2
//
///////////////////////////////////////////////////////////////////////////////

class  Point2
{
public:

    Point2() = default;
    Point2(const Point2& p) = default;
    Point2(float x_, float y_) :
        x(x_),
        y(y_) {}

    Point2& operator = (const Point2& v) = default;

    Point2& operator += (const Point2& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    Point2& operator -= (const Point2& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Point2& operator *= (float scale)
    {
        x *= scale;
        y *= scale;
        return *this;
    }

    Point2 operator + (const Point2& v) const
    {
        return Point2(x + v.x, y + v.y);
    }

    Point2 operator - (const Point2& v) const
    {
        return Point2(x - v.x, y - v.y);
    }

    Point2 operator * (float scale) const
    {
        return Point2(x * scale, y * scale);
    }

    friend Point2 operator * (float scale, const Point2& v)
    {
        return Point2(v.x * scale, v.y * scale);
    }

    bool operator == (const Point2& v) const
    {
        return ((x == v.x) && (y == v.y));
    }

    bool operator != (const Point2& v) const
    {
        return !(*this == v);
    }

    bool equal(const Point2& v, float e = EPSILON) const
    {
        return (Equal(x, v.x, e) &&
                Equal(y, v.y, e));
    }

    float distanceSquare(const Point2& v) const
    {
        return (Sqr(x - v.x) + Sqr(y - v.y));
    }

    float distance(const Point2& v) const
    {
        return sqrtf(distanceSquare(v));
    }

    void set(const Point2& v)
    {
        this->x = v.x;
        this->y = v.y;
    }

    void set(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    float& at(int index)
    {
        switch (index)
        {
            case 0: return x;
            case 1: return y;
        };
#ifndef __EMSCRIPTEN__
        throw std::out_of_range("Invalid index.");
#else
        return x;
#endif
    }
    const float& at(int index) const
    {
        switch (index)
        {
            case 0: return x;
            case 1: return y;
        };
#ifndef __EMSCRIPTEN__
        throw std::out_of_range("Invalid index.");
#else
        return x;
#endif
    }

public:

    float x = 0.0f, y = 0.0f;

};

///////////////////////////////////////////////////////////////////////////////
//
// Point 3
//
///////////////////////////////////////////////////////////////////////////////

class  Point3
{
public:

    Point3() = default;
    Point3(const Point3& p) = default;
    Point3(const Point2& p) :
        x(p.x),
        y(p.y),
        z(0.0f) {}

    Point3(float x_, float y_, float z_) :
        x(x_),
        y(y_),
        z(z_) {}

    Point3& operator = (const Point3& v) = default;

    Point3& operator += (const Point3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Point3& operator -= (const Point3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Point3& operator *= (float scale)
    {
        x *= scale;
        y *= scale;
        z *= scale;
        return *this;
    }

    Point3 operator + (const Point3& v) const
    {
        return Point3(x + v.x, y + v.y, z + v.z);
    }

    Point3 operator - (const Point3& v) const
    {
        return Point3(x - v.x, y - v.y, z - v.z);
    }

    Point3 operator * (float scale) const
    {
        return Point3(x * scale, y * scale, z * scale);
    }

    friend Point3 operator * (float scale, const Point3& v)
    {
        return Point3(v.x * scale, v.y * scale, v.z * scale);
    }

    bool operator == (const Point3& v) const
    {
        return ((x == v.x) && (y == v.y) && (z == v.z));
    }

    bool operator != (const Point3& v) const
    {
        return !(*this == v);
    }

    bool equal(const Point3& v, float e = EPSILON) const
    {
        return (Equal(x, v.x, e) &&
                Equal(y, v.y, e) &&
                Equal(z, v.z, e));
    }

    float distanceSquare(const Point3& v) const
    {
        return (Sqr(x - v.x) + Sqr(y - v.y) + Sqr(z - v.z));
    }

    float distance(const Point3& v) const
    {
        return sqrtf(distanceSquare(v));
    }

    void set(const Point3& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    void set(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float& at(int index)
    {
        switch (index)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
        };
#ifndef __EMSCRIPTEN__
        throw std::out_of_range("Invalid index.");
#else
        return x;
#endif
    }
    const float& at(int index) const
    {
        switch (index)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
        };
#ifndef __EMSCRIPTEN__
        throw std::out_of_range("Invalid index.");
#else
        return x;
#endif
    }

public:

    float x = 0.0f, y = 0.0f, z = 0.0f;

};

///////////////////////////////////////////////////////////////////////////////
//
// Vector 2
//
///////////////////////////////////////////////////////////////////////////////

class Vector2
{
public:

    Vector2() {}

    Vector2(const Vector2& v) :
        x(v.x),
        y(v.y)
    {}
    Vector2(const Point2& p) :
        x(p.x),
        y(p.y)
    {}

    Vector2(float x_, float y_) :
        x(x_),
        y(y_)
    {}

    Vector2(const Point2& origin, const Point2& end) :
        x(end.x - origin.x),
        y(end.y - origin.y)
    {}

    inline Vector2& operator = (const Vector2& v)
    {
        x = v.x;
        y = v.y;
        return *this;
    }

    inline Vector2& operator += (const Vector2& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    inline Vector2& operator -= (const Vector2& v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    inline Vector2& operator *= (float scale)
    {
        x *= scale;
        y *= scale;
        return *this;
    }

    inline Vector2& operator /= (float scale)
    {
        x /= scale;
        y /= scale;
        return *this;
    }

    inline Vector2 operator - () const
    {
        return Vector2(-x, -y);
    }

    inline Vector2 operator + () const
    {
        return Vector2(+x, +y);
    }

    inline Vector2 operator + (const Vector2& v) const
    {
        return Vector2(x + v.x, y + v.y);
    }

    inline Vector2 operator - (const Vector2& v) const
    {
        return Vector2(x - v.x, y - v.y);
    }

    inline Vector2 operator * (float scale) const
    {
        return Vector2(x * scale, y * scale);
    }

    inline Vector2 operator / (float scale) const
    {
        return Vector2(x / scale, y / scale);
    }

    friend Vector2 operator * (float scale, const Vector2& v)
    {
        return Vector2(v.x * scale, v.y * scale);
    }

    inline bool operator == (const Vector2& v) const
    {
        return ((x == v.x) && (y == v.y));
    }

    inline bool operator != (const Vector2& v) const
    {
        return !(*this == v);
    }

    inline bool equal(const Vector2& v, float e = EPSILON) const
    {
        return (Equal(x, v.x, e) &&
                Equal(y, v.y, e));
    }

    float normalize();

    inline float length() const
    {
        return sqrtf(x * x + y * y);
    }

    inline float lengthSquare() const
    {
        return (x * x + y * y);
    }

    inline float dot(const Vector2& v) const
    {
        return (x * v.x + y * v.y);
    }

    inline float distanceSquare(const Vector2& v) const
    {
        return (Sqr(x - v.x) + Sqr(y - v.y));
    }

    inline float distance(const Vector2& v) const
    {
        return sqrtf(distanceSquare(v));
    }

    inline void set(const Vector2& v)
    {
        x = v.x;
        y = v.y;
    }

    inline void set(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

public:

    float x, y;

};

///////////////////////////////////////////////////////////////////////////////
//
// Vector 3
//
///////////////////////////////////////////////////////////////////////////////

class  Vector3
{
public:

    Vector3() = default;
    Vector3(const Vector3& v) = default;

    Vector3(float x_, float y_, float z_) :
        x(x_),
        y(y_),
        z(z_) {}

    Vector3(const Point3& origin, const Point3& end) :
        x(end.x - origin.x),
        y(end.y - origin.y),
        z(end.z - origin.z)
    {}

    Vector3& operator = (const Vector3& v) = default;

    Vector3& operator += (const Vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3& operator -= (const Vector3& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3& operator *= (float scale)
    {
        x *= scale;
        y *= scale;
        z *= scale;
        return *this;
    }

    Vector3& operator /= (float scale)
    {
        const float invScale = 1.0f / scale;
        x *= invScale;
        y *= invScale;
        z *= invScale;
        return *this;
    }

    Vector3 operator - () const
    {
        return Vector3(-x, -y, -z);
    }

    Vector3 operator + () const
    {
        return Vector3(+x, +y, +z);
    }

    Vector3 operator + (const Vector3& v) const
    {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator - (const Vector3& v) const
    {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 operator * (float scale) const
    {
        return Vector3(x * scale, y * scale, z * scale);
    }

    Vector3 operator / (float scale) const
    {
        const float invScale = 1.0f / scale;
        return Vector3(x * invScale, y * invScale, z * invScale);
    }

    friend Vector3 operator * (float scale, const Vector3& v)
    {
        return Vector3(v.x * scale, v.y * scale, v.z * scale);
    }

    bool operator == (const Vector3& v) const
    {
        return ((x == v.x) && (y == v.y) && (z == v.z));
    }

    bool operator != (const Vector3& v) const
    {
        return !(*this == v);
    }

    bool equal(const Vector3& v, float e = EPSILON) const
    {
        return (Equal(x, v.x, e) &&
                 Equal(y, v.y, e) &&
                 Equal(z, v.z, e));
    }

    float normalize();

    float length() const
    {
        return sqrtf(x * x + y * y + z * z);
    }

    float lengthSquare() const
    {
        return (x * x + y * y + z * z);
    }

    float dot(const Vector3& v) const
    {
        return (x * v.x + y * v.y + z * v.z);
    }
    float dot(const Point3& p) const
    {
        return (x * p.x + y * p.y + z * p.z);
    }

    float distanceSquare(const Vector3& v) const
    {
        return (Sqr(x - v.x) + Sqr(y - v.y) + Sqr(z - v.z));
    }

    float distance(const Vector3& v) const
    {
        return sqrtf(distanceSquare(v));
    }

    void set(const Vector3& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    void set(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float& at(int index)
    {
        switch (index)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
        };
#ifndef __EMSCRIPTEN__
        throw std::out_of_range("Invalid index.");
#else
        return x;
#endif
    }
    const float& at(int index) const
    {
        switch (index)
        {
            case 0: return x;
            case 1: return y;
            case 2: return z;
        };
#ifndef __EMSCRIPTEN__
        throw std::out_of_range("Invalid index.");
#else
        return x;
#endif
    }

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

    static Vector3 Subtract(const Point3& p0, const Point3& p1);

public:

    float x = 0.0f, y = 0.0f, z = 0.0f;

};

enum EContainment
{
    CONT_NONE,          // Not contained
    CONT_PARTIAL,       // Partially contained
    CONT_ALL            // Entirely contained
};

///////////////////////////////////////////////////////////////////////////////
//
// Circle
//
///////////////////////////////////////////////////////////////////////////////

class Circle
{

public:
    Circle() :
        m_center(),
        m_radius(0.0f)
    {}
    Circle(const Point2& center, float r) :
        m_center(center),
        m_radius(r)
    {}
    const Point2& center() const
    {
        return m_center;
    }
    inline Point2& center()
    {
        return m_center;
    }
    inline const float& radius() const
    {
        return m_radius;
    }
    inline float& radius()
    {
        return m_radius;
    }

    // Get point at angle t (radians) on circle
    Point2 pointAt(float t) const
    {
        // Parametric form of an circle:
        // t   : angle from 0..2pi
        // X(t) = centerX + radius * cos(t)
        // Y(t) = centerY + radius * sin(t)

        Point2 point;
        point.x = m_center.x + m_radius * cosf(t);
        point.y = m_center.y + m_radius * sinf(t);

        return point;
    }

    float area() const
    {
        const auto a = static_cast<float>(M_PI) * m_radius * m_radius;
        return a;
    }

    float circumference() const
    {
        const float c = static_cast<float>(2) * static_cast<float>(M_PI) * m_radius;
        return c;
    }

private:
    Point2 m_center;
    float  m_radius;
};

///////////////////////////////////////////////////////////////////////////////
//
// Sphere
//
///////////////////////////////////////////////////////////////////////////////

class Sphere
{
public:

    Sphere() = default;

    Sphere(const Point3& center, float radius) :
        m_center(center),
        m_radius(radius)
    {}

    bool makeAroundPnts(const Point3 *points, unsigned int num);

    void setCenter(const Point3& center)
    {
        m_center = center;
    }

    const Point3& getCenter() const
    {
        return m_center;
    }

    void setRadius(const float radius)
    {
        m_radius = radius;
    }

    float getRadius() const
    {
        return m_radius;
    }

    float getRadiusSqr() const
    {
        return (m_radius * m_radius);
    }

    float getInvRadius() const
    {
        return (1.0f / m_radius);
    }

    EContainment contains(const Point3& point) const;

protected:

    Point3 m_center = {0.0f, 0.0f, 0.0f};
    float m_radius = 1.0f;
};

///////////////////////////////////////////////////////////////////////////////
//
// Plane
//
///////////////////////////////////////////////////////////////////////////////

class Plane
{
public:

    Plane() = default;

    Plane(const Vector3& normal, float offset) :
        m_normal(normal),
        m_offset(offset)
    {}

    Plane(const Point3& p0, const Point3& p1, const Point3& p2);

    Point3 nearestPointOnPlane(const Point3& point) const;
    float  distance(const Point3& point) const;

    EContainment contains(const Point3& point) const;

public:

  Vector3 m_normal = {0.0f, 0.0f, 1.0f};
  float m_offset = 0.0f;

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

    Box3() = default;

    Box3(const Point3& max, const Point3& min)
    {
        m_maxMin[ BI_MAX_X ] = max.x;
        m_maxMin[ BI_MAX_Y ] = max.y;
        m_maxMin[ BI_MAX_Z ] = max.z;
        m_maxMin[ BI_MIN_X ] = min.x;
        m_maxMin[ BI_MIN_Y ] = min.y;
        m_maxMin[ BI_MIN_Z ] = min.z;
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
        center.x = (m_maxMin[BI_MAX_X] + m_maxMin[BI_MIN_X]) * 0.5f;
        center.y = (m_maxMin[BI_MAX_Y] + m_maxMin[BI_MIN_Y]) * 0.5f;
        center.z = (m_maxMin[BI_MAX_Z] + m_maxMin[BI_MIN_Z]) * 0.5f;
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

    float m_maxMin[ BI_NUM_NDX ] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

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

    OrientedBox() = default;

    explicit OrientedBox(const Box3& box);

    EContainment contains(const Point3& point) const;

public:

    Point3 m_center = {0.0f, 0.0f, 0.0f};
    int   m_pad = 0;

    // Basis vectors (directions of box sides).
    // Note, could combine basis vector and center into a basis matrix.
    Vector3 m_bu = {1.0f, 0.0f, 0.0f};

    // Half length
    float m_hu = 0.0f;

    Vector3 m_bv = {0.0f, 1.0f, 0.0f};
    float m_hv = 0.0f;

    Vector3 m_bw = {0.0f, 0.0f, 1.0f};
    float m_hw = 0.0f;
};

///////////////////////////////////////////////////////////////////////////////
//
// Ray2 (parametric line)
//
///////////////////////////////////////////////////////////////////////////////

class Ray2
{
public:

    Ray2() = default;

    Ray2(const Point2& origin, const Vector2& dir) :
        m_origin(origin),
        m_dir(dir)
    {}

    Ray2(const Point2& origin, const Point2& end) :
        m_origin(origin),
        m_dir(origin, end)
    {
        m_dir.normalize();
    }

    Point2 point_at(float t) const
    {
        Point2 point(m_origin);
        point.x += m_dir.x * t;
        point.y += m_dir.y * t;
        return point;
    }

    const Point2& get_origin() const
    {
        return m_origin;
    }

    const Vector2& get_direction() const
    {
        return m_dir;
    }

public:

    Point2 m_origin = {0.0f, 0.0f};
    Vector2 m_dir = {0.0f, 0.0f};
};

///////////////////////////////////////////////////////////////////////////////
//
// Ray (parametric line)
//
///////////////////////////////////////////////////////////////////////////////

class Ray
{
public:

    Ray() = default;

    Ray(const Point3& origin, const Vector3& dir) :
        m_origin(origin),
        m_dir(dir)
    {
        m_dir.normalize();
    }

    Ray(const Point3& origin, const Point3& end) :
        m_origin(origin),
        m_dir(origin, end)
    {
        m_dir.normalize();
    }

    Point3 point_at(const float t) const
    {
        Point3 point(m_origin);
        point.x += m_dir.x * t;
        point.y += m_dir.y * t;
        point.z += m_dir.z * t;
        return point;
    }

    const Point3& get_origin() const
    {
        return m_origin;
    }

    const Vector3& get_direction() const
    {
        return m_dir;
    }

public:

    Point3 m_origin = {0.0f, 0.0f, 0.0f};
    Vector3 m_dir = {0.0f, 0.0f, 1.0f};
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
    Triangle() = default;

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
    explicit TriangleFast(const Triangle& tri);

    Point3   m_v0;
    int     pad0;

    float   m_nu, m_nv, m_nd;   // Plane coeff of projection plane
    int     m_k;                // Projection plane index (X, Y, Z)

    float   m_bnu, m_bnv;       // Projection plane edge equations
    float   m_cnu, m_cnv;
};

extern bool Intersect(const Ray2& ray, const Circle& sphere, RayHit& h0, RayHit& h1);
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


