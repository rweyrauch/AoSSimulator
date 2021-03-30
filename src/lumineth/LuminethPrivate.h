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

    constexpr std::array<int, 7> g_greatNations = {
            ToInteger(GreatNation::None),
            ToInteger(GreatNation::Ymetrica),
            ToInteger(GreatNation::Iliatha),
            ToInteger(GreatNation::Syar),
            ToInteger(GreatNation::Zaitrec),
            ToInteger(GreatNation::Alumnia),
            ToInteger(GreatNation::Helon)
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

    constexpr std::array<int, 4> g_commandTraitsVanari = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Grand_Strategist),
            ToInteger(CommandTrait::Consummate_Warrior),
            ToInteger(CommandTrait::Astute_Commander),
    };

    constexpr std::array<int, 4> g_commandTraitsHurakan = {
            ToInteger(CommandTrait::None),
            ToInteger(CommandTrait::Grand_Windrider),
            ToInteger(CommandTrait::Swift),
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

    constexpr std::array<int, 4> g_artefactsVanari = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Syari_Pommel),
            ToInteger(Artefact::Senlui_Amulet),
            ToInteger(Artefact::Sun_Stone)
    };

    constexpr std::array<int, 4> g_artefactsHurakan = {
            ToInteger(Artefact::None),
            ToInteger(Artefact::Windblast_Fan),
            ToInteger(Artefact::Wind_Stone),
            ToInteger(Artefact::Buffeting_Aspiragillum)
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

    constexpr std::array<int, 7> g_loreOfWinds = {
            ToInteger(Lore::None),
            ToInteger(Lore::Freezing_Squall),
            ToInteger(Lore::Howling_Gale),
            ToInteger(Lore::Guiding_Flurries),
            ToInteger(Lore::Calming_Zephyr),
            ToInteger(Lore::Burning_Simoom),
            ToInteger(Lore::Transporting_Vortex),
    };

    constexpr std::array<int, 19> g_loreTeclis = {
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
            ToInteger(Lore::Freezing_Squall),
            ToInteger(Lore::Howling_Gale),
            ToInteger(Lore::Guiding_Flurries),
            ToInteger(Lore::Calming_Zephyr),
            ToInteger(Lore::Burning_Simoom),
            ToInteger(Lore::Transporting_Vortex),
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

