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

namespace Ironjawz
{

void Init()
{
    OrrukArdboys::Init();
    OrrukBrutes::Init();
    OrrukGoreGruntas::Init();
    OrrukMegaboss::Init();
}

} // namespace Ironjawz

