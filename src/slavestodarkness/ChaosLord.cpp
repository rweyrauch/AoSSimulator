/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosLord.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 40;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 110;

    bool ChaosLord::s_registered = false;

    Unit *ChaosLord::Create(const ParameterList &parameters) {
        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Reaperblade);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        auto general = GetBoolParam("General", parameters, false);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);

        return new ChaosLord(legion, mark, weapon, trait, artefact, general);
    }

    void ChaosLord::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Reaperblade, Daemonbound_Steel, Daemonbound_War_Flail};
            static FactoryMethod factoryMethod = {
                    ChaosLord::Create,
                    ChaosLord::ValueToString,
                    ChaosLord::EnumStringToInt,
                    ChaosLord::ComputePoints,
                    {
                            EnumParameter("Weapon", Reaperblade, weapons),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Lord", factoryMethod);
        }
    }

    ChaosLord::ChaosLord(DamnedLegion legion, MarkOfChaos mark, WeaponOption option, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SlavesToDarknessBase(legion, "Chaos Lord", 5, g_wounds, 8, 4, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, HERO, CHAOS_LORD};
        m_weapons = {&m_blade, &m_steel, &m_flail};
        m_battleFieldRole = Role::Leader;

        setMarkOfChaos(mark);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());

        if (option == Reaperblade)
            model->addMeleeWeapon(&m_blade);
        else if (option == Daemonbound_Steel)
            model->addMeleeWeapon(&m_steel);
        else if (option == Daemonbound_War_Flail)
            model->addMeleeWeapon(&m_flail);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Wounds ChaosLord::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = SlavesToDarknessBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
        // Daemonbound
        if ((hitRoll == 6) && ((weapon->name() == m_steel.name()) || (weapon->name() == m_flail.name()))) {
            damage.mortal++;
        }
        return damage;
    }

    std::string ChaosLord::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            switch (parameter.intValue) {
                case Reaperblade:
                    return "Reaperblade";
                case Daemonbound_Steel:
                    return "Daemonbound Steel";
                case Daemonbound_War_Flail:
                    return "Daemonbound War-flail";
                default:
                    break;
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int ChaosLord::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Reaperblade") return Reaperblade;
        else if (enumString == "Daemonbound Steel") return Daemonbound_Steel;
        else if (enumString == "Daemonbound War-flail") return Daemonbound_War_Flail;

        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    int ChaosLord::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace SlavesToDarkness
