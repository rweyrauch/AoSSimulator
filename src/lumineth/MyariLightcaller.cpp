/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/MyariLightcaller.h>
#include "LuminethPrivate.h"
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace LuminethRealmLords {

    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool MyariLigthcaller::s_registered = false;

    Unit *MyariLigthcaller::Create(const ParameterList &parameters) {
        auto unit = new MyariLigthcaller();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->setNation(GreatNation::Ymetrica);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int MyariLigthcaller::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void MyariLigthcaller::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General"),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Myari Ligthcaller", factoryMethod);
        }
    }

    MyariLigthcaller::MyariLigthcaller() :
            LuminethBase("Myari Ligthcaller", 6, g_wounds, 8, 5, false),
            m_beams(Weapon::Type::Missile, "Searing Beams", 18, 3, 3, 3, -2, 1),
            m_staff(Weapon::Type::Melee, "Staff of Enlightenment", 1, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, SCINARI, YMETRICA, HERO, WIZARD, MYARI_LIGHTCALLER};
        m_weapons = {&m_beams, &m_staff};
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool MyariLigthcaller::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_beams);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);

        return true;
    }

} // namespace LuminethRealmLords