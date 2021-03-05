/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/ArkanautIronclad.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KharadronPrivate.h"

namespace KharadronOverlords {
    static const int g_basesize = 0;
    static const int g_wounds = 18;
    static const int g_pointsPerUnit = 480;

    struct TableEntry {
        int m_move;
        int m_boardingAttacks;
        int m_bombRacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 15, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 8, 2},
                    {8,  7, 1},
                    {6,  6, 0},
                    {6,  5, 0},
                    {4,  4, -1}
            };

    bool ArkanautIronclad::s_registered = false;

    Unit *ArkanautIronclad::Create(const ParameterList &parameters) {
        auto unit = new ArkanautIronclad();
        auto option = (WeaponOption) GetEnumParam("Weapon", parameters, Great_Sky_Cannon);

        auto port = (Skyport) GetEnumParam("Skyport", parameters, g_skyport[0]);
        unit->setSkyport(port);

        auto artycle = (Artycle) GetEnumParam("Artycle", parameters, g_artycles[0]);
        auto amendment = (Amendment) GetEnumParam("Amendment", parameters, g_amendments[0]);
        auto footnote = (Footnote) GetEnumParam("Footnote", parameters, g_footnotes[0]);
        unit->setCode(artycle, amendment, footnote);

        auto endrinwork = (Endrinwork) GetEnumParam("Endrinwork", parameters, g_ironcladEndrinworks[0]);
        unit->setEndrinwork(endrinwork);

        bool ok = unit->configure(option);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string ArkanautIronclad::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Great_Sky_Cannon) return "Great Sky Cannon";
            else if (parameter.intValue == Great_Volley_Cannon) return "Great Volley Cannon";
            else if (parameter.intValue == Great_Skyhook) return "Great Skyhook";
        }
        return KharadronBase::ValueToString(parameter);
    }

    int ArkanautIronclad::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Great Sky Cannon") return Great_Sky_Cannon;
        else if (enumString == "Great Volley Cannon") return Great_Volley_Cannon;
        else if (enumString == "Great Skyhook") return Great_Skyhook;
        return KharadronBase::EnumStringToInt(enumString);
    }

    void ArkanautIronclad::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Great_Sky_Cannon, Great_Skyhook, Great_Volley_Cannon};
            static FactoryMethod factoryMethod = {
                    ArkanautIronclad::Create,
                    ArkanautIronclad::ValueToString,
                    ArkanautIronclad::EnumStringToInt,
                    ArkanautIronclad::ComputePoints,
                    {
                            EnumParameter("Weapon", Great_Sky_Cannon, weapons),
                            EnumParameter("Skyport", g_skyport[0], g_skyport),
                            EnumParameter("Artycle", g_artycles[0], g_artycles),
                            EnumParameter("Amendment", g_amendments[0], g_amendments),
                            EnumParameter("Footnote", g_footnotes[0], g_footnotes),
                            EnumParameter("Endrinworks", g_ironcladEndrinworks[0], g_ironcladEndrinworks)
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Arkanaut Ironclad", factoryMethod);
        }
    }

    ArkanautIronclad::ArkanautIronclad() :
            KharadronBase("Arkanaut Ironclad", 10, g_wounds, 8, 3, true),
            m_cannonShrapnel(Weapon::Type::Missile, "Great Sky Cannon: Shrapnel", 24, 6, 3, 3, -1, 2),
            m_cannonShell(Weapon::Type::Missile, "Great Sky Cannon: Shell", 30, 1, 3, 2, -2, 6),
            m_skyhook(Weapon::Type::Missile, "Great Skyhook", 24, 1, 3, 2, -2, 6),
            m_volleyCannon(Weapon::Type::Missile, "Great Volley Cannon", 18, RAND_4D6, 3, 3, -1, 1),
            m_torpedoes(Weapon::Type::Missile, "Aethershock Torpedoes", 24, 4, 4, 3, -1, RAND_D3),
            m_carbines(Weapon::Type::Missile, "Aethershot Carbines", 12, 8, 3, 3, -1, 2),
            m_boardingWeapons(Weapon::Type::Melee, "Boarding Weapons", 1, 8, 4, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, WAR_MACHINE, SKYVESSEL, ARKANAUT_IRONCLAD};
        m_weapons = {&m_cannonShrapnel,
                     &m_cannonShell,
                     &m_skyhook,
                     &m_volleyCannon,
                     &m_torpedoes,
                     &m_carbines,
                     &m_boardingWeapons};
        m_battleFieldRole = Behemoth;
    }

    bool ArkanautIronclad::configure(WeaponOption option) {
        auto model = new Model(g_basesize, wounds());
        if (option == Great_Sky_Cannon) {
            model->addMissileWeapon(&m_cannonShrapnel);
            model->addMissileWeapon(&m_cannonShell);
            m_cannonShell.activate(false);
        } else if (option == Great_Skyhook) {
            model->addMissileWeapon(&m_skyhook);
        } else if (option == Great_Volley_Cannon) {
            model->addMissileWeapon(&m_volleyCannon);
        }
        model->addMissileWeapon(&m_carbines);
        model->addMissileWeapon(&m_torpedoes);
        model->addMeleeWeapon(&m_boardingWeapons);
        addModel(model);

        m_weaponOption = option;

        m_points = g_pointsPerUnit;

        return true;
    }

    void ArkanautIronclad::onRestore() {
        KharadronBase::onRestore();
        // Restore table-driven attributes
        onWounded();
    }

    void ArkanautIronclad::onWounded() {
        KharadronBase::onWounded();
        const int damageIndex = getDamageTableIndex();
        m_boardingWeapons.setAttacks(g_damageTable[damageIndex].m_boardingAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int ArkanautIronclad::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void ArkanautIronclad::onStartShooting(PlayerId player) {
        KharadronBase::onStartShooting(player);

        if (m_weaponOption == Great_Sky_Cannon) {
            auto nearestUnit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (nearestUnit) {
                const auto preferShell = (nearestUnit->save() < 4);
                m_cannonShell.activate(preferShell);
                m_cannonShrapnel.activate(!preferShell);
            }
        }
    }

    int ArkanautIronclad::chargeModifier() const {
        auto mod = KharadronBase::chargeModifier();

        if (m_weaponOption == Great_Skyhook) mod += 2;

        return mod;
    }

    void ArkanautIronclad::onStartHero(PlayerId player) {
        KharadronBase::onStartHero(player);

        // Aetheric Navigator/Endrinrigger
        if (player == owningPlayer()) {
            m_models.front()->applyHealing(1);
        }
    }

    Rerolls ArkanautIronclad::runRerolls() const {
        // Aetheric Navigator/Endrinrigger
        return Reroll_Failed;
    }

    void ArkanautIronclad::onStartCombat(PlayerId player) {
        KharadronBase::onStartCombat(player);

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

    int ArkanautIronclad::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //KharadronOverlords
