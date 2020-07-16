/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunemaster.h>
#include <Board.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    bool AuricRunemaster::s_registered = false;

    AuricRunemaster::AuricRunemaster() :
            Fyreslayer("Auric Runemaster", 4, WOUNDS, 8, 4, false),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_brazierStaff(Weapon::Type::Melee, "Brazier-staff", 2, 1, 4, 3, -1, RAND_D3),
            m_runicIron(Weapon::Type::Melee, "Runic Iron", 1, 2, 3, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, PRIEST, AURIC_RUNEMASTER};
        m_weapons = {&m_throwingAxe, &m_brazierStaff, &m_runicIron};
        m_battleFieldRole = Leader;

        s_globalToHitReroll.connect(this, &AuricRunemaster::holySeekerToHitRerolls, &m_holySeekerToHitSlot);
        s_globalToWoundReroll.connect(this, &AuricRunemaster::holySeekerToWoundRerolls, &m_holySeekerToWoundSlot);
    }

    AuricRunemaster::~AuricRunemaster() {
        m_holySeekerToHitSlot.disconnect();
        m_holySeekerToWoundSlot.disconnect();
    }

    bool AuricRunemaster::configure(Prayer prayer) {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMeleeWeapon(&m_brazierStaff);
        model->addMeleeWeapon(&m_runicIron);
        addModel(model);

        m_prayer = prayer;

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *AuricRunemaster::Create(const ParameterList &parameters) {
        auto unit = new AuricRunemaster();

        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        unit->setLodge(lodge);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_masterSmiterTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_forgeTempleArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto prayer = (Prayer) GetEnumParam("Prayer", parameters, g_prayers[0]);

        bool ok = unit->configure(prayer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AuricRunemaster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AuricRunemaster::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    AuricRunemaster::ComputePoints,
                    {
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                            EnumParameter("Command Trait", g_masterSmiterTraits[0], g_masterSmiterTraits),
                            EnumParameter("Artefact", g_forgeTempleArtefacts[0], g_forgeTempleArtefacts),
                            EnumParameter("Prayer", g_prayers[0], g_prayers),
                            BoolParameter("General")
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Auric Runemaster", factoryMethod);
        }
    }

    int AuricRunemaster::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void AuricRunemaster::onStartHero(PlayerId player) {
        Fyreslayer::onStartHero(player);

        // Holy Seeker
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= 12.0)) {
            Dice::RollResult result;
            Dice::rollD6(2, result);
            if (result.rollsGE(6) >= 1) {
                m_holySeekerToHit = true;
                m_holySeekerTarget = unit;
            }
            if (result.rollsGE(6) == 2) {
                m_holySeekerToWound = true;
            }
        }
    }

    void AuricRunemaster::onRestore() {
        Unit::onRestore();

        m_holySeekerToHit = false;
        m_holySeekerToWound = false;
        m_holySeekerTarget = nullptr;
    }

    Rerolls
    AuricRunemaster::holySeekerToHitRerolls(const Unit *attacker, const Weapon * /*weapon*/, const Unit *target) {

        if (m_holySeekerToHit && (target == m_holySeekerTarget)) {
            if (isFriendly(attacker) && attacker->hasKeyword(FYRESLAYERS)) {
                return RerollOnes;
            }
        }
        return NoRerolls;
    }

    Rerolls
    AuricRunemaster::holySeekerToWoundRerolls(const Unit *attacker, const Weapon * /*weapon*/, const Unit *target) {

        if (m_holySeekerToWound && (target == m_holySeekerTarget)) {
            if (isFriendly(attacker) && attacker->hasKeyword(FYRESLAYERS)) {
                return RerollOnes;
            }
        }
        return NoRerolls;
    }

} // namespace Fyreslayers