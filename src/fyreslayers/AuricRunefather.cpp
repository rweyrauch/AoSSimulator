/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunefather.h>
#include <Board.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 100;

    bool AuricRunefather::s_registered = false;

    AuricRunefather::AuricRunefather() :
            Fyreslayer("Auric Runefather", 4, WOUNDS, 8, 4, false),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_grandAxe(Weapon::Type::Melee, "Latchkey Grandaxe", 3, 3, 3, 3, -1, 3) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, AURIC_RUNEFATHER};
        m_weapons = {&m_throwingAxe, &m_grandAxe};
    }

    bool AuricRunefather::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMeleeWeapon(&m_grandAxe);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *AuricRunefather::Create(const ParameterList &parameters) {
        auto unit = new AuricRunefather();

        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        unit->setLodge(lodge);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_fatherSonTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_heirloomArtefacts[0]);
        unit->setArtefact(artefact);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AuricRunefather::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AuricRunefather::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    AuricRunefather::ComputePoints,
                    {
                        EnumParameter("Lodge", g_lodge[0], g_lodge),
                        EnumParameter("Command Trait", g_fatherSonTraits[0], g_fatherSonTraits),
                        EnumParameter("Artefact", g_heirloomArtefacts[0], g_heirloomArtefacts),
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Auric Runefather", factoryMethod);
        }
    }

    int AuricRunefather::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void AuricRunefather::onStartHero(PlayerId player) {
        Fyreslayer::onStartHero(player);

        if (owningPlayer() == player) {
            // Stare Down
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) <= 3.0)) {
                unit->buffModifier(Bravery, -Dice::rollD3(), {Hero, m_battleRound + 1, owningPlayer()});
            }
        }
    }

    Wounds AuricRunefather::onEndCombat(PlayerId player) {
        auto wounds = Unit::onEndCombat(player);

        // Weapon-breaker
        auto unit = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), HERO, 3.0);
        if (unit) {
            if (Dice::rollD6() == 6) {
                // TODO: buff only affects a single weapon
                unit->buffModifier(ToHitMelee, -1, {Battleshock, std::numeric_limits<int>::max(), owningPlayer()});
            }
        }
        return wounds;
    }

} // namespace Fyreslayers