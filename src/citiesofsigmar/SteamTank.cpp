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
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 105;
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 180;
    static const int g_pointsForCommander = 50;

    struct TableEntry {
        int m_move;
        int m_canonRange;
        int m_gunToWound;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {RAND_2D6, 30, 2},
                    {RAND_2D6, 24, 3},
                    {RAND_D6,  18, 4},
                    {RAND_D6,  12, 5},
                    {RAND_D3,  6,  6}
            };

    bool SteamTank::s_registered = false;

    Unit *SteamTank::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        auto commander = GetBoolParam("Commander", parameters, true);
        auto drug = (Narcotic) GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new SteamTank(city, commander, drug, trait, artefact, general);
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
                            BoolParameter("Commander"),
                            EnumParameter("City", g_city[0], g_city),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Steam Tank", factoryMethod);
        }
    }

    SteamTank::SteamTank(City city, bool commander, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral) :
            CitizenOfSigmar(city, "Steam Tank", RAND_2D6, g_wounds, 8, 3, false, g_pointsPerUnit),
            m_steamCannon(Weapon::Type::Missile, "Steam Cannon", 30, 1, 4, 2, -2, RAND_D6),
            m_steamGun(Weapon::Type::Missile, "Steam Gun", 8, RAND_2D6, 4, 2, 0, 1),
            m_longRifle(Weapon::Type::Missile, "Long Rifle", 30, 1, 3, 3, -1, 2),
            m_handgun(Weapon::Type::Missile, "Repeater Handgun", 16, RAND_D3, 4, 3, -1, 1),
            m_crushingWheels(Weapon::Type::Melee, "Crushing Wheels", 1, RAND_D6, 4, 3, -1, 2),
            m_sword(Weapon::Type::Melee, "Sword or Rod", 1, 2, 5, 4, 0, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, WAR_MACHINE, STEAM_TANK};
        m_weapons = {&m_steamCannon, &m_steamGun, &m_longRifle, &m_handgun, &m_crushingWheels, &m_sword};
        m_battleFieldRole = Role::Behemoth;

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_steamCannon);
        model->addMissileWeapon(&m_steamGun);
        model->addMeleeWeapon(&m_crushingWheels);

        if (commander) {
            addKeyword(HERO);
            model->addMissileWeapon(&m_longRifle);
            model->addMissileWeapon(&m_handgun);
            model->addMeleeWeapon(&m_sword);
            m_battleFieldRole = Role::Leader_Behemoth;
            m_points += g_pointsForCommander;
            setCommandTrait(trait);
            setArtefact(artefact);
            setGeneral(isGeneral);
            setNarcotic(narcotic);
        }
        addModel(model);
        m_commander = commander;
    }

    void SteamTank::onRestore() {
        CitizenOfSigmar::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    void SteamTank::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_steamCannon.setRange(g_damageTable[damageIndex].m_canonRange);
        m_steamGun.setToWound(g_damageTable[damageIndex].m_gunToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        CitizenOfSigmar::onWounded();
    }

    size_t SteamTank::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int SteamTank::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = CitizenOfSigmar::toHitModifier(weapon, target);

        // Bouncing Cannon Balls
        if ((weapon->name() == m_steamCannon.name()) && (target->remainingModels() >= 10)) mod++;

        return mod;
    }

    void SteamTank::onCharged() {
        CitizenOfSigmar::onCharged();

        // Steel Behemoth
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && distanceTo(unit) <= 1.0) {
            if (Dice::RollD6() >= 2) {
                unit->applyDamage({0, Dice::RollD3()}, this);
            }
        }
    }

    void SteamTank::onStartHero(PlayerId player) {
        CitizenOfSigmar::onStartHero(player);

        if (owningPlayer() == player) {
            m_overpressured = false;

            // Apply 'More Pressure!' or 'I'll Fix It'
            if ((remainingWounds() < initialWounds()) && m_commander) {
                // Damaged - try to heal
                heal(Dice::RollD3());
            } else {
                // TODO: decide to overpressure
            }
        }
    }

    int SteamTank::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = CitizenOfSigmar::extraAttacks(attackingModel, weapon, target);

        if (m_overpressured) extra += 2;

        return extra;
    }

    int SteamTank::moveModifier() const {
        auto mod = CitizenOfSigmar::moveModifier();

        if (m_overpressured) mod += 2;

        return mod;
    }

    int SteamTank::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace CitiesOfSigmar