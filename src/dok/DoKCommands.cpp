/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "DoKCommands.h"

namespace DaughtersOfKhaine {

    CommandAbility *CreateSendForthTheCauldrons(Unit *hero) {
        return nullptr;
    }
    CommandAbility *CreateAThousandBladeforms(Unit *hero) {
        return new BuffModifierCommandAbility(hero, "A Thousand Bladeforms", 12, 12, Phase::Combat, Attribute::To_Hit_Melee, 1,
                                              Abilities::Target::Friendly, {WITCH_AELVES, SISTERS_OF_SLAUGHTER});
    }
    CommandAbility *CreateInspiredByCarnage(Unit* hero) {
        return new BuffModifierCommandAbility(hero, "Inspired by Carnage", 12, 12, Phase::Combat, Attribute::To_Wound_Melee, 1,
                                              Abilities::Target::Friendly, {SISTERS_OF_SLAUGHTER});
    }
    CommandAbility *CreateMastersOfTheShadowpaths(Unit* hero) {
        return nullptr;
    }
    CommandAbility *CreateBleedTheMind(Unit* hero) {
        return nullptr;
    }
    CommandAbility *CreatePowerInTheBlood(Unit* hero) {
        return new BuffModifierCommandAbility(hero, "Power in the Blood", 12, 12, Phase::Combat, Attribute::Attacks_Melee, 1,
                                              Abilities::Target::Friendly, {MELUSAI, KHINERAI_HARPIES});
    }

    CommandAbility* CreateCommandAbility(Temple temple, Unit* hero) {
        switch (temple) {
            case Temple::Hagg_Nar:
                return CreateSendForthTheCauldrons(hero);
            case Temple::Draichi_Ganeth:
                return CreateAThousandBladeforms(hero);
            case Temple::The_Kraith:
                return CreateInspiredByCarnage(hero);
            case Temple::Khailebron:
                return CreateMastersOfTheShadowpaths(hero);
            case Temple::Khelt_Nar:
                return CreateBleedTheMind(hero);
            case Temple::Zainthar_Kai:
                return CreatePowerInTheBlood(hero);
            default:
                break;
        }
        return nullptr;
    }

    CommandTraitAbility *CreateCommandTrait(CommandTrait which, Unit* general) {
        switch (which) {
        case CommandTrait::Bathed_In_Blood:
            return nullptr;
        case CommandTrait::Zealous_Orator:
            return nullptr;
        case CommandTrait::Sacrificer_Overseer:
            return nullptr;
        case CommandTrait::Terrifying_Beauty:
            return nullptr;
        case CommandTrait::Master_Of_Poisons:
            return nullptr;
        case CommandTrait::True_Believer:
            return nullptr;
        case CommandTrait::Arcane_Mastery:
            return nullptr;
        case CommandTrait::Writhing_Coils:
            return nullptr;
        case CommandTrait::Fearsome_Presence:
            return nullptr;
        case CommandTrait::Veteran_Of_The_Cathirar_Dhule:
            return nullptr;
        case CommandTrait::Impenentrable_Scales:
            return nullptr;
        case CommandTrait::Fuelled_By_Revenge:
            return nullptr;
        case CommandTrait::Devoted_Disciple:
            return nullptr;
        case CommandTrait::Mistress_Of_Illusion:
            return nullptr;
        case CommandTrait::Curse_Of_The_Bloody_Handed:
            return nullptr;
        case CommandTrait::Bathe_In_Their_Blood:
            return nullptr;
        case CommandTrait::The_Circling_Flock:
            return nullptr;
        case CommandTrait::Victor_Of_Yaithril:
            return nullptr;
            default:
                break;
        }
        return nullptr;
    }

    CommandTraitAbility *CreateCommandTrait(Temple temple, Unit* general) {
        switch (temple) {
            case Temple::Hagg_Nar:
                return CreateCommandTrait(CommandTrait::Devoted_Disciple, general);
            case Temple::Draichi_Ganeth:
                return CreateCommandTrait(CommandTrait::Victor_Of_Yaithril, general);
            case Temple::The_Kraith:
                return CreateCommandTrait(CommandTrait::Bathe_In_Their_Blood, general);
            case Temple::Khailebron:
                return CreateCommandTrait(CommandTrait::Mistress_Of_Illusion, general);
            case Temple::Zainthar_Kai:
                return CreateCommandTrait(CommandTrait::Curse_Of_The_Bloody_Handed, general);
            case Temple::Khelt_Nar:
                return CreateCommandTrait(CommandTrait::The_Circling_Flock, general);
            default:
                break;
        }
        return nullptr;
    }
}
