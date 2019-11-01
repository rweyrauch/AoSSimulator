/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "ossiarch/OssiarchBonereaperBase.h"

#include "ossiarch/MortekGuard.h"
#include "ossiarch/NecropolisStalkers.h"
#include "ossiarch/Vokmortian.h"
#include "ossiarch/ImmortisGuard.h"
#include "ossiarch/KavalosDeathriders.h"
#include "ossiarch/GothizzarHarvester.h"
#include "ossiarch/MortekCrawler.h"
#include "ossiarch/OrpheonKatakros.h"
#include "ossiarch/LiegeKavalos.h"

namespace OssiarchBonereapers
{

std::string OssiarchBonereaperBase::ValueToString(const Parameter &parameter)
{
    return ParameterValueToString(parameter);
}

int OssiarchBonereaperBase::EnumStringToInt(const std::string &enumString)
{
    return 0;
}

void Init()
{
    GothizzarHarvester::Init();
    ImmortisGuard::Init();
    KavalosDeathriders::Init();
    LiegeKavalos::Init();
    MortekCrawler::Init();
    MortekGuard::Init();
    NecropolisStalkers::Init();
    OrpheonKatakros::Init();
    Vokmortian::Init();
}

} // namespace OssiarchBonereapers
