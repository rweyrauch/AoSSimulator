/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/IronskullsBoyz.h>
#include <UnitFactory.h>
#include "IronjawzPrivate.h"

namespace Ironjawz {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_pointsPerUnit = 80;

    bool IronskullsBoyz::s_registered = false;

    IronskullsBoyz::IronskullsBoyz(Warclan warclan) :
            Ironjawz(warclan, "Ironskull's Boyz", 4, g_wounds, 6, 4, false, g_pointsPerUnit),
            m_eadButt(Weapon::Type::Melee, "'Eadbutt'", 1, 1, 4, 3, 0, RAND_D3),
            m_pairedChoppas(Weapon::Type::Melee, "Pair of Ardboy Choppas", 1, 2, 3, 3, -1, 1),
            m_bigChoppa(Weapon::Type::Melee, "Ardboy Big Choppa", 1, 2, 4, 3, -1, 2) {
        m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, IRONSUNZ, ARDBOYS, IRONSKULLS_BOYZ};
        m_weapons = {&m_eadButt, &m_pairedChoppas, &m_bigChoppa};

        // Add the Boss
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_eadButt);
        bossModel->addMeleeWeapon(&m_bigChoppa);
        addModel(bossModel);

        for (auto i = 1; i < 4; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_eadButt);
            model->addMeleeWeapon(&m_pairedChoppas);
            addModel(model);
        }
    }

    Unit *IronskullsBoyz::Create(const ParameterList &parameters) {
        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        return new IronskullsBoyz(warclan);
    }

    void IronskullsBoyz::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    IronskullsBoyz::Create,
                    Ironjawz::ValueToString,
                    Ironjawz::EnumStringToInt,
                    IronskullsBoyz::ComputePoints,
                    {
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Ironskull's Boyz", factoryMethod);
        }
    }

    Wounds IronskullsBoyz::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Dead 'Ard
        Dice::RollResult result;
        Dice::RollD6(wounds.normal, result);
        Wounds modWounds = wounds;
        modWounds.normal -= result.numUnmodified6s();
        return modWounds;
    }

    int IronskullsBoyz::toHitModifier(const Weapon *weapon, const Unit *target) const {
        // Paired Choppas
        if (weapon->name() == m_pairedChoppas.name()) {
            return 1;
        }
        return Unit::toHitModifier(weapon, target);
    }

    int IronskullsBoyz::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }


} // namespace Ironjawz
