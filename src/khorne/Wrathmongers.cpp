/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <khorne/Wrathmongers.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 140 * 4;

    bool Wrathmongers::s_registered = false;

    Wrathmongers::Wrathmongers() :
            KhorneBase("Wrathmongers", 5, WOUNDS, 7, 5, false),
            m_wrathflails(Weapon::Type::Melee, "Wrath-flails", 2, 4, 4, 3, -1, 1),
            m_wrathflailsMaster(Weapon::Type::Melee, "Wrath-flails", 2, 5, 4, 3, -1, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, WRATHMONGERS};
        m_weapons = {&m_wrathflails, &m_wrathflailsMaster};

        s_globalAttackMod.connect(this, &Wrathmongers::crimsonHaze, &m_hazeSlot);
    }

    Wrathmongers::~Wrathmongers() {
        m_hazeSlot.disconnect();
    }

    bool Wrathmongers::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto master = new Model(BASESIZE, wounds());
        master->addMeleeWeapon(&m_wrathflailsMaster);
        addModel(master);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_wrathflails);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Wrathmongers::Create(const ParameterList &parameters) {
        auto unit = new Wrathmongers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Wrathmongers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Wrathmongers::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Wrathmongers::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Wrathmongers", factoryMethod);
        }
    }

    int Wrathmongers::toHitModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = KhorneBase::toHitModifier(weapon, target);

        // Furious Assault
        if (m_charged) {
            modifier += 1;
        }
        return modifier;
    }

    int Wrathmongers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int
    Wrathmongers::crimsonHaze(const Unit * /*attacker*/, const Model * /*attackingModel*/, const Weapon * /*weapon*/,
                              const Unit *target) {
        // Crimson Haze
        if (distanceTo(target) <= 8.0) {
            // TODO: Does not apply for attacking WRATHMONGER models
            return 1;
        }
        return 0;
    }

    void Wrathmongers::onModelSlain(Wounds::Source source) {
        Unit::onModelSlain(source);

        // Bloodfury
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        auto mod = 0;
        if (units.size() >= 2) mod = 1;

        for (auto unit : units) {
            int roll = Dice::rollD6() + mod;
            if (roll >= 2 && roll <= 5) {
                unit->applyDamage({0, 1});
            } else if (roll >= 6) {
                unit->applyDamage({0, Dice::rollD3()});
            }
        }
    }

} //namespace Khorne