/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/MyarisPurifiers.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace LuminethRealmLords {

    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_pointsPerUnit = 100;

    bool MyarisPurifiers::s_registered = false;

    Unit *MyarisPurifiers::Create(const ParameterList &parameters) {
        return new MyarisPurifiers();
    }

    int MyarisPurifiers::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void MyarisPurifiers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Myari's Purifiers", factoryMethod);
        }
    }

    MyarisPurifiers::MyarisPurifiers() :
            LuminethBase(GreatNation::Ymetrica, "Myari's Purifiers", 6, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_bow(Weapon::Type::Missile, "Auralan Bow", 18, 2, 3, 4, -1, 1),
            m_mallet(Weapon::Type::Melee, "Stone Mallet", 1, 3, 3, 3, -1, 1),
            m_greatsword(Weapon::Type::Melee, "Sunmetal Greatsword", 1, 2, 2, 2, -1, 1),
            m_dagger(Weapon::Type::Melee, "Vanari Dagger", 1, 1, 3, 4, 0, 1) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, YMETRICA, MYARIS_PURIFIERS, Sunmetal_Weapons};
        m_weapons = {&m_bow, &m_mallet, &m_greatsword, &m_dagger};

        auto ailenn = new Model(g_basesize, wounds());
        ailenn->addMeleeWeapon(&m_greatsword);
        addModel(ailenn);

        auto bahannar = new Model(g_basesize, wounds());
        bahannar->addMeleeWeapon(&m_mallet);
        addModel(bahannar);

        auto senaela = new Model(g_basesize, wounds());
        senaela->addMeleeWeapon(&m_bow);
        senaela->addMeleeWeapon(&m_dagger);
        addModel(senaela);
    }

    Wounds MyarisPurifiers::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Sunmetal weapons
        if ((hitRoll >= 6) && ((weapon->name() == m_greatsword.name()) || (weapon->name() == m_bow.name()))) {
            return {0, 1};
        }

        // Crushing Blow
        if ((hitRoll >= 6) && (weapon->name() == m_mallet.name())) {
            return {weapon->damage() + 1, 0};
        }

        return LuminethBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

} // namespace LuminethRealmLords
