/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/EndrinmasterWithSuit.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KharadronPrivate.h"

namespace KharadronOverlords {
    static const int g_basesize = 0;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 190;

    bool EndrinmasterWithDirigibleSuit::s_registered = false;

    Unit *EndrinmasterWithDirigibleSuit::Create(const ParameterList &parameters) {
        auto port = (Skyport) GetEnumParam("Skyport", parameters, g_skyport[0]);
        auto artycle = (Artycle) GetEnumParam("Artycle", parameters, g_artycles[0]);
        auto amendment = (Amendment) GetEnumParam("Amendment", parameters, g_amendments[0]);
        auto footnote = (Footnote) GetEnumParam("Footnote", parameters, g_footnotes[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_endrinmasterCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_dirigibleArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new EndrinmasterWithDirigibleSuit(port, artycle, amendment, footnote, trait, artefact, general);
    }

    void EndrinmasterWithDirigibleSuit::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    EndrinmasterWithDirigibleSuit::Create,
                    KharadronBase::ValueToString,
                    KharadronBase::EnumStringToInt,
                    EndrinmasterWithDirigibleSuit::ComputePoints,
                    {
                            EnumParameter("Skyport", g_skyport[0], g_skyport),
                            EnumParameter("Artycle", g_artycles[0], g_artycles),
                            EnumParameter("Amendment", g_amendments[0], g_amendments),
                            EnumParameter("Footnote", g_footnotes[0], g_footnotes),
                            EnumParameter("Command Trait", g_endrinmasterCommandTraits[0], g_endrinmasterCommandTraits),
                            EnumParameter("Artefact", g_dirigibleArtefacts[0], g_dirigibleArtefacts),
                            BoolParameter("General")

                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Endrinmaster with Dirigible Suit", factoryMethod);
        }
    }

    EndrinmasterWithDirigibleSuit::EndrinmasterWithDirigibleSuit(Skyport port, Artycle artycle, Amendment amendment, Footnote footnote,
                                                                 CommandTrait trait, Artefact artefact, bool isGeneral) :
            KharadronBase(port, artycle, amendment, footnote, "Endrinmaster with Dirigible Suit", 12, g_wounds, 8, 3, true, g_pointsPerUnit),
            m_aethercannon(Weapon::Type::Missile, "Aethercannon", 12, 1, 3, 2, -2, RAND_D3),
            m_weaponBattery(Weapon::Type::Missile, "Dirigible Suit Weapon Battery", 18, 6, 3, 3, -1, 1),
            m_gaze(Weapon::Type::Missile, "Gaze of Grungni", 9, 1, 3, 2, -1, RAND_D3),
            m_saw(Weapon::Type::Melee, "Aethermatic Saw", 1, 3, 3, 2, -2, RAND_D3) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, HERO, SKYFARER, ENDRINMASTER};
        m_weapons = {&m_aethercannon, &m_weaponBattery, &m_gaze, &m_saw};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_aethercannon);
        model->addMissileWeapon(&m_weaponBattery);
        model->addMissileWeapon(&m_gaze);
        model->addMeleeWeapon(&m_saw);
        addModel(model);
    }

    int EndrinmasterWithDirigibleSuit::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void EndrinmasterWithDirigibleSuit::onStartHero(PlayerId player) {
        KharadronBase::onStartHero(player);

        // Endrinmaster
        auto vessels = Board::Instance()->getUnitsWithin(this, owningPlayer(), 1.0);
        for (auto vessel : vessels) {
            if (vessel->hasKeyword(SKYVESSEL) && (vessel->remainingWounds() < vessel->wounds())) {
                heal(3);
                break;
            }
        }
    }

} //KharadronOverlords
