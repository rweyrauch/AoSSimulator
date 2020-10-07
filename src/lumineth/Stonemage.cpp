/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "lumineth/Stonemage.h"
#include "LuminethPrivate.h"
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace LuminethRealmLords {

    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 130;

    bool AlarithStonemage::s_registered = false;

    Unit *AlarithStonemage::Create(const ParameterList &parameters) {
        auto unit = new AlarithStonemage();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto nation = (GreatNation)GetEnumParam("Nation", parameters, (int)GreatNation::None);
        unit->setNation(nation);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int AlarithStonemage::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void AlarithStonemage::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General"),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Alarith Stonemage", factoryMethod);
        }
    }

    AlarithStonemage::AlarithStonemage() :
            LuminethBase("Alarith Stonemage", 6, WOUNDS, 8, 5, false),
            m_staff(Weapon::Type::Melee, "Staff of the High Peaks", 3, RAND_D3, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, AELEMENTIRI, ALARITH, HERO, WIZARD, STONEMAGE};
        m_weapons = {&m_staff};
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool AlarithStonemage::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);

        return true;
    }

} // namespace LuminethRealmLords