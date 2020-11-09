/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "slavestodarkness/ChaosLordOnKarkadrak.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 9;
    static const int g_pointsPerUnit = 230;

    bool ChaosLordOnKarkadrak::s_registered = false;

    Unit *ChaosLordOnKarkadrak::Create(const ParameterList &parameters) {
        auto unit = new ChaosLordOnKarkadrak();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        unit->setMarkOfChaos(mark);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ChaosLordOnKarkadrak::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ChaosLordOnKarkadrak::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ChaosLordOnKarkadrak::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Lord On Karkadrak", factoryMethod);
        }
    }

    ChaosLordOnKarkadrak::ChaosLordOnKarkadrak() :
            SlavesToDarknessBase("Chaos Lord On Karkadrak", 9, g_wounds, 8, 3, false),
            m_battleAxe(Weapon::Type::Melee, "Hexed Battle-axe", 1, 5, 3, 3, 0, 2),
            m_blade(Weapon::Type::Melee, "Daemonbound Blade", 1, 3, 3, 3, -1, RAND_D3),
            m_hornsAndClaws(Weapon::Type::Melee, "Tearing Horn and Claws", 1, 3, 4, 3, -1, 2),
            m_tail(Weapon::Type::Melee, "Battering Tail", 1, 2, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, HERO, CHAOS_LORD};
        m_weapons = {&m_battleAxe, &m_blade, &m_hornsAndClaws, &m_tail};
        m_battleFieldRole = Leader;
        m_hasMount = true;
    }

    bool ChaosLordOnKarkadrak::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_battleAxe);
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_hornsAndClaws);
        model->addMeleeWeapon(&m_tail);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    void ChaosLordOnKarkadrak::onCharged() {
        Unit::onCharged();

        // Brutish Rampage
        if (m_charged) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
            for (auto unit : units) {
                if (Dice::RollD6() >= 2) {
                    unit->applyDamage({0, Dice::RollD3()});
                }
            }
        }
    }

    Wounds
    ChaosLordOnKarkadrak::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
        // Daemonbound
        if ((hitRoll == 6) && (weapon->name() == m_blade.name())) {
            damage.mortal++;
        }
        return damage;
    }

    Wounds ChaosLordOnKarkadrak::applyWoundSave(const Wounds &wounds) {
        auto savedWounds = Unit::applyWoundSave(wounds);
        Dice::RollResult result;
        // Rune-etched Plating
        Dice::RollD6(savedWounds.mortal, result);
        savedWounds.mortal -= result.rollsGE(5);
        return savedWounds;
    }

    int ChaosLordOnKarkadrak::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace SlavesToDarkness
