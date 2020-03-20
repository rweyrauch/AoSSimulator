/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/BeastsOfChaosBase.h>
#include "beastsofchaos/Gors.h"
#include "beastsofchaos/Ghorgon.h"
#include "beastsofchaos/Ungors.h"
#include "beastsofchaos/Bestigors.h"
#include "beastsofchaos/Bullgors.h"
#include "beastsofchaos/Cygor.h"
#include "beastsofchaos/Centigors.h"
#include "beastsofchaos/Doombull.h"
#include "beastsofchaos/Chimera.h"
#include "beastsofchaos/Beastlord.h"
#include "beastsofchaos/GreatBrayShaman.h"
#include "beastsofchaos/UngorRaiders.h"
#include "beastsofchaos/TuskgorChariots.h"
#include "beastsofchaos/Cockatrice.h"
#include "beastsofchaos/ChaosSpawn.h"
#include "beastsofchaos/ChaosWarhounds.h"
#include "beastsofchaos/Jabberslythe.h"
#include "beastsofchaos/DragonOgors.h"
#include "beastsofchaos/DragonOgorShaggoth.h"
#include "beastsofchaos/Razorgors.h"

namespace BeastsOfChaos {

    void BeastsOfChaosBase::setGreatfray(BeastsOfChaosBase::Greatfray fray) {

    }

    std::string BeastsOfChaosBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Greatfray") {
            if (parameter.intValue == Allherd) { return "Allherd"; }
            else if (parameter.intValue == Darkwalkers) { return "Darkwalkers"; }
            else if (parameter.intValue == Gavespawn) { return "Gavespawn"; }
            else if (parameter.intValue == None) { return "None"; }
        }
        return ParameterValueToString(parameter);
    }

    int BeastsOfChaosBase::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Allherd") { return Allherd; }
        else if (enumString == "Darkwalkers") { return Darkwalkers; }
        else if (enumString == "Gavespawn") { return Gavespawn; }
        else if (enumString == "None") { return None; }
        return 0;
    }

    void Init() {
        Beastlord::Init();
        Bestigors::Init();
        Bullgors::Init();
        Centigors::Init();
        ChaosSpawn::Init();
        ChaosWarhounds::Init();
        Chimera::Init();
        Cockatrice::Init();
        Cygor::Init();
        Doombull::Init();
        DragonOgorShaggoth::Init();
        DragonOgors::Init();
        Ghorgon::Init();
        Gors::Init();
        GreatBrayShaman::Init();
        Jabberslythe::Init();
        Razorgors::Init();
        TuskgorChariots::Init();
        UngorRaiders::Init();
        Ungors::Init();
    }

} // namespace BeastsOfChaos
