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
    static const int g_basesize = 40;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 140 * 4;

    bool Wrathmongers::s_registered = false;

    Wrathmongers::Wrathmongers(SlaughterHost host, int numModels, int points) :
            KhorneBase("Wrathmongers", 5, g_wounds, 7, 5, false, points) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, WRATHMONGERS};
        m_weapons = {&m_wrathflails, &m_wrathflailsMaster};

        s_globalAttackMod.connect(this, &Wrathmongers::crimsonHaze, &m_hazeSlot);

        setSlaughterHost(host);

        auto master = new Model(g_basesize, wounds());
        master->addMeleeWeapon(&m_wrathflailsMaster);
        addModel(master);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_wrathflails);
            addModel(model);
        }
    }

    Wrathmongers::~Wrathmongers() {
        m_hazeSlot.disconnect();
    }

    Unit *Wrathmongers::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);

        return new Wrathmongers(host, numModels, ComputePoints(parameters));
    }

    void Wrathmongers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Wrathmongers::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Wrathmongers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
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

    int Wrathmongers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int
    Wrathmongers::crimsonHaze(const Unit *attacker, const Model * /*attackingModel*/, const Weapon * /*weapon*/,
                              const Unit *target) {
        // Crimson Haze
        if (!attacker->hasKeyword(WRATHMONGERS) && (distanceTo(target) <= 8.0)) {
            return 1;
        }
        return 0;
    }

    void Wrathmongers::onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) {
        KhorneBase::onFriendlyModelSlain(numSlain, attacker, source);

        // Bloodfury
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        auto mod = 0;
        if (units.size() >= 2) mod = 1;

        for (auto unit : units) {
            int roll = Dice::RollD6() + mod;
            if (roll >= 2 && roll <= 5) {
                unit->applyDamage({0, 1, Wounds::Source::Ability, nullptr}, this);
            } else if (roll >= 6) {
                unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Ability, nullptr}, this);
            }
        }
    }

} //namespace Khorne