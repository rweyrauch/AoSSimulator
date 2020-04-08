/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gtest/gtest.h>
#include <MathUtils.h>

using namespace Math;

TEST(Circle, POI_CenterHit) {

    Circle c1({0, 0}, 1.0);
    Circle c2({3, 0}, 1.0);

    double poi = -1.0;
    auto hit = PointOfIntersection(c1, {3, 0}, c2, poi);
    ASSERT_TRUE(hit);
    ASSERT_DOUBLE_EQ(poi, 1.0);

    c2.center() = {3, 1};
    hit = PointOfIntersection(c1, {3, 1}, c2, poi);
    ASSERT_TRUE(hit);
    auto expectedPoi = Vector2(c1.center(), c2.center()).length() - (c1.radius() + c2.radius());
    ASSERT_NEAR(poi, expectedPoi, EPSILON);

    hit = PointOfIntersection(c2, {-3, -1}, c1, poi);
    ASSERT_TRUE(hit);
    ASSERT_NEAR(poi, expectedPoi, EPSILON);
}

TEST(Circle, POI_GlancingHit) {

    Circle c1({0, 0}, 1.0);
    Circle c2({3, 1}, 1.0);

    double poi = -1.0;
    auto hit = PointOfIntersection(c1, {30, 20}, c2, poi);
    ASSERT_TRUE(hit);
    ASSERT_GE(hit, 1.0);

    hit = PointOfIntersection(c1, {10, 0}, c2, poi);
    ASSERT_TRUE(hit);
    ASSERT_GE(hit, 1.0);

    hit = PointOfIntersection(c2, {-10, 0}, c1, poi);
    ASSERT_TRUE(hit);
    ASSERT_GE(hit, 1.0);
}

TEST(Circle, POI_NoHit) {
    Circle c1({0, 0}, 1.0);
    Circle c2({3, 1}, 1.0);

    double poi = -1.0;
    auto hit = PointOfIntersection(c1, {-3, 2}, c2, poi);
    ASSERT_FALSE(hit);

    hit = PointOfIntersection(c1, {3, -2}, c2, poi);
    ASSERT_FALSE(hit);

    hit = PointOfIntersection(c1, {0.5, 3}, c2, poi);
    ASSERT_FALSE(hit);
}