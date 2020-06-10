/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <idonethdeepkin/Volturnos.h>
#include <UnitFactory.h>
#include <Board.h>

namespace IdonethDeepkin {
    static const int BASESIZE = 60;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 280;

    bool Volturnos::s_registered = false;

    Volturnos::Volturnos() :
            IdonethDeepkinBase("Volturnos", 14, WOUNDS, 8, 3, true),
            m_theAstraSolus(Weapon::Type::Melee, "The Astra Solus", 1, 5, 3, 3, -1, RAND_D3),
            m_deepmareJawsTalons(Weapon::Type::Melee, "Deepmare's Fanged Jaw and Talons", 2, 3, 3, 3, -1, 1),
            m_deepmareTails(Weapon::Type::Melee, "Deepmare's Lashing Tails", 2, 3, 3, 3, 0, 2) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, HERO, AKHELIAN, AKHELIAN_KING, VOLTURNOS};
        m_weapons = {&m_theAstraSolus, &m_deepmareJawsTalons, &m_deepmareTails};

        s_globalBraveryMod.connect(this, &Volturnos::crestOfTheHighKings, &m_connection);
    }

    Volturnos::~Volturnos() {
        m_connection.disconnect();
    }

    bool Volturnos::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_theAstraSolus);
        model->addMeleeWeapon(&m_deepmareJawsTalons);
        model->addMeleeWeapon(&m_deepmareTails);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *Volturnos::Create(const ParameterList &parameters) {
        auto unit = new Volturnos();

        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, Enclave::Custom);
        unit->setEnclave(enclave);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Volturnos::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    IdonethDeepkinBase::ValueToString,
                    IdonethDeepkinBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Enclave", g_enclave[0], g_enclave),
                    },
                    ORDER,
                    {IDONETH_DEEPKIN}
            };

            s_registered = UnitFactory::Register("Volturnos", factoryMethod);
        }
    }

    void Volturnos::onCharged() {
        // Deepmare Horn
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        if (!units.empty()) {
            int roll = Dice::rollD6();
            if (roll >= 2) {
                units.front()->applyDamage({0, Dice::rollD3()});
            }
        }

        Unit::onCharged();
    }

    int Volturnos::crestOfTheHighKings(const Unit *target) {
        // The Crest of the High Kings
        if (target->hasKeyword(IDONETH_DEEPKIN) && (target->owningPlayer() == owningPlayer()) &&
            (distanceTo(target) <= 18.0)) {
            return 1;
        }

        return 0;
    }

    int Volturnos::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // The Astra Solus
        if ((hitRoll >= 6) && (weapon->name() == m_theAstraSolus.name())) {
            return -5;
        }
        return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
    }

    int Volturnos::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace IdonethDeepkin
