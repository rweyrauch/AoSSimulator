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
#include "IdonethDeepkinPrivate.h"

namespace IdonethDeepkin {
    static const int g_basesize = 60;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 270;

    bool Volturnos::s_registered = false;

    Volturnos::Volturnos(Enclave enclave, MountTrait trait, bool isGeneral) :
            IdonethDeepkinBase(enclave, "Volturnos", 14, g_wounds, 8, 3, true, g_pointsPerUnit),
            m_theAstraSolus(Weapon::Type::Melee, "The Astra Solus", 1, 5, 3, 3, -1, RAND_D3),
            m_deepmareJawsTalons(Weapon::Type::Melee, "Deepmare's Fanged Jaw and Talons", 2, 3, 3, 3, -1, 1),
            m_deepmareTails(Weapon::Type::Melee, "Deepmare's Lashing Tails", 2, 3, 3, 3, 0, 2) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, HERO, AKHELIAN, AKHELIAN_KING, VOLTURNOS};
        m_weapons = {&m_theAstraSolus, &m_deepmareJawsTalons, &m_deepmareTails};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_deepmareTails.setMount(true);
        m_deepmareJawsTalons.setMount(true);

        setGeneral(isGeneral);

        s_globalBraveryMod.connect(this, &Volturnos::crestOfTheHighKings, &m_connection);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_theAstraSolus);
        model->addMeleeWeapon(&m_deepmareJawsTalons);
        model->addMeleeWeapon(&m_deepmareTails);
        addModel(model);
    }

    Volturnos::~Volturnos() {
        m_connection.disconnect();
    }

    Unit *Volturnos::Create(const ParameterList &parameters) {
        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        auto trait = (MountTrait) GetEnumParam("Mount Trait", parameters, g_leviadonTrait[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Volturnos(enclave, trait, general);
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
                            EnumParameter("Mount Trait", g_leviadonTrait[0], g_leviadonTrait),
                            BoolParameter("General")
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
            int roll = Dice::RollD6();
            if (roll >= 2) {
                units.front()->applyDamage({0, Dice::RollD3()}, this);
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

    int Volturnos::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // The Astra Solus
        if ((hitRoll >= 6) && (weapon->name() == m_theAstraSolus.name())) {
            return -5;
        }
        return Unit::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int Volturnos::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace IdonethDeepkin
