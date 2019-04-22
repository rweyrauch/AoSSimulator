/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/Fyreslayer.h>

#include "fyreslayers/AuricHearthguard.h"
#include "fyreslayers/Battlesmith.h"
#include "fyreslayers/Doomseeker.h"
#include "fyreslayers/HearthguardBerzerkers.h"
#include "fyreslayers/GrimwrathBerzerker.h"
#include "fyreslayers/VulkiteBerzerkers.h"
#include "fyreslayers/AuricRunefather.h"
#include "fyreslayers/AuricRunemaster.h"
#include "fyreslayers/AuricRuneson.h"
#include "fyreslayers/AuricRunesmiterOnMagmadroth.h"
#include "fyreslayers/AuricRunesonOnMagmadroth.h"
#include "fyreslayers/AuricRunefatherOnMagmadroth.h"
#include "fyreslayers/AuricRunesmiter.h"

namespace Fyreslayers
{

void Init()
{
    AuricHearthguard::Init();
    Battlesmith::Init();
    GrimwrathBerzerker::Init();
    HearthguardBerzerkers::Init();
    Doomseeker::Init();
    VulkiteBerzerkers::Init();
    AuricRunefather::Init();
    AuricRunemaster::Init();
    AuricRuneson::Init();
    AuricRunesmiter::Init();
    AuricRunesmiterOnMagmadroth::Init();
    AuricRunefatherOnMagmadroth::Init();
    AuricRunesonOnMagmadroth::Init();
}

} // namespace Fyreslayers
