/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <kruleboyz/Killaboss.h>
#include <UnitFactory.h>

namespace Kruleboyz {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 0;

    bool Killaboss::s_registered = false;

    Killaboss::Killaboss(Warclan warclan, WeaponOption option, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Kruleboyz(warclan, "Killaboss", 5, g_wounds, 7, 4, false, g_pointsPerUnit) {
        m_keywords = {DESTRUCTION, ORRUK_WARCLANS, ORRUK, KRULEBOYZ, HERO, KILLABOSS};
        m_weapons = {&m_hacka, &m_flail, &m_shiv};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_hacka);
        if (option == BossHackaAndRustingFlail) {
            model->addMeleeWeapon(&m_flail);
        }
        model->addMeleeWeapon(&m_shiv);
        addModel(model);
    }

    Unit *Killaboss::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new Killaboss(Warclan::None, BossHackaAndRustingFlail, CommandTrait::None, Artefact::None, general);
    }

    void Killaboss::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Killaboss::Create,
                    Kruleboyz::ValueToString,
                    Kruleboyz::EnumStringToInt,
                    Killaboss::ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Killaboss", factoryMethod);
        }
    }

    int Killaboss::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Kruleboyz