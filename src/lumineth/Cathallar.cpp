/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "lumineth/Cathallar.h"
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace LuminethRealmLords {

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    bool ScinariCathallar::s_registered = false;

    Unit *ScinariCathallar::Create(const ParameterList &parameters) {
        auto unit = new ScinariCathallar();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int ScinariCathallar::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void ScinariCathallar::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Scinari Cathallar", factoryMethod);
        }
    }

    ScinariCathallar::ScinariCathallar() :
            LuminethBase("Scinari Cathallar", 6, WOUNDS, 7, 5, false),
            m_touch(Weapon::Type::Melee, "Despairing Touch", 1, 1, 4, 2, 0, RAND_D3) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, SCINARI, HERO, WIZARD, CATHALLAR};
        m_weapons = {&m_touch};
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool ScinariCathallar::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_touch);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);

        return true;
    }

} // namespace LuminethRealmLords