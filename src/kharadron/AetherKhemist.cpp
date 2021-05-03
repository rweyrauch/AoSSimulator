/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/AetherKhemist.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KharadronPrivate.h"

namespace KharadronOverlords {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 90;

    bool AetherKhemist::s_registered = false;

    Unit *AetherKhemist::Create(const ParameterList &parameters) {
        auto port = (Skyport) GetEnumParam("Skyport", parameters, g_skyport[0]);
        auto artycle = (Artycle) GetEnumParam("Artycle", parameters, g_artycles[0]);
        auto amendment = (Amendment) GetEnumParam("Amendment", parameters, g_amendments[0]);
        auto footnote = (Footnote) GetEnumParam("Footnote", parameters, g_footnotes[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_khemistCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_khemistArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new AetherKhemist(port, artycle, amendment, footnote, trait, artefact, general);
    }

    void AetherKhemist::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    KharadronBase::ValueToString,
                    KharadronBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Skyport", g_skyport[0], g_skyport),
                            EnumParameter("Artycle", g_artycles[0], g_artycles),
                            EnumParameter("Amendment", g_amendments[0], g_amendments),
                            EnumParameter("Footnote", g_footnotes[0], g_footnotes),
                            EnumParameter("Command Trait", g_khemistCommandTraits[0], g_khemistCommandTraits),
                            EnumParameter("Artefact", g_khemistArtefacts[0], g_khemistArtefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };

            s_registered = UnitFactory::Register("Aether Khemist", factoryMethod);
        }
    }

    AetherKhemist::AetherKhemist(Skyport port, Artycle artycle, Amendment amendment, Footnote footnote, CommandTrait trait, Artefact artefact, bool isGeneral) :
            KharadronBase(port, artycle, amendment, footnote, "Aether Khemist", 4, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_anatomiser(Weapon::Type::Missile, "Atmospheric Anatomiser", 9, RAND_3D6, 4, 4, -2, 1),
            m_instruments(Weapon::Type::Melee, "Heavy Instruments", 1, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, HERO, SKYFARER, MARINE, AETHER_KHEMIST};
        m_weapons = {&m_anatomiser, &m_instruments};
        m_battleFieldRole = Role::Leader;

        s_globalToHitMod.connect(this, &AetherKhemist::atmosphericIsolation, &m_connection);

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_anatomiser);
        model->addMeleeWeapon(&m_instruments);
        addModel(model);
    }

    AetherKhemist::~AetherKhemist() {
        m_connection.disconnect();
    }

    int AetherKhemist::atmosphericIsolation(const Unit * /*attacker*/, const Weapon * /*weapon*/, const Unit *target) {
        // Atmospheric Isolation
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }

        return 0;
    }

    int AetherKhemist::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void AetherKhemist::onStartHero(PlayerId player) {
        KharadronBase::onStartHero(player);

        // Aetheric Augmentation
        auto totalUnits = (m_commandTrait == CommandTrait::Khemist_Supreme) ? 2 : 1;
        auto range = (m_commandTrait == CommandTrait::Genius_In_The_Making) ? 18.0 : 12.0;
        auto skyfarers = Board::Instance()->getUnitsWithin(this, owningPlayer(), range);
        for (auto unit : skyfarers) {
            if (unit->hasKeyword(SKYFARER)) {
                unit->buffReroll(Attribute::To_Wound_Melee, Rerolls::Ones,
                                 {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                totalUnits--;
                if (totalUnits <= 0) break;
            }
        }
    }

} //KharadronOverlords
