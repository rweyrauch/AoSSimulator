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

namespace KharadronOverlords {
    static const int BASESIZE = 0;
    static const int WOUNDS = 18;
    static const int POINTS_PER_UNIT = 510;

    struct TableEntry {
        int m_move;
        int m_boardingAttacks;
        int m_bombRacks;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 15, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
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
        auto option = (WeaponOption) GetEnumParam("Weapon", parameters, GreatSkyCannon);

        auto port = (Skyport) GetEnumParam("Skyport", parameters, KharadronBase::None);
        unit->setSkyport(port);

        bool ok = unit->configure(option);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string ArkanautIronclad::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == GreatSkyCannon) return "Great Sky Cannon";
            else if (parameter.intValue == GreatVolleyCannon) return "Great Volley Cannon";
            else if (parameter.intValue == GreatSkyhook) return "Great Skyhook";
        }
        return KharadronBase::ValueToString(parameter);
    }

    int ArkanautIronclad::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Great Sky Cannon") return GreatSkyCannon;
        else if (enumString == "Great Volley Cannon") return GreatVolleyCannon;
        else if (enumString == "Great Skyhook") return GreatSkyhook;
        return KharadronBase::EnumStringToInt(enumString);
    }

    void ArkanautIronclad::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ArkanautIronclad::Create,
                    ArkanautIronclad::ValueToString,
                    ArkanautIronclad::EnumStringToInt,
                    ArkanautIronclad::ComputePoints,
                    {
                            {
                                    ParamType::Enum, "Weapon", ArkanautIronclad::GreatSkyCannon,
                                    ArkanautIronclad::GreatSkyCannon,
                                    ArkanautIronclad::GreatVolleyCannon, 1
                            },
                            {ParamType::Enum, "Skyport", KharadronBase::None, KharadronBase::None,
                             KharadronBase::Custom, 1},
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Arkanaut Ironclad", factoryMethod);
        }
    }

    ArkanautIronclad::ArkanautIronclad() :
            KharadronBase("Arkanaut Ironclad", 10, WOUNDS, 8, 3, true),
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
    }

    bool ArkanautIronclad::configure(WeaponOption option) {
        auto model = new Model(BASESIZE, wounds());
        if (option == GreatSkyCannon) {
            model->addMissileWeapon(&m_cannonShrapnel);
            model->addMissileWeapon(&m_cannonShell);
            m_cannonShell.activate(false);
        } else if (option == GreatSkyhook) {
            model->addMissileWeapon(&m_skyhook);
        } else if (option == GreatVolleyCannon) {
            model->addMissileWeapon(&m_volleyCannon);
        }
        model->addMissileWeapon(&m_carbines);
        model->addMissileWeapon(&m_torpedoes);
        model->addMeleeWeapon(&m_boardingWeapons);
        addModel(model);

        m_weaponOption = option;

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void ArkanautIronclad::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    void ArkanautIronclad::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_boardingWeapons.setAttacks(g_damageTable[damageIndex].m_boardingAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int ArkanautIronclad::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void ArkanautIronclad::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        if (m_weaponOption == GreatSkyCannon) {
            auto nearestUnit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (nearestUnit) {
                const auto preferShell = (nearestUnit->save() < 4);
                m_cannonShell.activate(preferShell);
                m_cannonShrapnel.activate(!preferShell);
            }
        }
    }

    int ArkanautIronclad::chargeModifier() const {
        auto mod = Unit::chargeModifier();

        if (m_weaponOption == GreatSkyhook) mod += 2;

        return mod;
    }

    void ArkanautIronclad::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Aetheric Navigator/Endrinrigger
        if (player == owningPlayer()) {
            m_models.front()->applyHealing(1);
        }
    }

    Rerolls ArkanautIronclad::runRerolls() const {
        // Aetheric Navigator/Endrinrigger
        return RerollFailed;
    }

    void ArkanautIronclad::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        // Bomb Racks
        auto nearestUnit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (nearestUnit && (distanceTo(nearestUnit) <= 1.0f)) {
            auto roll = Dice::rollD6();
            roll += g_damageTable[getDamageTableIndex()].m_bombRacks;

            if (roll >= 4) {
                int wounds = Dice::rollD3();
                nearestUnit->applyDamage({0, wounds});
            }
        }
    }

    int ArkanautIronclad::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //KharadronOverlords
