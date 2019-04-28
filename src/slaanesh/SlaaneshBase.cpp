/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/SlaaneshBase.h>
#include "slaanesh/Fiends.h"
#include "slaanesh/Seekers.h"
#include "slaanesh/Daemonettes.h"
#include "slaanesh/KeeperOfSecrets.h"
#include "slaanesh/Hellstriders.h"
#include "slaanesh/ShalaxiHelbane.h"
#include "slaanesh/BladebringerExaltedChariot.h"
#include "slaanesh/BladebringerHellflayer.h"
#include "slaanesh/BladebringerSeekerChariot.h"
#include "slaanesh/ExaltedChariot.h"
#include "slaanesh/Hellflayer.h"
#include "slaanesh/SeekerChariots.h"
#include "slaanesh/TheMasque.h"
#include "slaanesh/Viceleader.h"
#include "slaanesh/InfernalEnrapturess.h"
#include "slaanesh/SyllEsske.h"
#include "slaanesh/ContortedEpitome.h"

namespace Slaanesh
{

void Init()
{
    Daemonettes::Init();
    Fiends::Init();
    Seekers::Init();
    KeeperOfSecrets::Init();
    Hellstriders::Init();
    ShalaxiHelbane::Init();
    BladebringerOnExaltedChariot::Init();
    BladebringerOnHellflayer::Init();
    BladebringerOnSeekerChariot::Init();
    ExaltedChariot::Init();
    Hellflayer::Init();
    SeekerChariots::Init();
    TheMasque::Init();
    ViceleaderHeraldOfSlaanesh::Init();
    InfernalEnrapturess::Init();
    SyllEsske::Init();
    TheContortedEpitome::Init();
}

} // namespace Slaanesh
