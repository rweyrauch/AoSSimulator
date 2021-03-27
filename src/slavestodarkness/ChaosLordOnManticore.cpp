/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosLordOnManticore.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 280;

    struct TableEntry {
        int m_move;
        int m_fangsToWound;
        int m_tailAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 1, 5},
                    {10, 2, 4},
                    {8,  3, 3},
                    {6,  4, 2},
                    {6,  5, 1}
            };

    bool ChaosLordOnManticore::s_registered = false;

    Unit *ChaosLordOnManticore::Create(const ParameterList &parameters) {
        auto unit = new ChaosLordOnManticore();

        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Blade_And_Shield);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        unit->setMarkOfChaos(mark);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        unit->configure(weapon);
        return unit;
    }

    void ChaosLordOnManticore::Init() {
        if (!s_registered) {
            static const std::array<int, 6> weapons = {Blade_And_Lance, Flail_And_Lance, Blade_And_Shield,
                                                       Flail_And_Shield,
                                                       Blade_And_Daggerfist, Flail_And_Daggerfist};
            static FactoryMethod factoryMethod = {
                    ChaosLordOnManticore::Create,
                    ChaosLordOnManticore::ValueToString,
                    ChaosLordOnManticore::EnumStringToInt,
                    ChaosLordOnManticore::ComputePoints,
                    {
                            EnumParameter("Weapon", Blade_And_Shield, weapons),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Lord On Manticore", factoryMethod);
        }
    }

    ChaosLordOnManticore::ChaosLordOnManticore() :
            SlavesToDarknessBase("Chaos Lord On Manticore", 12, g_wounds, 8, 4, true),
            m_blade(Weapon::Type::Melee, "Daemon Blade", 1, 3, 3, 3, -1, RAND_D3),
            m_lance(Weapon::Type::Melee, "Chaos Lance", 2, 3, 3, 3, 0, 2),
            m_flail(Weapon::Type::Melee, "Chaos Flail", 2, 6, 3, 3, 0, 1),
            m_fangsAndClaws(Weapon::Type::Melee, "Honed Fangs and Claws", 1, 5, 3, 1, -1, 2),
            m_tail(Weapon::Type::Melee, "Shredding Tail", 3, 5, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, MANTICORE, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, MONSTER, HERO,
                      CHAOS_LORD};
        m_hasMount = true;
        m_fangsAndClaws.setMount(true);
        m_tail.setMount(true);
        m_weapons = {&m_blade, &m_lance, &m_flail, &m_fangsAndClaws, &m_tail};
        m_battleFieldRole = Role::Leader_Behemoth;
    }

    void ChaosLordOnManticore::configure(WeaponOption weapon) {
        m_weapon = weapon;

        auto model = new Model(g_basesize, wounds());

        switch (weapon) {
            case WeaponOption::Blade_And_Lance:
                model->addMeleeWeapon(&m_blade);
                model->addMeleeWeapon(&m_lance);
                break;
            case WeaponOption::Blade_And_Shield:
            case WeaponOption::Blade_And_Daggerfist:
                model->addMeleeWeapon(&m_blade);
                break;
            case WeaponOption::Flail_And_Lance:
                model->addMeleeWeapon(&m_flail);
                model->addMeleeWeapon(&m_lance);
                break;
            case WeaponOption::Flail_And_Shield:
            case WeaponOption::Flail_And_Daggerfist:
                model->addMeleeWeapon(&m_flail);
                break;
        }

        model->addMeleeWeapon(&m_fangsAndClaws);
        model->addMeleeWeapon(&m_tail);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    void ChaosLordOnManticore::onRestore() {
        SlavesToDarknessBase::onRestore();
        // Reset table-drive attributes
        onWounded();
    }

    void ChaosLordOnManticore::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_fangsAndClaws.setToWound(g_damageTable[damageIndex].m_fangsToWound);
        m_tail.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    size_t ChaosLordOnManticore::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    std::string ChaosLordOnManticore::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            switch (parameter.intValue) {
                case Blade_And_Lance:
                    return "Daemon Blade and Chaos Lance";
                case Flail_And_Lance:
                    return "Chaos Flail and Chaos Lance";
                case Blade_And_Shield:
                    return "Daemon Blade and Chaos Runeshield";
                case Flail_And_Shield:
                    return "Chaos Flail and Chaos Runeshield";
                case Blade_And_Daggerfist:
                    return "Daemon Blade and Daggerfist";
                case Flail_And_Daggerfist:
                    return "Chaos Flail and Daggerfist";
                default:
                    break;
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int ChaosLordOnManticore::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Daemon Blade and Chaos Lance") return Blade_And_Lance;
        else if (enumString == "Chaos Flail and Chaos Lance") return Flail_And_Lance;
        else if (enumString == "Daemon Blade and Chaos Runeshield") return Blade_And_Shield;
        else if (enumString == "Chaos Flail and Chaos Runeshield") return Flail_And_Shield;
        else if (enumString == "Daemon Blade and Daggerfist") return Blade_And_Daggerfist;
        else if (enumString == "Chaos Flail and Daggerfist") return Flail_And_Daggerfist;

        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    Wounds ChaosLordOnManticore::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto savedWounds = SlavesToDarknessBase::applyWoundSave(wounds, attackingUnit);
        Dice::RollResult result;
        // Chaos Runeshield
        Dice::RollD6(savedWounds.mortal, result);
        savedWounds.mortal -= result.rollsGE(5);
        return savedWounds;
    }

    Wounds
    ChaosLordOnManticore::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = SlavesToDarknessBase::weaponDamage(weapon, target, hitRoll, woundRoll);
        // Daemonbound
        if ((hitRoll == 6) && (weapon->name() == m_blade.name())) {
            damage.mortal++;
        }
            // Chaos Lance
        else if (m_charged && (weapon->name() == m_lance.name())) {
            damage.normal++;
        }
        return damage;
    }

    Wounds ChaosLordOnManticore::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto damage = SlavesToDarknessBase::computeReturnedDamage(weapon, saveRoll);

        // Daggerfist
        if ((saveRoll == 6) && (!weapon->isMissile()) &&
            (m_weapon == WeaponOption::Flail_And_Daggerfist || m_weapon == WeaponOption::Blade_And_Daggerfist)) {
            damage.mortal++;
        }
        return damage;
    }

    int ChaosLordOnManticore::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto rend = SlavesToDarknessBase::weaponRend(weapon, target, hitRoll, woundRoll);

        // Chaos Lance
        if (m_charged && (weapon->name() == m_lance.name())) {
            rend -= 2;
        }
        return rend;
    }

    Rerolls ChaosLordOnManticore::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Territorial Predator
        if ((weapon->name() == m_fangsAndClaws.name()) && (target->hasKeyword(MONSTER))) {
            return Rerolls::Failed;
        }
        return SlavesToDarknessBase::toHitRerolls(weapon, target);
    }

    int ChaosLordOnManticore::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace SlavesToDarkness
