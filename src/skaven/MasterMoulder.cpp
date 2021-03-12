/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/MasterMoulder.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SkavenPrivate.h"

namespace Skaven {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool MasterMoulder::s_registered = false;

    Unit *MasterMoulder::Create(const ParameterList &parameters) {
        auto unit = new MasterMoulder();

        WeaponOption option = Lash;

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_moulderCommandTraits[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_moulderArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(option);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void MasterMoulder::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Command Trait", g_moulderCommandTraits[0], g_moulderCommandTraits),
                            EnumParameter("Artefact", g_moulderArtefacts[0], g_moulderArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Master Moulder", factoryMethod);
        }
    }

    MasterMoulder::MasterMoulder() :
            Skaventide("Master Moulder", 6, g_wounds, 6, 5, false),
            m_lash(Weapon::Type::Melee, "Warpstone-tipped Lash", 3, 6, 3, 4, -1, 1),
            m_catcher(Weapon::Type::Melee, "Things-catcher", 2, 4, 4, 4, -1, 2) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_MOULDER, HERO, MASTER_MOULDER};
        m_weapons = {&m_lash, &m_catcher};
        m_battleFieldRole = Role::Leader;

        s_globalToHitMod.connect(this, &MasterMoulder::crackTheWhip, &m_whipSlot);
        s_globalBraveryMod.connect(this, &MasterMoulder::crackTheWhipBravery, &m_whipBraverySlot);
    }

    MasterMoulder::~MasterMoulder() {
        m_whipSlot.disconnect();
        m_whipBraverySlot.disconnect();
    }

    bool MasterMoulder::configure(WeaponOption option) {
        auto model = new Model(g_basesize, wounds());
        if (option == Lash)
            model->addMeleeWeapon(&m_lash);
        else if (option == Things_Catcher)
            model->addMeleeWeapon(&m_catcher);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    int MasterMoulder::crackTheWhip(const Unit *attacker, const Weapon * /*weapon*/, const Unit * /*target*/) {
        // Crack the Whip
        if (attacker->hasKeyword(CLANS_MOULDER) && attacker->hasKeyword(PACK) && (distanceTo(attacker) <= 12.0)) {
            return 1;
        }
        return 0;
    }

    int MasterMoulder::crackTheWhipBravery(const Unit *unit) {
        // Crack the Whip
        if (unit->hasKeyword(CLANS_MOULDER) && unit->hasKeyword(PACK) && (distanceTo(unit) <= 12.0)) {
            // Double unit's bravery
            return unit->bravery();
        }
        return 0;
    }

    void MasterMoulder::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Master Moulder
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 3.0);
        for (auto unit : units) {
            if (unit->hasKeyword(CLANS_MOULDER) && unit->hasKeyword(PACK) &&
                (unit->remainingWounds() < unit->initialWounds())) {
                unit->heal(Dice::RollD3());
                break;
            }
        }
    }

    int MasterMoulder::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
