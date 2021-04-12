/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusOldblood.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 32;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 110;

    bool SaurusOldblood::s_registered = false;

    SaurusOldblood::SaurusOldblood(WayOfTheSeraphon way, Constellation constellation, WeaponOption option, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SeraphonBase("Saurus Oldblood", 5, g_wounds, 8, 3, false) {
        m_keywords = {ORDER, SERAPHON, SAURUS, HERO, OLDBLOOD};
        m_weapons = {&m_maul, &m_warblade, &m_warspear, &m_greatblade, &m_jaws};
        m_battleFieldRole = Role::Leader;

        setWayOfTheSeraphon(way, constellation);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        if (option == Celestite_Maul) {
            model->addMeleeWeapon(&m_maul);
        } else if (option == Celestite_Warblade) {
            model->addMeleeWeapon(&m_warblade);
        } else if (option == Celestite_Warspear) {
            model->addMeleeWeapon(&m_warspear);
        } else if (option == Celestite_Greatblade) {
            model->addMeleeWeapon(&m_greatblade);
        }
        model->addMeleeWeapon(&m_jaws);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<BuffModifierCommandAbility>(this, "Wrath of the Seraphon", 18, 18, Phase::Combat,
                                                                                  Attribute::To_Hit_Melee, 1, Abilities::Target::SelfAndFriendly,
                                                                                  std::vector<Keyword>(SAURUS)));

        m_points = ComputePoints(1);
    }

    Unit *SaurusOldblood::Create(const ParameterList &parameters) {
        auto option = (WeaponOption) GetEnumParam("Weapon", parameters, Celestite_Warblade);
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_saurusCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_celestialRelicsOfTheWarrior[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new SaurusOldblood(way, constellation, option, trait, artefact, general);
    }

    void SaurusOldblood::Init() {
        if (!s_registered) {
            static const std::array<int, 4> weapons = {Celestite_Maul, Celestite_Warblade,
                                                       Celestite_Warspear, Celestite_Greatblade};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Weapon", Celestite_Warblade, weapons),
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation),
                            EnumParameter("Command Trait", g_saurusCommandTrait[0], g_saurusCommandTrait),
                            EnumParameter("Artefact", g_celestialRelicsOfTheWarrior[0], g_celestialRelicsOfTheWarrior),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Saurus Oldblood", factoryMethod);
        }
    }

    std::string SaurusOldblood::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Celestite_Maul) return "Celestite Maul";
            else if (parameter.intValue == Celestite_Warblade) return "Celestite Warblade";
            else if (parameter.intValue == Celestite_Warspear) return "Celestite War-spear";
            else if (parameter.intValue == Celestite_Greatblade) return "Celestite Greatblade";
        }
        return SeraphonBase::ValueToString(parameter);
    }

    int SaurusOldblood::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Celestite Maul") return Celestite_Maul;
        else if (enumString == "Celestite Warblade") return Celestite_Warblade;
        else if (enumString == "Celestite War-spear") return Celestite_Warspear;
        else if (enumString == "Celestite Greatblade") return Celestite_Greatblade;
        return SeraphonBase::EnumStringToInt(enumString);
    }

    int SaurusOldblood::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Cold Ferocity
        if ((unmodifiedHitRoll == 6) &&
            ((weapon->name() == m_warblade.name()) ||
             (weapon->name() == m_warspear.name()) ||
             (weapon->name() == m_greatblade.name()) ||
             (weapon->name() == m_maul.name()))) {
            return 2;
        }
        return SeraphonBase::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int SaurusOldblood::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Seraphon
