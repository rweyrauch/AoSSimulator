/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/ArkanautFrigate.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include "KharadronPrivate.h"

namespace KharadronOverlords {
    static const int g_basesize = 0;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 220;

    struct TableEntry {
        int m_move;
        int m_boardingAttacks;
        int m_bombRacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 6, 1},
                    {10, 5, 0},
                    {8,  4, 0},
                    {6,  3, -1},
                    {4,  2, -1}
            };

    bool ArkanautFrigate::s_registered = false;

    Unit *ArkanautFrigate::Create(const ParameterList &parameters) {
        auto unit = new ArkanautFrigate();
        auto option = (WeaponOption) GetEnumParam("Weapon", parameters, Heavy_Sky_Cannon);

        auto port = (Skyport) GetEnumParam("Skyport", parameters, g_skyport[0]);
        unit->setSkyport(port);

        auto artycle = (Artycle) GetEnumParam("Artycle", parameters, g_artycles[0]);
        auto amendment = (Amendment) GetEnumParam("Amendment", parameters, g_amendments[0]);
        auto footnote = (Footnote) GetEnumParam("Footnote", parameters, g_footnotes[0]);
        unit->setCode(artycle, amendment, footnote);

        auto endrinwork = (Endrinwork) GetEnumParam("Endrinwork", parameters, g_frigateEndrinworks[0]);

        bool ok = unit->configure(option, endrinwork);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string ArkanautFrigate::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Heavy_Sky_Cannon) return "Heavy Sky Cannon";
            else if (parameter.intValue == Heavy_Skyhook) return "Heavy Skyhook";
        }
        return KharadronBase::ValueToString(parameter);
    }

    int ArkanautFrigate::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Heavy Sky Cannon") return Heavy_Sky_Cannon;
        else if (enumString == "Heavy Skyhook") return Heavy_Skyhook;
        return KharadronBase::EnumStringToInt(enumString);
    }

    void ArkanautFrigate::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Heavy_Sky_Cannon, Heavy_Skyhook};
            static FactoryMethod factoryMethod = {
                    ArkanautFrigate::Create,
                    ArkanautFrigate::ValueToString,
                    ArkanautFrigate::EnumStringToInt,
                    ArkanautFrigate::ComputePoints,
                    {
                            EnumParameter("Weapon", Heavy_Sky_Cannon, weapons),
                            EnumParameter("Skyport", g_skyport[0], g_skyport),
                            EnumParameter("Artycle", g_artycles[0], g_artycles),
                            EnumParameter("Amendment", g_amendments[0], g_amendments),
                            EnumParameter("Footnote", g_footnotes[0], g_footnotes),
                            EnumParameter("Endrinworks", g_frigateEndrinworks[0], g_frigateEndrinworks)
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Arkanaut Frigate", factoryMethod);
        }
    }

    ArkanautFrigate::ArkanautFrigate() :
            KharadronBase("Arkanaut Frigate", 12, g_wounds, 8, 4, true),
            m_cannonShrapnel(Weapon::Type::Missile, "Heavy Sky Cannon: Shrapnel", 24, RAND_D6, 3, 3, -1, 2),
            m_cannonShell(Weapon::Type::Missile, "Heavy Sky Cannon: Shell", 30, 1, 3, 2, -2, RAND_D6),
            m_skyhook(Weapon::Type::Missile, "Heavy Skyhook", 24, 1, 3, 2, -2, RAND_D6),
            m_carbines(Weapon::Type::Missile, "Aethershot Carbines", 12, 4, 3, 3, -1, 2),
            m_boardingWeapons(Weapon::Type::Melee, "Boarding Weapons", 1, 6, 4, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, WAR_MACHINE, SKYVESSEL, ARKANAUT_FRIGATE};
        m_weapons = {&m_cannonShrapnel,
                     &m_cannonShell,
                     &m_skyhook,
                     &m_carbines,
                     &m_boardingWeapons};
        m_battleFieldRole = Behemoth;
    }

    bool ArkanautFrigate::configure(WeaponOption option, Endrinwork endrinwork) {
        auto model = new Model(g_basesize, wounds());
        if (option == Heavy_Sky_Cannon) {
            model->addMissileWeapon(&m_cannonShrapnel);
            model->addMissileWeapon(&m_cannonShell);
            m_cannonShell.activate(false);
        } else if (option == Heavy_Skyhook) {
            model->addMissileWeapon(&m_skyhook);
        }
        model->addMissileWeapon(&m_carbines);
        model->addMeleeWeapon(&m_boardingWeapons);
        addModel(model);

        m_weaponOption = option;
        m_endrinwork = endrinwork;

        m_points = g_pointsPerUnit;

        return true;
    }

    void ArkanautFrigate::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    void ArkanautFrigate::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_boardingWeapons.setAttacks(g_damageTable[damageIndex].m_boardingAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int ArkanautFrigate::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void ArkanautFrigate::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        if (m_weaponOption == Heavy_Sky_Cannon) {
            auto nearestUnit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (nearestUnit) {
                const auto preferShell = (nearestUnit->save() < 4);
                m_cannonShell.activate(preferShell);
                m_cannonShrapnel.activate(!preferShell);
            }
        }
    }

    int ArkanautFrigate::chargeModifier() const {
        auto mod = Unit::chargeModifier();

        if (m_weaponOption == Heavy_Skyhook) mod += 2;

        return mod;
    }

    void ArkanautFrigate::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Aetheric Navigator/Endrinrigger
        if (player == owningPlayer()) {
            m_models.front()->applyHealing(1);
        }

    }

    Rerolls ArkanautFrigate::runRerolls() const {
        // Aetheric Navigator/Endrinrigger
        return Reroll_Failed;
    }

    void ArkanautFrigate::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        // Bomb Racks
        auto nearestUnit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (nearestUnit && (distanceTo(nearestUnit) <= 1.0)) {
            auto roll = Dice::RollD6();
            roll += g_damageTable[getDamageTableIndex()].m_bombRacks;

            if (roll >= 4) {
                int wounds = Dice::RollD3();
                nearestUnit->applyDamage({0, wounds}, this);
            }
        }
    }

    int ArkanautFrigate::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //KharadronOverlords
