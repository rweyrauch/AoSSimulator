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

    constexpr std::array<int, 4> g_commandTraitsScinari = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Spellmaster),
            ToInteger(CommandTrait::Loremaster),
            ToInteger(CommandTrait::Warmaster),
    };

    constexpr std::array<int, 4> g_commandTraitsAlarith = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Majestic),
            ToInteger(CommandTrait::Enduring),
            ToInteger(CommandTrait::Loremaster),
    };

    constexpr std::array<int, 4> g_artefactsScinari = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Phoenix_Stone),
            ToInteger(Artefact::Silver_Wand),
            ToInteger(Artefact::Blade_Of_Leaping_Gold),
    };

    constexpr std::array<int, 4> g_artefactsAlarith = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Heartstone_Amulet),
            ToInteger(Artefact::Ebony_Stone),
            ToInteger(Artefact::Magmic_Hammer)
    };

    constexpr std::array<int, 7> g_loreOfHysh = {
            ToInteger(Lore::None),
            ToInteger(Lore::Speed_Of_Hysh),
            ToInteger(Lore::Solar_Flare),
            ToInteger(Lore::Lambent_Light),
            ToInteger(Lore::Etheral_Blessing),
            ToInteger(Lore::Total_Eclipse),
            ToInteger(Lore::Protection_Of_Hysh),
    };

    constexpr std::array<int, 7> g_loreOfHighPeaks = {
            ToInteger(Lore::None),
            ToInteger(Lore::Unyielding_Calm),
            ToInteger(Lore::Crippling_Vertigo),
            ToInteger(Lore::Voice_Of_The_Mountains),
            ToInteger(Lore::Living_Fissue),
            ToInteger(Lore::Entomb),
            ToInteger(Lore::Assault_Of_Stone),
    };

    constexpr std::array<int, 13> g_loreTeclis = {
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
            ToInteger(Lore::Assault_Of_Stone)
    };

    class PowerOfHysh : public Spell {
    public:
        explicit PowerOfHysh(Unit *caster);

    protected:

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override;

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }

    };

    Spell *CreateLore(Lore which, Unit *caster);

} // namespace LuminethRealmLords

