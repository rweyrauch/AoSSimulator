/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightAzyros.h>
#include <iostream>
#include <Board.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool KnightAzyros::s_registered = false;

    KnightAzyros::KnightAzyros() :
            StormcastEternal("Knight-Azyros", 12, g_wounds, 9, 3, true),
            m_starblade(Weapon::Type::Melee, "Starblade", 1, 4, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, KNIGHT_AZYROS};
        m_weapons = {&m_starblade};
        m_battleFieldRole = Leader;

        s_globalToHitReroll.connect(this, &KnightAzyros::illuminatorOfTheLostReroll, &m_illuminatorSlot);
    }

    KnightAzyros::~KnightAzyros() {
        m_illuminatorSlot.disconnect();
    }

    bool KnightAzyros::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_starblade);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *KnightAzyros::Create(const ParameterList &parameters) {
        auto unit = new KnightAzyros();

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void KnightAzyros::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Knight-Azyros", factoryMethod);
        }
    }

    void KnightAzyros::onStartHero(PlayerId player) {
        if ((player == owningPlayer()) && !m_usedLightOfSigmar) {
            // The Light of Sigmar - use the first chance is can
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 8.0);
            if (!units.empty()) {
                for (auto ip : units) {
                    if (ip->hasKeyword(CHAOS)) {
                        ip->applyDamage({0, Dice::RollD6()});
                    } else {
                        ip->applyDamage({0, Dice::RollD3()});
                    }
                }
                m_usedLightOfSigmar = true;
            }
        }
        StormcastEternal::onStartHero(player);
    }

    Rerolls
    KnightAzyros::illuminatorOfTheLostReroll(const Unit *attacker, const Weapon * /*weapon*/, const Unit *target) {
        if (isFriendly(attacker) && distanceTo(target) <= 10.0) return Reroll_Ones;
        return No_Rerolls;
    }

    int KnightAzyros::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }


} // namespace StormcastEternals