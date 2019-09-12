/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include "AgeOfSigmarSim.h"

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);

    Initialize();

    return RUN_ALL_TESTS();
}