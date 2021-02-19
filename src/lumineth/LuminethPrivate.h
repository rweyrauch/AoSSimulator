/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <lumineth/LuminethRealmLords.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace LuminethRealmLords {

    constexpr std::array<int, 5> g_greatNations = {
            ToInteger(GreatNation::None),
            ToInteger(GreatNation::Ymetrica),
            ToInteger(GreatNation::Iliatha),
            ToInteger(GreatNation::Syar),
            ToInteger(GreatNation::Zaitrec)
    };

    constexpr std::array<int, 11> g_commandTraits = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Lords_Of_Brilliance),
            ToInteger(CommandTrait::Lords_Of_Stone),
            ToInteger(CommandTrait::Spellmaster),
            ToInteger(CommandTrait::Loremaster),
            ToInteger(CommandTrait::Warmaster),
            ToInteger(CommandTrait::Majestic),
            ToInteger(CommandTrait::Enduring),
            ToInteger(CommandTrait::Almighty_Blow),
            ToInteger(CommandTrait::Goading_Arrogance),
            ToInteger(CommandTrait::Fast_Learner)
    };

    constexpr std::array<int, 11> g_artefacts = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Phoenix_Stone),
            ToInteger(Artefact::Silver_Wand),
            ToInteger(Artefact::Blade_Of_Leaping_Gold),
            ToInteger(Artefact::Heartstone_Amulet),
            ToInteger(Artefact::Ebony_Stone),
            ToInteger(Artefact::Magmic_Hammer),
            ToInteger(Artefact::Mountains_Gift),
            ToInteger(Artefact::The_Perfect_Blade),
            ToInteger(Artefact::Simulacra_Amulet),
            ToInteger(Artefact::Gift_Of_Celennar)
    };


    constexpr std::array<int, 14> g_lore = {
            ToInteger(Lore::None),
            ToInteger(Lore::Speed_Of_Hysh),
            ToInteger(Lore::Solar_Flare),
            ToInteger(Lore::Lambent_Light),
            ToInteger(Lore::Etheral_Blessing),
            ToInteger(Lore::Total_Eclipse),
            ToInteger(Lore::Protection_Of_Hysh),
            ToInteger(Lore::Unyielding_Calm),
            ToInteger(Lore::Crippling_Vertigo),
            ToInteger(Lore::Voice_Of_The_Mountains),
            ToInteger(Lore::Living_Fissue),
            ToInteger(Lore::Entomb),
            ToInteger(Lore::Assault_Of_Stone),
            ToInteger(Lore::Overwhelming_Heat)
    };

    class PowerOfHysh : public Spell {
    public:
        explicit PowerOfHysh(Unit *caster);

    protected:

        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override;
        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }

    };

    Spell* CreateLore(Lore which, Unit* caster);

} // namespace LuminethRealmLords

