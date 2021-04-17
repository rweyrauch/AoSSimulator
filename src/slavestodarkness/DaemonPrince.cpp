/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/DaemonPrince.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 60;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 210;

    bool DaemonPrince::s_registered = false;

    Unit *DaemonPrince::Create(const ParameterList &parameters) {
        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Daemonic_Axe);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        auto general = GetBoolParam("General", parameters, false);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);

        return new DaemonPrince(legion, mark, weapon, trait, artefact, general);
    }

    std::string DaemonPrince::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            switch (parameter.intValue) {
                case Daemonic_Axe:
                    return "Daemonic Axe";
                case Hellforged_Sword:
                    return "Hellforged Sword";
                default:
                    break;
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int DaemonPrince::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Daemonic Axe") return Daemonic_Axe;
        else if (enumString == "Hellforged Sword") return Hellforged_Sword;

        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    int DaemonPrince::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void DaemonPrince::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Daemonic_Axe, Hellforged_Sword};
            static FactoryMethod factoryMethod = {
                    DaemonPrince::Create,
                    DaemonPrince::ValueToString,
                    DaemonPrince::EnumStringToInt,
                    DaemonPrince::ComputePoints,
                    {
                            EnumParameter("Weapon", Daemonic_Axe, weapons),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Daemon Prince", factoryMethod);
        }
    }

    DaemonPrince::DaemonPrince(DamnedLegion legion, MarkOfChaos mark, WeaponOption option, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SlavesToDarknessBase("Daemon Prince", 12, g_wounds, 10, 3, true) {
        m_keywords = {CHAOS, DAEMON, SLAVES_TO_DARKNESS, HERO, MARK_OF_CHAOS, DAEMON_PRINCE};
        m_weapons = {&m_axe, &m_sword, &m_talons};
        m_battleFieldRole = Role::Leader;

        setDamnedLegion(legion);
        setMarkOfChaos(mark);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        // Immortal Champion
        buffAbility(Ability::Fights_First, 1, {GamePhase::Hero, DurationRestOfGame, owningPlayer()});

        auto model = new Model(g_basesize, wounds());

        if (option == Daemonic_Axe)
            model->addMeleeWeapon(&m_axe);
        else if (option == Hellforged_Sword)
            model->addMeleeWeapon(&m_sword);
        model->addMeleeWeapon(&m_talons);
        addModel(model);

        switch (mark) {
            case MarkOfChaos::Khorne:
                break;
            case MarkOfChaos::Tzeentch:
                m_commandAbilities.push_back(std::make_unique<BuffModifierCommandAbility>(this, "Arcane Influence", 12, 12, GamePhase::Hero,
                                                                                          Attribute::Casting_Roll, 1, Abilities::Target::Friendly,
                                                                                          std::vector<Keyword>{SLAVES_TO_DARKNESS, WIZARD}));
                break;
            case MarkOfChaos::Nurgle:
                break;
            case MarkOfChaos::Slaanesh:
                break;
            default:
                break;
        }
        m_points = g_pointsPerUnit;
    }

    int DaemonPrince::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = SlavesToDarknessBase::toHitModifier(weapon, target);

        // Bounding Charge
        if (m_charged) mod++;

        return mod;
    }

    Wounds DaemonPrince::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Hellforged Sword
        if ((weapon->name() == m_sword.name()) && (hitRoll == 6)) {
            return {0, 2};
        }
        return SlavesToDarknessBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

} // namespace SlavesToDarkness


