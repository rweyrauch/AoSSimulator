/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/SteamTank.h"

namespace CitiesOfSigmar {
    static const int BASESIZE = 105;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 200;
    static const int POINTS_FOR_COMMANDER = 50;

    struct TableEntry {
        int m_move;
        int m_canonRange;
        int m_gunToWound;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {RAND_2D6, 30, 2},
                    {RAND_2D6, 24, 3},
                    {RAND_D6,  18, 4},
                    {RAND_D6,  12, 5},
                    {RAND_D3,  6,  6}
            };

    bool SteamTank::s_registered = false;

    Unit *SteamTank::Create(const ParameterList &parameters) {
        auto unit = new SteamTank();

        auto commander = GetBoolParam("Commander", parameters, true);

        auto city = (City) GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
        unit->setCity(city);

        bool ok = unit->configure(commander);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string SteamTank::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int SteamTank::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void SteamTank::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SteamTank::Create,
                    SteamTank::ValueToString,
                    SteamTank::EnumStringToInt,
                    SteamTank::ComputePoints,
                    {
                            {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal,
                             CitizenOfSigmar::TempestsEye, 1},
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Steam Tank", factoryMethod);
        }
    }

    SteamTank::SteamTank() :
            CitizenOfSigmar("Steam Tank", RAND_2D6, WOUNDS, 8, 3, false),
            m_steamCannon(Weapon::Type::Missile, "Steam Cannon", 30, 1, 4, 2, -2, RAND_D6),
            m_steamGun(Weapon::Type::Missile, "Steam Gun", 8, RAND_2D6, 4, 2, 0, 1),
            m_longRifle(Weapon::Type::Missile, "Long Rifle", 30, 1, 3, 3, -1, 2),
            m_handgun(Weapon::Type::Missile, "Repeater Handgun", 16, RAND_D3, 4, 3, -1, 1),
            m_crushingWheels(Weapon::Type::Melee, "Crushing Wheels", 1, RAND_D6, 4, 3, -1, 2),
            m_sword(Weapon::Type::Melee, "Sword or Rod", 1, 2, 5, 4, 0, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, WAR_MACHINE, STEAM_TANK};
        m_weapons = {&m_steamCannon, &m_steamGun, &m_longRifle, &m_handgun, &m_crushingWheels, &m_sword};
    }

    bool SteamTank::configure(bool commander) {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_steamCannon);
        model->addMissileWeapon(&m_steamGun);
        model->addMeleeWeapon(&m_crushingWheels);

        m_points = POINTS_PER_UNIT;

        if (commander) {
            addKeyword(HERO);
            model->addMissileWeapon(&m_longRifle);
            model->addMissileWeapon(&m_handgun);
            model->addMeleeWeapon(&m_sword);

            m_points += POINTS_FOR_COMMANDER;
        }
        addModel(model);
        m_commander = commander;

        return true;
    }

    void SteamTank::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    void SteamTank::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_steamCannon.setRange(g_damageTable[damageIndex].m_canonRange);
        m_steamGun.setToWound(g_damageTable[damageIndex].m_gunToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        Unit::onWounded();
    }

    int SteamTank::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int SteamTank::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        // Bouncing Cannon Balls
        if ((weapon->name() == m_steamCannon.name()) && (target->remainingModels() >= 10)) mod++;

        return mod;
    }

    void SteamTank::onCharged() {
        Unit::onCharged();

        // Steel Behemoth
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && distanceTo(unit) <= 1.0f) {
            if (Dice::rollD6() >= 2) {
                unit->applyDamage({0, Dice::rollD3()});
            }
        }
    }

    void SteamTank::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {
            m_overpressured = false;

            // Apply 'More Pressure!' or 'I'll Fix It'
            if ((remainingWounds() < initialWounds()) && m_commander) {
                // Damaged - try to heal
                heal(Dice::rollD3());
            } else {
                // TODO: decide to overpressure
            }
        }
    }

    int SteamTank::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);

        if (m_overpressured) extra += 2;

        return extra;
    }

    int SteamTank::moveModifier() const {
        auto mod = Unit::moveModifier();

        if (m_overpressured) mod += 2;

        return mod;
    }

    int SteamTank::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace CitiesOfSigmar