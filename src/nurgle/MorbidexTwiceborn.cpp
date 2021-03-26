/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/MorbidexTwiceborn.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 100;
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 240;

    bool MorbidexTwiceborn::s_registered = false;

    Unit *MorbidexTwiceborn::Create(const ParameterList &parameters) {
        auto unit = new MorbidexTwiceborn();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        unit->setLegion(legion);

        unit->configure();
        return unit;
    }

    void MorbidexTwiceborn::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MorbidexTwiceborn::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    MorbidexTwiceborn::ComputePoints,
                    {
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Morbidex Twiceborn", factoryMethod);
        }
    }

    MorbidexTwiceborn::MorbidexTwiceborn() :
            NurgleBase("Morbidex Twicebornd", 10, g_wounds, 9, 3, false),
            m_tongues(Weapon::Type::Missile, "Slabrous Tongues", 6, 3, 3, 2, -1, 1),
            m_scythe(Weapon::Type::Melee, "Fleshreaper Scythe", 2, 5, 3, 3, -1, 2),
            m_claws(Weapon::Type::Melee, "Monstrous Claws", 3, 5, 4, 2, -1, 1) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, MONSTER, HERO, MORBIDEX_TWICEBORN};
        m_weapons = {&m_tongues, &m_scythe, &m_claws};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_tongues.setMount(true);
        m_claws.setMount(true);
        s_globalToWoundMod.connect(this, &MorbidexTwiceborn::maliciousMitesWoundMod, &m_maliciousMitesSlot);
    }

    MorbidexTwiceborn::~MorbidexTwiceborn() {
        m_maliciousMitesSlot.disconnect();
    }

    bool MorbidexTwiceborn::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_tongues);
        model->addMeleeWeapon(&m_scythe);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    int MorbidexTwiceborn::maliciousMitesWoundMod(const Unit *attacker, const Weapon * /*weapon*/,
                                                  const Unit * /*target*/) {
        if (isFriendly(attacker) && attacker->hasKeyword(NURGLINGS) && (distanceTo(attacker) <= 7.0)) return 1;
        return 0;
    }

    int MorbidexTwiceborn::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle
