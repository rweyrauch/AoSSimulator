/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/Ironjawz.h>

#include "ironjawz/OrrukArdboys.h"
#include "ironjawz/OrrukBrutes.h"
#include "ironjawz/GoreGruntas.h"
#include "ironjawz/OrrukMegaboss.h"
#include "ironjawz/OrrukWarchanter.h"
#include "ironjawz/Gordrakk.h"
#include "ironjawz/MegabossOnMawKrusha.h"

namespace Ironjawz
{

void Init()
{
    OrrukArdboys::Init();
    OrrukBrutes::Init();
    OrrukGoreGruntas::Init();
    OrrukMegaboss::Init();
    OrrukWarchanter::Init();
    GordrakkTheFistOfGork::Init();
    MegabossOnMawKrusha::Init();
}

} // namespace Ironjawz

