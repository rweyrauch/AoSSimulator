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

    KnightAzyros::KnightAzyros(Stormhost stormhost, CommandTrait trait, Artefact artefact, bool isGeneral) :
            StormcastEternal(stormhost, "Knight-Azyros", 12, g_wounds, 9, 3, true),
            m_starblade(Weapon::Type::Melee, "Starblade", 1, 4, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, KNIGHT_AZYROS};
        m_weapons = {&m_starblade};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        s_globalToHitReroll.connect(this, &KnightAzyros::illuminatorOfTheLostReroll, &m_illuminatorSlot);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_starblade);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    KnightAzyros::~KnightAzyros() {
        m_illuminatorSlot.disconnect();
    }

    Unit *KnightAzyros::Create(const ParameterList &parameters) {
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefactsLights[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new KnightAzyros(stormhost, trait, artefact, general);
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
                            EnumParameter("Artefact", g_artefactsLights[0], g_artefactsLights),
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
                        ip->applyDamage({0, Dice::RollD6()}, this);
                    } else {
                        ip->applyDamage({0, Dice::RollD3()}, this);
                    }
                }
                m_usedLightOfSigmar = true;
            }
        }
        StormcastEternal::onStartHero(player);
    }

    Rerolls
    KnightAzyros::illuminatorOfTheLostReroll(const Unit *attacker, const Weapon * /*weapon*/, const Unit *target) {
        if (isFriendly(attacker) && distanceTo(target) <= 10.0) return Rerolls::Ones;
        return Rerolls::None;
    }

    int KnightAzyros::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }


} // namespace StormcastEternals