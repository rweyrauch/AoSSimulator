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
#include <cfloat>

#ifndef __EMSCRIPTEN__

#include <stdexcept>

#endif

namespace Math {

// coordinate names
    enum {
        Coord_X = 0,
        Coord_Y = 1,
        Coord_Z = 2,
    };

// How close to zero is to considered zero?
    const double g_epsilon = 1.0e-6;
    const double g_infinite = DBL_MAX;
    
    template<class T>
    inline T Clamp(T val, T min, T max) {
        return ((val > max) ? max : ((val < min) ? min : val));
    }

    template<class T>
    inline bool RangeClosed(T val, T min, T max) {
        return ((val <= max) && (val >= min));
    }

    template<class T>
    inline bool RangeOpen(T val, T min, T max) {
        return ((val < max) && (val > min));
    }

    template<class T>
    inline T Min(T val1, T val2) {
        return ((val1 < val2) ? val1 : val2);
    }

    template<class T>
    inline T Max(T val1, T val2) {
        return ((val1 > val2) ? val1 : val2);
    }

    template<class T>
    inline T Abs(T val) {
        return (val < static_cast<T>(0) ? -val : val);
    }

    template<class T>
    inline bool Equal(T val1, T val2, T tol) {
        return (val1 >= (val2 - tol)) && (val1 <= (val2 + tol));
    }

    template<class T>
    inline T Sqr(T val) {
        return (val * val);
    }

    template<class T>
    inline T DegToRad(T deg) {
        return (deg * static_cast<T>(M_PI) / static_cast<T>(180));
    }

    template<class T>
    inline T RadToDeg(T rad) {
        return (rad * static_cast<T>(180) / static_cast<T>(M_PI));
    }

    template<class T>
    T Lerp(T t, T a, T b) {
        return (a + t * (b - a));
    }

    template<class T>
    T SCurve(T t) {
        return (t * t * (static_cast<T>(3.0) - static_cast<T>(2.0) * t));
    }

    template<class T>
    void Swap(T &a, T &b) {
        T tmp = a;
        a = b;
        b = tmp;
    }

    inline double Trunc(double val) {
        if (val >= 0.0)
            return floor(val);
        return ceil(val);
    }

///////////////////////////////////////////////////////////////////////////////
//
// Point 2
//
///////////////////////////////////////////////////////////////////////////////

    class Point2 {
    public:

        Point2() = default;

        Point2(const Point2 &p) = default;

        Point2(double x_, double y_) :
                x(x_),
                y(y_) {}

        Point2 &operator=(const Point2 &v) = default;

        Point2 &operator+=(const Point2 &v) {
            x += v.x;
            y += v.y;
            return *this;
        }

        Point2 &operator-=(const Point2 &v) {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        Point2 &operator*=(double scale) {
            x *= scale;
            y *= scale;
            return *this;
        }

        Point2 operator+(const Point2 &v) const {
            return Point2(x + v.x, y + v.y);
        }

        Point2 operator-(const Point2 &v) const {
            return Point2(x - v.x, y - v.y);
        }

        Point2 operator*(double scale) const {
            return Point2(x * scale, y * scale);
        }

        friend Point2 operator*(double scale, const Point2 &v) {
            return Point2(v.x * scale, v.y * scale);
        }

        bool operator==(const Point2 &v) const {
            return ((x == v.x) && (y == v.y));
        }

        bool operator!=(const Point2 &v) const {
            return !(*this == v);
        }

        bool equal(const Point2 &v, double e = g_epsilon) const {
            return (Equal(x, v.x, e) &&
                    Equal(y, v.y, e));
        }

        double distanceSquare(const Point2 &v) const {
            return (Sqr(x - v.x) + Sqr(y - v.y));
        }

        double distance(const Point2 &v) const {
            return sqrtf(distanceSquare(v));
        }

        void set(const Point2 &v) {
            this->x = v.x;
            this->y = v.y;
        }

        void set(double xx, double yy) {
            this->x = xx;
            this->y = yy;
        }

        double &at(int index) {
            switch (index) {
                case 0:
                    return x;
                case 1:
                    return y;
                default:
                    break;
            }
#ifndef __EMSCRIPTEN__
            throw std::out_of_range("Invalid index.");
#else
            return x;
#endif
        }

        const double &at(int index) const {
            switch (index) {
                case 0:
                    return x;
                case 1:
                    return y;
                default:
                    break;
            }
#ifndef __EMSCRIPTEN__
            throw std::out_of_range("Invalid index.");
#else
            return x;
#endif
        }

    public:

        double x = 0.0f, y = 0.0f;

    };

///////////////////////////////////////////////////////////////////////////////
//
// Point 3
//
///////////////////////////////////////////////////////////////////////////////

    class Point3 {
    public:

        Point3() = default;

        Point3(const Point3 &p) = default;

        Point3(double x_, double y_) :
            x(x_),
            y(y_),
            z(0.0f) {}

        Point3(double x_, double y_, double z_) :
                x(x_),
                y(y_),
                z(z_) {}

        Point3 &operator=(const Point3 &v) = default;

        Point3 &operator+=(const Point3 &v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        Point3 &operator-=(const Point3 &v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        Point3 &operator*=(double scale) {
            x *= scale;
            y *= scale;
            z *= scale;
            return *this;
        }

        Point3 operator+(const Point3 &v) const {
            return Point3(x + v.x, y + v.y, z + v.z);
        }

        Point3 operator-(const Point3 &v) const {
            return Point3(x - v.x, y - v.y, z - v.z);
        }

        Point3 operator*(double scale) const {
            return Point3(x * scale, y * scale, z * scale);
        }

        friend Point3 operator*(double scale, const Point3 &v) {
            return Point3(v.x * scale, v.y * scale, v.z * scale);
        }

        bool operator==(const Point3 &v) const {
            return ((x == v.x) && (y == v.y) && (z == v.z));
        }

        bool operator!=(const Point3 &v) const {
            return !(*this == v);
        }

        bool equal(const Point3 &v, double e = g_epsilon) const {
            return (Equal(x, v.x, e) &&
                    Equal(y, v.y, e) &&
                    Equal(z, v.z, e));
        }

        double distanceSquare(const Point3 &v) const {
            return (Sqr(x - v.x) + Sqr(y - v.y) + Sqr(z - v.z));
        }

        double distance(const Point3 &v) const {
            const auto distSqr = distanceSquare(v);
            return (distSqr > 0.0f) ? sqrtf(distanceSquare(v)) : 0.0f;
        }

        void set(const Point3 &v) {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
        }

        void set(double xx, double yy, double zz) {
            this->x = xx;
            this->y = yy;
            this->z = zz;
        }

        double &at(int index) {
            switch (index) {
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
                default:
                    break;
            }
#ifndef __EMSCRIPTEN__
            throw std::out_of_range("Invalid index.");
#else
            return x;
#endif
        }

        const double &at(int index) const {
            switch (index) {
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
                default:
                    break;
            }
#ifndef __EMSCRIPTEN__
            throw std::out_of_range("Invalid index.");
#else
            return x;
#endif
        }

    public:

        double x = 0.0f, y = 0.0f, z = 0.0f;

    };

///////////////////////////////////////////////////////////////////////////////
//
// Vector 2
//
///////////////////////////////////////////////////////////////////////////////

    class Vector2 {
    public:

        Vector2() = default;

        Vector2(const Vector2 &v) = default;

        explicit Vector2(const Point2 &p) :
                x(p.x),
                y(p.y) {}

        Vector2(double x_, double y_) :
                x(x_),
                y(y_) {}

        Vector2(const Point2 &origin, const Point2 &end) :
                x(end.x - origin.x),
                y(end.y - origin.y) {}

        inline Vector2 &operator=(const Vector2 &v) = default;

        inline Vector2 &operator+=(const Vector2 &v) {
            x += v.x;
            y += v.y;
            return *this;
        }

        inline Vector2 &operator-=(const Vector2 &v) {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        inline Vector2 &operator*=(double scale) {
            x *= scale;
            y *= scale;
            return *this;
        }

        inline Vector2 &operator/=(double scale) {
            x /= scale;
            y /= scale;
            return *this;
        }

        inline Vector2 operator-() const {
            return Vector2(-x, -y);
        }

        inline Vector2 operator+() const {
            return Vector2(+x, +y);
        }

        inline Vector2 operator+(const Vector2 &v) const {
            return Vector2(x + v.x, y + v.y);
        }

        inline Vector2 operator-(const Vector2 &v) const {
            return Vector2(x - v.x, y - v.y);
        }

        inline Vector2 operator*(double scale) const {
            return Vector2(x * scale, y * scale);
        }

        inline Vector2 operator/(double scale) const {
            return Vector2(x / scale, y / scale);
        }

        friend Vector2 operator*(double scale, const Vector2 &v) {
            return Vector2(v.x * scale, v.y * scale);
        }

        inline bool operator==(const Vector2 &v) const {
            return ((x == v.x) && (y == v.y));
        }

        inline bool operator!=(const Vector2 &v) const {
            return !(*this == v);
        }

        inline bool equal(const Vector2 &v, double e = g_epsilon) const {
            return (Equal(x, v.x, e) &&
                    Equal(y, v.y, e));
        }

        Vector2 normalize() const;

        inline double length() const {
            return sqrtf(x * x + y * y);
        }

        inline double lengthSquare() const {
            return (x * x + y * y);
        }

        inline double dot(const Vector2 &v) const {
            return (x * v.x + y * v.y);
        }

        inline double distanceSquare(const Vector2 &v) const {
            return (Sqr(x - v.x) + Sqr(y - v.y));
        }

        inline double distance(const Vector2 &v) const {
            return sqrtf(distanceSquare(v));
        }

        inline void set(const Vector2 &v) {
            x = v.x;
            y = v.y;
        }

        inline void set(double xx, double yy) {
            this->x = xx;
            this->y = yy;
        }

    public:

        double x, y;

    };

///////////////////////////////////////////////////////////////////////////////
//
// Vector 3
//
///////////////////////////////////////////////////////////////////////////////

    class Vector3 {
    public:

        Vector3() = default;

        Vector3(const Vector3 &v) = default;

        Vector3(double x_, double y_, double z_) :
                x(x_),
                y(y_),
                z(z_) {}

        Vector3(const Point3 &origin, const Point3 &end) :
                x(end.x - origin.x),
                y(end.y - origin.y),
                z(end.z - origin.z) {}

        Vector3 &operator=(const Vector3 &v) = default;

        Vector3 &operator+=(const Vector3 &v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        Vector3 &operator-=(const Vector3 &v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        Vector3 &operator*=(double scale) {
            x *= scale;
            y *= scale;
            z *= scale;
            return *this;
        }

        Vector3 &operator/=(double scale) {
            const double invScale = 1.0f / scale;
            x *= invScale;
            y *= invScale;
            z *= invScale;
            return *this;
        }

        Vector3 operator-() const {
            return Vector3(-x, -y, -z);
        }

        Vector3 operator+() const {
            return Vector3(+x, +y, +z);
        }

        Vector3 operator+(const Vector3 &v) const {
            return Vector3(x + v.x, y + v.y, z + v.z);
        }

        Vector3 operator-(const Vector3 &v) const {
            return Vector3(x - v.x, y - v.y, z - v.z);
        }

        Vector3 operator*(double scale) const {
            return Vector3(x * scale, y * scale, z * scale);
        }

        Vector3 operator/(double scale) const {
            const double invScale = 1.0f / scale;
            return Vector3(x * invScale, y * invScale, z * invScale);
        }

        friend Vector3 operator*(double scale, const Vector3 &v) {
            return Vector3(v.x * scale, v.y * scale, v.z * scale);
        }

        bool operator==(const Vector3 &v) const {
            return ((x == v.x) && (y == v.y) && (z == v.z));
        }

        bool operator!=(const Vector3 &v) const {
            return !(*this == v);
        }

        bool equal(const Vector3 &v, double e = g_epsilon) const {
            return (Equal(x, v.x, e) &&
                    Equal(y, v.y, e) &&
                    Equal(z, v.z, e));
        }

        Vector3 normalize() const;

        double length() const {
            return sqrtf(x * x + y * y + z * z);
        }

        double lengthSquare() const {
            return (x * x + y * y + z * z);
        }

        double dot(const Vector3 &v) const {
            return (x * v.x + y * v.y + z * v.z);
        }

        double dot(const Point3 &p) const {
            return (x * p.x + y * p.y + z * p.z);
        }

        double distanceSquare(const Vector3 &v) const {
            return (Sqr(x - v.x) + Sqr(y - v.y) + Sqr(z - v.z));
        }

        double distance(const Vector3 &v) const {
            return sqrtf(distanceSquare(v));
        }

        void set(const Vector3 &v) {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
        }

        void set(double xx, double yy, double zz) {
            this->x = xx;
            this->y = yy;
            this->z = zz;
        }

        double &at(int index) {
            switch (index) {
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
                default:
                    break;
            }
#ifndef __EMSCRIPTEN__
            throw std::out_of_range("Invalid index.");
#else
            return x;
#endif
        }

        const double &at(int index) const {
            switch (index) {
                case 0:
                    return x;
                case 1:
                    return y;
                case 2:
                    return z;
                default:
                    break;
            }
#ifndef __EMSCRIPTEN__
            throw std::out_of_range("Invalid index.");
#else
            return x;
#endif
        }

        static double Dot(const Vector3 &v1, const Vector3 &v2);

        static double Dot(const Vector3 &v1, const Point3 &p2);

        static Vector3 Cross(const Vector3 &v1, const Vector3 &v2);

        // Compute reflection vector from unit incident and normal vectors.
        static Vector3 Reflect(const Vector3 &inc, const Vector3 &normal);

        // Compute reflection vector form non-unit incident and normal vectors.
        static Vector3 ReflectNormalize(const Vector3 &inc, const Vector3 &normal);

        // Compute refraction vector from unit incident and normal vectors.
        // Eta is relative index of refraction.
        static Vector3 Refract(const Vector3 &inc, const Vector3 &normal, double eta);

        static Vector3 Subtract(const Point3 &p0, const Point3 &p1);

    public:

        double x = 0.0f, y = 0.0f, z = 0.0f;

    };

    enum class Containment {
        None,          // Not contained
        Partial,       // Partially contained
        All            // Entirely contained
    };

///////////////////////////////////////////////////////////////////////////////
//
// Circle
//
///////////////////////////////////////////////////////////////////////////////

    class Circle {

    public:
        Circle() :
                m_center(),
                m_radius(0.0f) {}

        Circle(const Point2 &center, double r) :
                m_center(center),
                m_radius(r) {}

        const Point2 &center() const {
            return m_center;
        }

        inline Point2 &center() {
            return m_center;
        }

        inline const double &radius() const {
            return m_radius;
        }

        inline double &radius() {
            return m_radius;
        }

        // Get point at angle t (radians) on circle
        Point2 pointAt(double t) const {
            // Parametric form of an circle:
            // t   : angle from 0..2pi
            // X(t) = centerX + radius * cos(t)
            // Y(t) = centerY + radius * sin(t)

            Point2 point;
            point.x = m_center.x + m_radius * cosf(t);
            point.y = m_center.y + m_radius * sinf(t);

            return point;
        }

        double area() const {
            const auto a = static_cast<double>(M_PI) * m_radius * m_radius;
            return a;
        }

        double circumference() const {
            const double c = static_cast<double>(2) * static_cast<double>(M_PI) * m_radius;
            return c;
        }

    private:
        Point2 m_center;
        double m_radius;
    };

///////////////////////////////////////////////////////////////////////////////
//
// Sphere
//
///////////////////////////////////////////////////////////////////////////////

    class Sphere {
    public:

        Sphere() = default;

        Sphere(const Point3 &center, double radius) :
                m_center(center),
                m_radius(radius) {}

        bool makeAroundPnts(const Point3 *points, unsigned int num);

        void setCenter(const Point3 &center) {
            m_center = center;
        }

        const Point3 &getCenter() const {
            return m_center;
        }

        void setRadius(const double radius) {
            m_radius = radius;
        }

        double getRadius() const {
            return m_radius;
        }

        double getRadiusSqr() const {
            return (m_radius * m_radius);
        }

        double getInvRadius() const {
            return (1.0f / m_radius);
        }

        Containment contains(const Point3 &point) const;

    protected:

        Point3 m_center = {0.0f, 0.0f, 0.0f};
        double m_radius = 1.0f;
    };

///////////////////////////////////////////////////////////////////////////////
//
// Plane
//
///////////////////////////////////////////////////////////////////////////////

    class Plane {
    public:

        Plane() = default;

        Plane(const Vector3 &normal, double offset) :
                m_normal(normal),
                m_offset(offset) {}

        Plane(const Point3 &p0, const Point3 &p1, const Point3 &p2);

        Point3 nearestPointOnPlane(const Point3 &point) const;

        double distance(const Point3 &point) const;

        Containment contains(const Point3 &point) const;

    public:

        Vector3 m_normal = {0.0f, 0.0f, 1.0f};
        double m_offset = 0.0f;

    };

///////////////////////////////////////////////////////////////////////////////
//
// Axis-aligned box
//
///////////////////////////////////////////////////////////////////////////////

    class Box3 {
    public:
        enum EBoxIndex {
            Bi_Max_X = 0,
            Bi_Max_Y,
            Bi_Max_Z,
            Bi_Min_X,
            Bi_Min_Y,
            Bi_Min_Z,
            Bi_Num_Ndx
        };

        enum {
            Num_Corners = 8,
            Num_Edges = 12,
            Num_Faces = 6
        };

        Box3() = default;

        Box3(const Point3 &max, const Point3 &min) {
            m_maxMin[Bi_Max_X] = max.x;
            m_maxMin[Bi_Max_Y] = max.y;
            m_maxMin[Bi_Max_Z] = max.z;
            m_maxMin[Bi_Min_X] = min.x;
            m_maxMin[Bi_Min_Y] = min.y;
            m_maxMin[Bi_Min_Z] = min.z;
        }

        Box3(double maxX, double maxY, double maxZ, double minX, double minY, double minZ) {
            m_maxMin[Bi_Max_X] = maxX;
            m_maxMin[Bi_Max_Y] = maxY;
            m_maxMin[Bi_Max_Z] = maxZ;
            m_maxMin[Bi_Min_X] = minX;
            m_maxMin[Bi_Min_Y] = minY;
            m_maxMin[Bi_Min_Z] = minZ;
        }

        void empty() {
            m_maxMin[Bi_Max_X] = -g_infinite;
            m_maxMin[Bi_Max_Y] = -g_infinite;
            m_maxMin[Bi_Max_Z] = -g_infinite;
            m_maxMin[Bi_Min_X] = g_infinite;
            m_maxMin[Bi_Min_Y] = g_infinite;
            m_maxMin[Bi_Min_Z] = g_infinite;
        }

        bool split(double plane, unsigned int axis, Box3 &boxL, Box3 &boxR) const;

        void getCorner(unsigned int which, Point3 &corner) const;

        void getEdge(unsigned int which, int &c0, int &c1) const;

        void getFace(unsigned int which, int &c0, int &c1, int &c2, int &c3) const;

        void getCenter(Point3 &center) const {
            center.x = (m_maxMin[Bi_Max_X] + m_maxMin[Bi_Min_X]) * 0.5f;
            center.y = (m_maxMin[Bi_Max_Y] + m_maxMin[Bi_Min_Y]) * 0.5f;
            center.z = (m_maxMin[Bi_Max_Z] + m_maxMin[Bi_Min_Z]) * 0.5f;
        }

        void getDims(Vector3 &dims) const;

        void getMax(Point3 &max) const {
            max.set(m_maxMin[Bi_Max_X], m_maxMin[Bi_Max_Y], m_maxMin[Bi_Max_Z]);
        }

        void getMin(Point3 &min) const {
            min.set(m_maxMin[Bi_Min_X], m_maxMin[Bi_Min_Y], m_maxMin[Bi_Min_Z]);
        }

        double getSurfaceArea() const;

        Containment contains(const Point3 &point) const;

        Containment contains(const Box3 &box) const;

        void extendAround(const Point3 &point);

    public:

        double m_maxMin[Bi_Num_Ndx] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

    private:

        static int s_cornerIndex[Num_Corners][3];
        static int s_edges[Num_Edges][2];
        static int s_faces[Num_Faces][4];
    };

///////////////////////////////////////////////////////////////////////////////
//
// Oriented box
//
///////////////////////////////////////////////////////////////////////////////

    class OrientedBox {
    public:

        OrientedBox() = default;

        explicit OrientedBox(const Box3 &box);

        Containment contains(const Point3 &point) const;

    public:

        Point3 m_center = {0.0f, 0.0f, 0.0f};
        int m_pad = 0;

        // Basis vectors (directions of box sides).
        // Note, could combine basis vector and center into a basis matrix.
        Vector3 m_bu = {1.0f, 0.0f, 0.0f};

        // Half length
        double m_hu = 0.0f;

        Vector3 m_bv = {0.0f, 1.0f, 0.0f};
        double m_hv = 0.0f;

        Vector3 m_bw = {0.0f, 0.0f, 1.0f};
        double m_hw = 0.0f;
    };

///////////////////////////////////////////////////////////////////////////////
//
// Ray2 (parametric line)
//
///////////////////////////////////////////////////////////////////////////////

    class Ray2 {
    public:

        Ray2() = default;

        Ray2(const Point2 &origin, const Vector2 &dir) :
                m_origin(origin),
                m_dir(dir) {}

        Ray2(const Point2 &origin, const Point2 &end) :
                m_origin(origin),
                m_dir(origin, end) {
            m_dir = m_dir.normalize();
        }

        Point2 pointAt(double t) const {
            Point2 point(m_origin);
            point.x += m_dir.x * t;
            point.y += m_dir.y * t;
            return point;
        }

        const Point2 &getOrigin() const {
            return m_origin;
        }

        const Vector2 &getDirection() const {
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

    class Ray {
    public:

        Ray() = default;

        Ray(const Point3 &origin, const Vector3 &dir) :
                m_origin(origin),
                m_dir(dir) {
            m_dir = m_dir.normalize();
        }

        Ray(const Point3 &origin, const Point3 &end) :
                m_origin(origin),
                m_dir(origin, end) {
            m_dir = m_dir.normalize();
        }

        Point3 pointAt(const double t) const {
            Point3 point(m_origin);
            point.x += m_dir.x * t;
            point.y += m_dir.y * t;
            point.z += m_dir.z * t;
            return point;
        }

        const Point3 &getOrigin() const {
            return m_origin;
        }

        const Vector3 &getDirection() const {
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

    struct RayHit {
        double m_t;
        Point3 m_point;
        Vector3 m_norm;
        Vector3 m_bary;
    };

    struct Triangle {
        Triangle() = default;

        Triangle(const Point3 &v0, const Point3 &v1, const Point3 &v2) :
                m_v0(v0),
                m_v1(v1),
                m_v2(v2) {}

        double getArea() const;

        Point3 m_v0;
        Point3 m_v1;
        Point3 m_v2;
    };

    struct TriangleFast {
        explicit TriangleFast(const Triangle &tri);

        Point3 m_v0;

        double m_nu, m_nv, m_nd;   // Plane coeff of projection plane
        double m_bnu, m_bnv;       // Projection plane edge equations
        double m_cnu, m_cnv;
        int m_k;                // Projection plane index (X, Y, Z)
    };

    bool Intersect(const Ray2 &ray, const Circle &circle, RayHit &h0, RayHit &h1);

    bool Intersect(const Ray &ray, const Sphere &sphere, RayHit &h0, RayHit &h1);

    bool Intersect(const Ray &ray, const Box3 &box, double &t0, double &t1);

    bool Intersect(const Ray &ray, const Plane &plane, RayHit &h0);

    bool Intersect(const Ray &ray, const Triangle &tri, RayHit &h0);

    bool Intersect(const Ray &ray, const TriangleFast &tri, RayHit &h0);

    bool Intersect(const Triangle &tri, const Box3 &box);

    /*!
     * Determines the point of intersection between a moving circle (c0) and a stationary circle (c1).
     * The point of intersection is returned as the distance along the motion vector.
     * @param c0 Moving circle
     * @param motion0 Motion vector of c0
     * @param c1 Circle being intersected against
     * @param[out] poi Point of intersection along motion vector
     * @return True if the circles intersect, false otherwise.
     */
    bool PointOfIntersection(const Circle &c0, const Vector2 &motion0, const Circle &c1, double &poi);

#define MAX_INTERSECT_POINTS 4

    struct IntersectInfo {
        unsigned int m_numPoints;
        Point3 m_points[MAX_INTERSECT_POINTS];
    };

    bool Intersect(const Plane &plane, const Box3 &box, IntersectInfo &hitInfo);

    bool Intersect(const Plane &plane, const OrientedBox &box, IntersectInfo &hitInfo);

    bool Intersect(const Circle &c0, const Circle &c1, IntersectInfo &hitInfo);


} // namespace Math
