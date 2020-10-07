/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LUMINETHPRIVATE_H
#define LUMINETHPRIVATE_H

#include <lumineth/LuminethRealmLords.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace LuminethRealmLords {

    constexpr std::array<int, 5> g_greatNations = {
            to_integer(GreatNation::None),
            to_integer(GreatNation::Ymetrica),
            to_integer(GreatNation::Iliatha),
            to_integer(GreatNation::Syar),
            to_integer(GreatNation::Zaitrec)
    };

    constexpr std::array<int, 11> g_commandTraits = {
            to_integer(CommandTrait::None),
            to_integer(CommandTrait::Lords_of_Brilliance),
            to_integer(CommandTrait::Lords_of_Stone),
            to_integer(CommandTrait::Spellmaster),
            to_integer(CommandTrait::Loremaster),
            to_integer(CommandTrait::Warmaster),
            to_integer(CommandTrait::Majestic),
            to_integer(CommandTrait::Enduring),
            to_integer(CommandTrait::Almighty_Blow),
            to_integer(CommandTrait::Goading_Arrogance),
            to_integer(CommandTrait::Fast_Learner)
    };

    constexpr std::array<int, 11> g_artefacts = {
            to_integer(Artefact::None),
            to_integer(Artefact::Phoenix_Stone),
            to_integer(Artefact::Silver_Wand),
            to_integer(Artefact::Blade_of_Leaping_Gold),
            to_integer(Artefact::Heartstone_Amulet),
            to_integer(Artefact::Ebony_Stone),
            to_integer(Artefact::Magmic_Hammer),
            to_integer(Artefact::Mountains_Gift),
            to_integer(Artefact::The_Perfect_Blade),
            to_integer(Artefact::Simulacra_Amulet),
            to_integer(Artefact::Gift_of_Celennar)
    };


    constexpr std::array<int, 14> g_lore = {
            to_integer(Lore::None),
            to_integer(Lore::Speed_of_Hysh),
            to_integer(Lore::Solar_Flare),
            to_integer(Lore::Lambent_Light),
            to_integer(Lore::Etheral_Blessing),
            to_integer(Lore::Total_Eclipse),
            to_integer(Lore::Protection_of_Hysh),
            to_integer(Lore::Unyielding_Calm),
            to_integer(Lore::Crippling_Vertigo),
            to_integer(Lore::Voice_of_the_Mountains),
            to_integer(Lore::Living_Fissue),
            to_integer(Lore::Entomb),
            to_integer(Lore::Assault_of_Stone),
            to_integer(Lore::Overwhelming_Heat)
    };

    class PowerOfHysh : public Spell {
    public:
        explicit PowerOfHysh(Unit *caster);

        Result cast(Unit *target, int round) override;

        Result cast(double x, double y, int round) override { return Failed; }

    };

} // namespace LuminethRealmLords

#endif // LUMINETHPRIVATE_H
