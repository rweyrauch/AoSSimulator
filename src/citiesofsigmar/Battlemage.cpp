/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "citiesofsigmar/Battlemage.h"

namespace CitiesOfSigmar {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 90;

    bool Battlemage::s_registered = false;

    Unit *Battlemage::Create(const ParameterList &parameters) {
        auto unit = new Battlemage();

        auto realm = (Realm) GetEnumParam("Realm", parameters, Azyr);

        auto city = (City) GetEnumParam("City", parameters,Hammerhal);
        unit->setCity(city);

        bool ok = unit->configure(realm);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string Battlemage::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Battlemage::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Battlemage::Init() {
        if (!s_registered) {
            static const std::array<int, 8> realm = {Aqshy,Azyr,
                                                     Chamon,Ghur,Ghyran,Hysh,
                                                     Shyish,Ulgu};
            static FactoryMethod factoryMethod = {
                    Battlemage::Create,
                    Battlemage::ValueToString,
                    Battlemage::EnumStringToInt,
                    Battlemage::ComputePoints,
                    {
                            EnumParameter("Realm", Azyr, realm),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Battlemage", factoryMethod);
        }
    }

    Battlemage::Battlemage() :
            CitizenOfSigmar("Battlemage", 5, WOUNDS, 6, 6, false),
            m_staff(Weapon::Type::Melee, "Wizard's Staff", 2, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, COLLEGIATE_ARCANE, HERO, WIZARD, BATTLEMAGE};
        m_weapons = {&m_staff};
        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool Battlemage::configure(Realm realm) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_realm = realm;
        m_points = POINTS_PER_UNIT;

        return true;
    }

    int Battlemage::castingModifier() const {
        auto mod = Unit::castingModifier();

        // Magic of the Realms
        if (m_realm == Board::Instance()->getRealm()) mod++;

        return mod;
    }

    int Battlemage::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

}//namespace CitiesOfSigmar