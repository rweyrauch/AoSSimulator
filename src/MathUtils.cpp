/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "MathUtils.h"

namespace Math
{

///////////////////////////////////////////////////////////////////////////////
//
// Vector 3
//
///////////////////////////////////////////////////////////////////////////////

float Vector3::Dot(const Vector3& v1, const Vector3& v2)
{
    return v1.dot(v2);
}

float Vector3::Dot(const Vector3& v1, const Point3& p2)
{
    const Vector3 v2(p2.x(), p2.y(), p2.z());
    return v1.dot(v2);
}

float Vector3::normalize()
{
    const float dsqr = lengthSquare();

    if (Abs(dsqr - 1.0f) < EPSILON)
    {
        return 1.0f;
    }
    else if (dsqr < EPSILON * EPSILON)
    {
        set(0.0f, 0.0f, 0.0f);
        return  0.0f;
    }

    const float d = 1.0f / sqrtf(dsqr);
    m_x *= d;
    m_y *= d;
    m_z *= d;

    return d;
}

Vector3 Vector3::Reflect(const Vector3& inc, const Vector3& normal)
{
    Vector3 reflVec = inc - normal * normal.dot(inc) * 2.0f;
    return reflVec;
}

Vector3 Vector3::ReflectNormalize(const Vector3& inc, const Vector3& normal)
{
    const float normLenSqr = normal.lengthSquare();
    Vector3 reflVec = inc - normal * normal.dot(inc) * 2.0f / normLenSqr;
    return reflVec;
}

Vector3 Vector3::Refract(const Vector3& inc, const Vector3& normal, float eta)
{
    // From Realistic Ray Tracing, Peter Shirley.
    //
    // Refr = eta * (inc - normal * (inc . normal)) - normal * sqrt(1 - eta^2 * (1 - (inc . normal)^2)
    //
    // If the root term is negative the ray is reflected and the method return (0,0,0).
    Vector3 refrVec(0.0f, 0.0f, 0.0f);
    const float idotn = inc.dot(normal);
    const float rootTerm = 1.0f - (eta * eta * (1.0f - idotn * idotn));
    if (rootTerm > 0.0f)
    {
        refrVec = (inc - idotn * normal) * eta - sqrtf(rootTerm) * normal;
    }
    return refrVec;
}

Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2)
{
    return Vector3(v1.y() * v2.z() - v1.z() * v2.y(),
                   v1.z() * v2.x() - v1.x() * v2.z(),
                   v1.x() * v2.y() - v1.y() * v2.x());
}

///////////////////////////////////////////////////////////////////////////////
//
// Sphere
//
///////////////////////////////////////////////////////////////////////////////

bool Sphere::makeAroundPnts(const Point3 *points, unsigned int num)
{
    // Algorithm from:
    //  An Efficient Bounding Sphere
    //  by Jack Ritter
    //  from "Graphics Gems", Academic Press, 1990

    if (num == 0)
        return false;

    Point3 xmin(INFINITE, 0.0f, 0.0f);
    Point3 ymin(0.0f, INFINITE, 0.0f);
    Point3 zmin(0.0f, 0.0f, INFINITE);
    Point3 xmax(-INFINITE, 0.0f, 0.0f);
    Point3 ymax(0.0f, -INFINITE, 0.0f);
    Point3 zmax(0.0f, 0.0f, -INFINITE);
    Point3 cur;

    unsigned int i;
    // FIRST PASS: find 6 minima/maxima points
    for (i = 0; i < num; i++)
    {
        cur = points[i];
        if (cur.x() < xmin.x())
            xmin = cur; // New x minimum point
        if (cur.x() > xmax.x())
            xmax = cur;
        if (cur.y() < ymin.y())
            ymin = cur; // New y minimum point
        if (cur.y() > ymax.y())
            ymax = cur;
        if (cur.z() < zmin.z())
            zmin = cur; // New z minimum point
        if (cur.z() > zmax.z())
            zmax = cur;
    }

    // Set xspan = distance between the 2 points xmin & xmax (squared)
    float xspan = xmax.distanceSquare(xmin);

    // Same for y & z spans
    float yspan = ymax.distanceSquare(ymin);
    float zspan = zmax.distanceSquare(zmin);

    // Set points dia1 & dia2 to the maximally separated pair
    Point3 dia1;
    Point3 dia2;
    dia1 = xmin; dia2 = xmax; // assume xspan biggest
    float maxspan = xspan;

    if (yspan > maxspan)
    {
        maxspan = yspan;
        dia1 = ymin; dia2 = ymax;
    }
    if (zspan > maxspan)
    {
        dia1 = zmin; dia2 = zmax;
    }

    // dia1,dia2 is a diameter of initial sphere
    // calc initial center
    m_center = 0.5f * dia1 + 0.5f * dia2;

    // calculate initial radius**2 and radius
    float rad_sq = dia2.distanceSquare(m_center);
    m_radius = sqrtf(rad_sq);

    // SECOND PASS: increment current sphere
    float old_to_p_sq, old_to_p;
    float old_to_new;

    for (i = 0; i < num; i++)
    {
        cur = points[i];
        old_to_p_sq = cur.distanceSquare(m_center);

        if (old_to_p_sq > rad_sq)  // do r**2 test first
        {
            // this point is outside of current sphere
            old_to_p = sqrtf(old_to_p_sq);
            // calc radius of new sphere
            m_radius = (m_radius + old_to_p) / 2.0f;
            rad_sq = m_radius * m_radius; // for next r**2 compare
            old_to_new = old_to_p - m_radius;
            // calc center of new sphere
            m_center.x() = (m_radius * m_center.x() + old_to_new * cur.x()) / old_to_p;
            m_center.y() = (m_radius * m_center.y() + old_to_new * cur.y()) / old_to_p;
            m_center.z() = (m_radius * m_center.z() + old_to_new * cur.z()) / old_to_p;
        }
    }

    return true;
}

EContainment Sphere::contains(const Point3& point) const
{
    float dist_sqr = m_center.distanceSquare(point);
    if (dist_sqr > getRadiusSqr())
        return CONT_NONE;
    return CONT_ALL;
}


///////////////////////////////////////////////////////////////////////////////
//
// Plane
//
///////////////////////////////////////////////////////////////////////////////

Plane::Plane(const Point3& p0, const Point3& p1, const Point3& p2)
{
    Vector3 edge0(p0, p1);
    Vector3 edge1(p0, p2);

    m_normal = Vector3::Cross(edge0, edge1);
    m_normal.normalize();

    m_offset = m_normal.dot(Vector3(p0.x(), p0.y(), p0.z()));
}

Point3 Plane::nearestPointOnPlane(const Point3& point) const
{
    Vector3 pv(point.x(), point.y(), point.z());
    float scaler = (m_offset + m_normal.dot(pv)) / m_normal.dot(m_normal);
    Vector3 near_pnt = pv - scaler * m_normal;
    return Point3(near_pnt.x(), near_pnt.y(), near_pnt.z());
}

float Plane::distance(const Point3& point) const
{
    Point3 near_pnt = nearestPointOnPlane(point);
    return near_pnt.distance(point);
}

EContainment Plane::contains(const Point3& point) const
{
    Vector3 pv(point.x(), point.y(), point.z());
    const float result = Vector3::Dot(m_normal, pv) - m_offset;
    if (result > 0.0f)
    {
        return CONT_ALL;
    }
    return CONT_NONE;
}

///////////////////////////////////////////////////////////////////////////////
//
// Axis-aligned box
//
///////////////////////////////////////////////////////////////////////////////

bool Box3::split(float plane, unsigned int axis, Box3& boxL, Box3& boxR) const
{
    assert(axis < 3);
    if (RangeOpen(plane, m_maxMin[axis+3], m_maxMin[axis]))
    {
        boxL = *this;
        boxR = *this;
        boxL.m_maxMin[axis]   = plane;
        boxR.m_maxMin[axis+3] = plane;

        assert(boxL.m_maxMin[axis+3] <= boxL.m_maxMin[axis]);
        assert(boxR.m_maxMin[axis+3] <= boxR.m_maxMin[axis]);

        return true;
    }
    return false;
}

void Box3::extendAround(const Point3& point)
{
    if (point.x() < m_maxMin[BI_MIN_X])
    {
        m_maxMin[BI_MIN_X] = point.x();
    }
    if (point.x() > m_maxMin[BI_MAX_X])
    {
        m_maxMin[BI_MAX_X] = point.x();
    }
    if (point.y() < m_maxMin[BI_MIN_Y])
    {
        m_maxMin[BI_MIN_Y] = point.y();
    }
    if (point.y() > m_maxMin[BI_MAX_Y])
    {
        m_maxMin[BI_MAX_Y] = point.y();
    }
    if (point.z() < m_maxMin[BI_MIN_Z])
    {
        m_maxMin[BI_MIN_Z] = point.z();
    }
    if (point.z() > m_maxMin[BI_MAX_Z])
    {
        m_maxMin[BI_MAX_Z] = point.z();
    }
}

EContainment Box3::contains(const Point3& point) const
{
    if (point.x() < m_maxMin[BI_MIN_X])
    {
        return CONT_NONE;
    }
    if (point.x() > m_maxMin[BI_MAX_X])
    {
        return CONT_NONE;
    }
    if (point.y() < m_maxMin[BI_MIN_Y])
    {
        return CONT_NONE;
    }
    if (point.y() > m_maxMin[BI_MAX_Y])
    {
        return CONT_NONE;
    }
    if (point.z() < m_maxMin[BI_MIN_Z])
    {
        return CONT_NONE;
    }
    if (point.z() > m_maxMin[BI_MAX_Z])
    {
        return CONT_NONE;
    }
    return CONT_ALL;
}

EContainment Box3::contains(const Box3& box) const
{
    bool xoverlap = (box.m_maxMin[BI_MIN_X] <= m_maxMin[BI_MAX_X]) && (box.m_maxMin[BI_MAX_X] >= m_maxMin[BI_MIN_X]);
    bool yoverlap = (box.m_maxMin[BI_MIN_Y] <= m_maxMin[BI_MAX_Y]) && (box.m_maxMin[BI_MAX_Y] >= m_maxMin[BI_MIN_Y]);
    bool zoverlap = (box.m_maxMin[BI_MIN_Z] <= m_maxMin[BI_MAX_Z]) && (box.m_maxMin[BI_MAX_Z] >= m_maxMin[BI_MIN_Z]);

    if (xoverlap && yoverlap && zoverlap)
    {
        bool xin = (m_maxMin[BI_MIN_X] <= box.m_maxMin[BI_MIN_X]) && (m_maxMin[BI_MAX_X] >= box.m_maxMin[BI_MAX_X]);
        bool yin = (m_maxMin[BI_MIN_Y] <= box.m_maxMin[BI_MIN_Y]) && (m_maxMin[BI_MAX_Y] >= box.m_maxMin[BI_MAX_Y]);
        bool zin = (m_maxMin[BI_MIN_Z] <= box.m_maxMin[BI_MIN_Z]) && (m_maxMin[BI_MAX_Z] >= box.m_maxMin[BI_MAX_Z]);

        if (xin && yin && zin)
            return CONT_ALL;
        else
            return CONT_PARTIAL;
    }
    return CONT_NONE;
}

int Box3::s_cornerIndex[NUM_CORNERS][3] =
{
    { BI_MAX_X, BI_MAX_Y, BI_MAX_Z },
    { BI_MIN_X, BI_MAX_Y, BI_MAX_Z },
    { BI_MIN_X, BI_MIN_Y, BI_MAX_Z },
    { BI_MAX_X, BI_MIN_Y, BI_MAX_Z },
    { BI_MAX_X, BI_MAX_Y, BI_MIN_Z },
    { BI_MIN_X, BI_MAX_Y, BI_MIN_Z },
    { BI_MIN_X, BI_MIN_Y, BI_MIN_Z },
    { BI_MAX_X, BI_MIN_Y, BI_MIN_Z }
};

int Box3::s_edges[NUM_EDGES][2] =
{
    { 0, 1 },
    { 1, 2 },
    { 2, 3 },
    { 3, 0 },

    { 4, 5 },
    { 5, 6 },
    { 6, 7 },
    { 7, 4 },

    { 0, 4 },
    { 1, 5 },
    { 2, 6 },
    { 3, 7 },
};

int Box3::s_faces[NUM_FACES][4] =
{
    { 0, 1, 2, 3 },
    { 4, 5, 6, 7 },
    { 0, 4, 5, 1 },
    { 1, 5, 6, 2 },
    { 2, 6, 7, 3 },
    { 3, 7, 4, 0 }
};

void Box3::getCorner(unsigned int which, Point3& corner) const
{
    assert(which < NUM_CORNERS);
    corner.x() = m_maxMin[s_cornerIndex[which][0]];
    corner.y() = m_maxMin[s_cornerIndex[which][1]];
    corner.z() = m_maxMin[s_cornerIndex[which][2]];
}

void Box3::getEdge(unsigned int which, int& c0, int& c1) const
{
    assert(which < NUM_EDGES);
    c0 = s_edges[which][0];
    c1 = s_edges[which][1];
}

void Box3::getFace(unsigned int which, int& c0, int& c1, int& c2, int& c3) const
{
    assert(which < NUM_FACES);
    c0 = s_faces[which][0];
    c1 = s_faces[which][1];
    c2 = s_faces[which][2];
    c3 = s_faces[which][3];
}

void Box3::getDims(Vector3& dims) const
{
    dims = Vector3(m_maxMin[BI_MAX_X], m_maxMin[BI_MAX_Y], m_maxMin[BI_MAX_Z]) -
           Vector3(m_maxMin[BI_MIN_X], m_maxMin[BI_MIN_Y], m_maxMin[BI_MIN_Z]);
}

float Box3::getSurfaceArea() const
{
    Vector3 dims;
    getDims(dims);
    return 2.0f * (dims.x()*dims.y() +
                   dims.x()*dims.z() +
                   dims.y()*dims.z());
}

///////////////////////////////////////////////////////////////////////////////
//
// Oriented box
//
///////////////////////////////////////////////////////////////////////////////

OrientedBox::OrientedBox(const Box3& box)
{
    m_center.set(box.m_maxMin[Box3::BI_MAX_X] + box.m_maxMin[Box3::BI_MIN_X],
                 box.m_maxMin[Box3::BI_MAX_Y] + box.m_maxMin[Box3::BI_MIN_Y],
                 box.m_maxMin[Box3::BI_MAX_Z] + box.m_maxMin[Box3::BI_MIN_Z]);

    m_center *= 0.5f;

    m_bu.set(1.0f, 0.0f, 0.0f);
    m_bv.set(0.0f, 1.0f, 0.0f);
    m_bw.set(0.0f, 0.0f, 1.0f);

    m_hu = (box.m_maxMin[Box3::BI_MAX_X] - box.m_maxMin[Box3::BI_MIN_X]) / 2.0f;
    m_hv = (box.m_maxMin[Box3::BI_MAX_Y] - box.m_maxMin[Box3::BI_MIN_Y]) / 2.0f;
    m_hw = (box.m_maxMin[Box3::BI_MAX_Z] - box.m_maxMin[Box3::BI_MIN_Z]) / 2.0f;
}

EContainment OrientedBox::contains(const Point3& point) const
{
    return CONT_ALL;
}


///////////////////////////////////////////////////////////////////////////////
//
// Intersection methods
//
///////////////////////////////////////////////////////////////////////////////

bool Intersect(const Ray& ray, const Sphere& sphere, RayHit& h0, RayHit& h1)
{
    Vector3 dist(ray.get_origin(), sphere.getCenter());

    const float dirDotDist = dist.dot(ray.get_direction());
    const float distDotDist = dist.dot(dist);
    const float disc = dirDotDist * dirDotDist - (distDotDist - sphere.getRadiusSqr());

    bool hit = (disc >= 0.0f) ? true : false;

    if (hit)
    {
        float tEnter = -dirDotDist - sqrtf(disc);
        float tLeave = -dirDotDist + sqrtf(disc);

        // Compute the entry point.
        h0.m_t = tEnter;
        h0.m_point = ray.point_at(tEnter);
        h0.m_norm = Vector3(sphere.getCenter(), h0.m_point) * sphere.getInvRadius();

        // Compute the exit point.
        h1.m_t = tLeave;
        h1.m_point = ray.point_at(tLeave);
        h1.m_norm = Vector3(sphere.getCenter(), h1.m_point) * sphere.getInvRadius();
    }

    return hit;
}

bool Intersect(const Ray& ray, const Box3& box, float& t0, float& t1)
{
    for (int i = 0; i < 3; i++)
    {
        const float invRayDir = 1.0f / ray.m_dir.at(i);
        float tNear = (box.m_maxMin[i+3] - ray.m_origin.at(i)) * invRayDir;
        float tFar  = (box.m_maxMin[i] - ray.m_origin.at(i)) * invRayDir;
        if (tNear > tFar)
            Swap(tNear, tFar);
        t0 = tNear > t0 ? tNear : t0;
        t1 = tFar  < t1 ? tFar  : t1;
        if (t0 > t1)
            return false;
    }
    return true;
}

bool Intersect(const Ray& ray, const Plane& plane, RayHit& h0)
{
    bool hit = false;

    // Solve P dot R = 0.0
    float planeDotRayOrigin = Vector3::Dot(plane.m_normal, ray.m_origin);
    float planeDotRayDir = Vector3::Dot(plane.m_normal, ray.m_dir);

    // Detect co-planer ray/plane
    if (Equal(planeDotRayDir, 0.0f, EPSILON))
    {
        h0.m_t = 0.0f;
    }
    else
    {
        h0.m_t = -(plane.m_offset + planeDotRayOrigin) / planeDotRayDir;
        h0.m_norm = plane.m_normal;
        h0.m_point = ray.point_at(h0.m_t);
        hit = true;
    }
    return hit;
}

bool Intersect(const Ray& ray, const Triangle& tri, RayHit& h0)
{
    //
    // Tomas Moller and Ben Trumbore, "Fast Minimum Storage Ray-Triangle Intersection,"
    // Journal of Graphics Tools, Vol. 2, No. 1, pp. 21-28, 1997.
    //

    // Find vectors for two edges sharing v0.
    Vector3 edge1(tri.m_v1 - tri.m_v0);
    Vector3 edge2(tri.m_v2 - tri.m_v0);

    // Begin calculating determinant - also used to calculate U parameter.
    Vector3 pvec = Vector3::Cross(ray.get_direction(), edge2);

    // If determinant is near zero, ray lies in plane of triangle.
    float det = edge1.dot(pvec);

    if (det < EPSILON)
        return false;

    // Calculate distance from v0 to ray origin.
    Vector3 tvec(ray.get_origin() - tri.m_v0);

    // calculate U parameter and test bounds.
    float u = tvec.dot(pvec);
    if (u < 0.0f || u > det)
        return false;

    // Prepare to test V parameter.
    Vector3 qvec = Vector3::Cross(tvec, edge1);

    // Calculate V parameter and test bounds.
    float v = ray.get_direction().dot(qvec);
    if (v < 0.0f || u + v > det)
        return false;

    // Calculate t, scale parameters, ray intersects triangle.
    h0.m_t = edge2.dot(qvec);
    const float inv_det = 1.0f / det;
    h0.m_t *= inv_det;
    u *= inv_det;
    v *= inv_det;


    h0.m_point = (1.0f - u - v) * tri.m_v0 + u * tri.m_v1 + v * tri.m_v2;
    h0.m_norm = Vector3::Cross(edge1, edge2);
    h0.m_norm.normalize();
    h0.m_bary.set(1.0f - u - v, u, v);

    return true;
}

float Triangle::getArea() const
{
    Vector3 u(m_v1 - m_v0);
    Vector3 v(m_v2 - m_v0);
    Vector3 uv = Vector3::Cross(u, v);
    return 0.5f * uv.length();
}

TriangleFast::TriangleFast(const Triangle& tri)
{
    m_v0 = tri.m_v0;

    // Find vectors for two edges sharing v0.
    Vector3 c(tri.m_v1 - tri.m_v0);
    Vector3 b(tri.m_v2 - tri.m_v0);

    // Compute normal
    Vector3 N = Vector3::Cross(b, c);

    // Identify primary plane
    if (Abs(N.x()) > Abs(N.y()))
    {
        if (Abs(N.x()) > Abs(N.z()))
            m_k = COORD_X;
        else
            m_k = COORD_Z;
    }
    else
    {
        if (Abs(N.y()) > Abs(N.z()))
            m_k = COORD_Y;
        else
            m_k = COORD_Z;
    }

    // Compute triangle plane coefficients in projection plane
    int u = (m_k+1) % 3;
    int v = (m_k+2) % 3;
    float invNormPP = 1.0f / N.at(m_k);

    m_nu = N.at(u) * invNormPP;
    m_nv = N.at(v) * invNormPP;
    m_nd = N.dot(tri.m_v0) * invNormPP;

    // Compute projection plane edge equations
    float invDet = 1.0f / (b.at(u) * c.at(v) - b.at(v) * c.at(u));
    m_bnu = b.at(u) * invDet;
    m_bnv = -b.at(v) * invDet;

    m_cnu = c.at(v) * invDet;
    m_cnv = -c.at(u) * invDet;
}

bool Intersect(const Ray& ray, const TriangleFast& accel, RayHit& h0)
{
    //
    // "Real Time Ray Tracing and Interactive Global Illumination", Ingo Wald:
    // http://www.mpi-sb.mpg.de/~wald/PhD/
    //
    // Jakko Bikker
    // http://www.flipcode.com/articles/article_raytrace07.shtml
    //
    static int axisModulo[] = { 0, 1, 2, 0, 1 };
    const int ku = axisModulo[accel.m_k+1];
    const int kv = axisModulo[accel.m_k+2];

    const float nd = 1.0f / (ray.m_dir.at(accel.m_k) + accel.m_nu * ray.m_dir.at(ku) + accel.m_nv * ray.m_dir.at(kv));
    float t = (accel.m_nd - ray.m_origin.at(accel.m_k) - accel.m_nu * ray.m_origin.at(ku) - accel.m_nv * ray.m_origin.at(kv)) * nd;

    if (t < 0.0f)
    {
        return false;
    }

    const float hu = ray.m_origin.at(ku) + t * ray.m_dir.at(ku) - accel.m_v0.at(ku);
    const float hv = ray.m_origin.at(kv) + t * ray.m_dir.at(kv) - accel.m_v0.at(kv);

    const float u = hv * accel.m_bnu + hu * accel.m_bnv;
    if (u < 0.0f)
        return false;

    const float v = hu * accel.m_cnu + hv * accel.m_cnv;
    if (v < 0.0f)
        return false;

    if (u + v > 1.0f)
        return false;

    h0.m_t = t;
    h0.m_bary.set(1.0f - u - v, u, v);

    return true;
}

/********************************************************/
/* AABB-triangle overlap test code                      */
/* by Tomas Akenine-M�ller                              */
/* Function: int triBoxOverlap(float boxcenter[3],      */
/*          float boxhalfsize[3],float triverts[3][3]); */
/* History:                                             */
/*   2001-03-05: released the code in its first version */
/*   2001-06-18: changed the order of the tests, faster */
/*                                                      */
/* Acknowledgement: Many thanks to Pierre Terdiman for  */
/* suggestions and discussions on how to optimize code. */
/* Thanks to David Hunt for finding a ">="-bug!         */
/********************************************************/

bool planeBoxOverlap(const Vector3& normal, const Vector3& vert, const Vector3& maxbox)
{
    int q;

    float v;
    Vector3 vmin, vmax;

    for (q = 0; q < 3; q++)
    {
        v = vert.at(q);                    // -NJMP-
        if (normal.at(q) > 0.0f)
        {
            vmin.at(q) = -maxbox.at(q) - v;   // -NJMP-
            vmax.at(q) =  maxbox.at(q) - v;   // -NJMP-
        }
        else
        {
            vmin.at(q) =  maxbox.at(q) - v;   // -NJMP-
            vmax.at(q) = -maxbox.at(q) - v;   // -NJMP-
        }
    }

    if (Vector3::Dot(normal, vmin) > 0.0f)
        return false;   // -NJMP-

    if (Vector3::Dot(normal, vmax) >= 0.0f)
        return true;    // -NJMP-

    return false;
}

#define FINDMINMAX(x0,x1,x2,min,max) \
  min = max = x0;   \
  if(x1<min) min=x1;\
  if(x1>max) max=x1;\
  if(x2<min) min=x2;\
  if(x2>max) max=x2;




/*======================== X-tests ========================*/

#define AXISTEST_X01(a, b, fa, fb)             \
    p0 = a*v0.y() - b*v0.z();            \
    p2 = a*v2.y() - b*v2.z();            \
    if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
    rad = fa * boxhalfsize.y() + fb * boxhalfsize.z();   \
    if(min>rad || max<-rad) return false;

#define AXISTEST_X2(a, b, fa, fb)              \
    p0 = a*v0.y() - b*v0.z();                    \
    p1 = a*v1.y() - b*v1.z();                    \
    if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
    rad = fa * boxhalfsize.y() + fb * boxhalfsize.z();   \
    if(min>rad || max<-rad) return false;



/*======================== Y-tests ========================*/

#define AXISTEST_Y02(a, b, fa, fb)             \
    p0 = -a*v0.x() + b*v0.z();                   \
    p2 = -a*v2.x() + b*v2.z();                       \
    if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
    rad = fa * boxhalfsize.x() + fb * boxhalfsize.z();   \
    if(min>rad || max<-rad) return false;

#define AXISTEST_Y1(a, b, fa, fb)              \
    p0 = -a*v0.x() + b*v0.z();                   \
    p1 = -a*v1.x() + b*v1.z();                       \
    if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
    rad = fa * boxhalfsize.x() + fb * boxhalfsize.z();   \
    if(min>rad || max<-rad) return false;


/*======================== Z-tests ========================*/

#define AXISTEST_Z12(a, b, fa, fb)             \
    p1 = a*v1.x() - b*v1.y();                    \
    p2 = a*v2.x() - b*v2.y();                    \
    if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
    rad = fa * boxhalfsize.x() + fb * boxhalfsize.y();   \
    if(min>rad || max<-rad) return false;

#define AXISTEST_Z0(a, b, fa, fb)              \
    p0 = a*v0.x() - b*v0.y();                \
    p1 = a*v1.x() - b*v1.y();                    \
    if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
    rad = fa * boxhalfsize.x() + fb * boxhalfsize.y();   \
    if(min>rad || max<-rad) return false;


bool triBoxOverlap(const Point3& boxcenter, const Vector3& boxhalfsize, Point3 triverts[3])
{
  /*    use separating axis theorem to test overlap between triangle and box */
  /*    need to test for overlap in these directions: */
  /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
  /*       we do not even need to test these) */
  /*    2) normal of the triangle */
  /*    3) crossproduct(edge from tri, {x,y,z}-directin) */
  /*       this gives 3x3=9 more tests */
    Vector3 v0,v1,v2;
//   float axis[3];
   float min,max,p0,p1,p2,rad,fex,fey,fez;      // -NJMP- "d" local variable removed
   Vector3 normal,e0,e1,e2;

   /* This is the fastest branch on Sun */
   /* move everything so that the boxcenter is in (0,0,0) */
   v0 = Vector3(boxcenter, triverts[0]);
   v1 = Vector3(boxcenter, triverts[1]);
   v2 = Vector3(boxcenter, triverts[2]);

   /* compute triangle edges */
   e0 = v1 - v0;      /* tri edge 0 */
   e1 = v2 - v1;      /* tri edge 1 */
   e2 = v0 - v2;      /* tri edge 2 */

   /* Bullet 3:  */
   /*  test the 9 tests first (this was faster) */
   fex = fabsf(e0.x());
   fey = fabsf(e0.y());
   fez = fabsf(e0.z());

   AXISTEST_X01(e0.z(), e0.y(), fez, fey);
   AXISTEST_Y02(e0.z(), e0.x(), fez, fex);
   AXISTEST_Z12(e0.y(), e0.x(), fey, fex);

   fex = fabsf(e1.x());
   fey = fabsf(e1.y());
   fez = fabsf(e1.z());

   AXISTEST_X01(e1.z(), e1.y(), fez, fey);
   AXISTEST_Y02(e1.z(), e1.x(), fez, fex);
   AXISTEST_Z0(e1.y(), e1.x(), fey, fex);

   fex = fabsf(e2.x());
   fey = fabsf(e2.y());
   fez = fabsf(e2.z());

   AXISTEST_X2(e2.z(), e2.y(), fez, fey);
   AXISTEST_Y1(e2.z(), e2.x(), fez, fex);
   AXISTEST_Z12(e2.y(), e2.x(), fey, fex);

   /* Bullet 1: */
   /*  first test overlap in the {x,y,z}-directions */
   /*  find min, max of the triangle each direction, and test for overlap in */
   /*  that direction -- this is equivalent to testing a minimal AABB around */
   /*  the triangle against the AABB */

   /* test in X-direction */
   FINDMINMAX(v0.x(),v1.x(),v2.x(),min,max);
   if(min>boxhalfsize.x() || max<-boxhalfsize.x()) return false;

   /* test in Y-direction */
   FINDMINMAX(v0.y(),v1.y(),v2.y(),min,max);
   if(min>boxhalfsize.y() || max<-boxhalfsize.y()) return false;

   /* test in Z-direction */
   FINDMINMAX(v0.z(),v1.z(),v2.z(),min,max);
   if(min>boxhalfsize.z() || max<-boxhalfsize.z()) return false;

   /* Bullet 2: */
   /*  test if the box intersects the plane of the triangle */
   /*  compute plane equation of triangle: normal*x+d=0 */
   normal = Vector3::Cross(e0, e1);

   // -NJMP- (line removed here)
   if(!planeBoxOverlap(normal,v0,boxhalfsize)) return false;    // -NJMP-

   return true;   /* box and triangle overlaps */
}

bool Intersect(const Triangle& tri, const Box3& box)
{
    Point3 boxCenter;
    box.getCenter(boxCenter);

    Vector3 boxHalfsize;
    box.getDims(boxHalfsize);
    boxHalfsize *= 0.5f;

    Point3 triVerts[3];
    triVerts[0] = tri.m_v0;
    triVerts[1] = tri.m_v1;
    triVerts[2] = tri.m_v2;
    return triBoxOverlap(boxCenter, boxHalfsize, triVerts);
}


bool Intersect(const Plane& plane, const Box3& box, IntersectInfo& hitInfo)
{
    return false;
}

bool Intersect(const Plane& plane, const OrientedBox& box, IntersectInfo& hitInfo)
{
    return false;
}

} // namespace rgl
