/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "nurgle/FeculaFlyblown.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 40;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 100;

    bool FeculaFlyblown::s_registered = false;

    Unit *Nurgle::FeculaFlyblown::Create(const ParameterList &parameters) {
        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        auto general = GetBoolParam("General", parameters, false);
        return new FeculaFlyblown(legion, general);
    }

    void Nurgle::FeculaFlyblown::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FeculaFlyblown::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    FeculaFlyblown::ComputePoints,
                    {
                            BoolParameter("General"),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Fecula Flyblown", factoryMethod);
        }
    }

    Nurgle::FeculaFlyblown::FeculaFlyblown(PlagueLegion legion, bool isGeneral) :
            NurgleBase(legion,"Fecula Flyblown", 4, g_wounds, 8, 4, false, g_pointsPerUnit),
            m_staff(Weapon::Type::Melee, "Rotwood Staff", 2, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, BLESSED_SONS, HERO, WIZARD, SORCERER, FECULA_FLYBLOWN};
        m_weapons = {&m_staff};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    int FeculaFlyblown::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }


} // namespace Nurgle
