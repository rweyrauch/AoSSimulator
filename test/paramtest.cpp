/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include <UnitFactory.h>
#include "AgeOfSigmarSim.h"

TEST(Parameters, Valid)
{
    for (auto ruip = UnitFactory::RegisteredUnitsBegin(); ruip != UnitFactory::RegisteredUnitsEnd(); ++ruip)
    {
        for (auto pip : ruip->second.m_parameters)
        {
            if (pip.paramType == ParamType::Integer)
            {
            }
            else if (pip.paramType == ParamType::Boolean)
            {
            }
            else if (pip.paramType == ParamType::Enum)
            {
                Parameter param = pip;
                for (auto e = pip.minValue; e < pip.maxValue; e++)
                {
                    param.intValue = e;
                    const auto str = ruip->second.m_paramToString(param);
                    ASSERT_TRUE(!str.empty());
                }
            }
        }
    }
}