/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "wanderers/GladeGuard.h"
#include "wanderers/EternalGuard.h"
#include "wanderers/WildwoodRangers.h"

namespace Wanderers
{

void Init()
{
    GladeGuard::Init();
    EternalGuard::Init();
    WildwoodRangers::Init();
}

} // namespace Wanderers