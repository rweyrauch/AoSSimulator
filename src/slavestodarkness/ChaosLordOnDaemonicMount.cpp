/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosLordOnDaemonicMount.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 170;

    bool ChaosLordOnDaemonicMount::s_registered = false;

    Unit *ChaosLordOnDaemonicMount::Create(const ParameterList &parameters) {
        auto unit = new ChaosLordOnDaemonicMount();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        unit->setMarkOfChaos(mark);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ChaosLordOnDaemonicMount::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ChaosLordOnDaemonicMount::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ChaosLordOnDaemonicMount::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Lord On Daemonic Mount", factoryMethod);
        }
    }

    ChaosLordOnDaemonicMount::ChaosLordOnDaemonicMount() :
            SlavesToDarknessBase("Chaos Lord On Daemonic Mount", 10, WOUNDS, 8, 4, false),
            m_hammer(Weapon::Type::Melee, "Cursed Warhammer", 1, 4, 3, 3, -1, 2),
            m_hooves(Weapon::Type::Melee, "Mighty Hooves", 1, 3, 4, 3, 0, 1) {
        m_keywords = {CHAOS, DAEMON, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, HERO, CHAOS_LORD};
        m_weapons = {&m_hammer, &m_hooves};
        m_hasMount = true;
    }

    bool ChaosLordOnDaemonicMount::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_hammer);
        model->addMeleeWeapon(&m_hooves);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Wounds ChaosLordOnDaemonicMount::applyWoundSave(const Wounds &wounds) {
        auto savedWounds = Unit::applyWoundSave(wounds);
        Dice::RollResult result;
        // Chaos Runeshield
        Dice::rollD6(savedWounds.mortal, result);
        savedWounds.mortal -= result.rollsGE(5);
        return savedWounds;
    }

    Wounds
    ChaosLordOnDaemonicMount::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
        // Cursed Warhammer
        if ((hitRoll == 6) && (weapon->name() == m_hooves.name())) {
            damage.mortal = 2;
            damage.normal = 0;
        }
        return damage;
    }

    int ChaosLordOnDaemonicMount::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace SlavesToDarkness
