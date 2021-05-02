/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/KurnothHunters.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SylvanethPrivate.h"

namespace Sylvaneth {
    static const int g_basesize = 50;
    static const int g_wounds = 5;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 190;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool KurnothHunters::s_registered = false;

    KurnothHunters::KurnothHunters(Glade glade, int numModels, WeaponOption weapons, int points) :
            SylvanethBase("Kurnoth Hunters", 5, g_wounds, 7, 4, false, points),
            m_greatbow(Weapon::Type::Missile, "Kurnoth Greatbow", 30, 2, 4, 3, -1, RAND_D3),
            m_greatbowHuntmaster(Weapon::Type::Missile, "Kurnoth Greatbow", 30, 2, 3, 3, -1, RAND_D3),
            m_greatsword(Weapon::Type::Melee, "Kurnoth Greatsword", 1, 4, 3, 3, -1, 2),
            m_greatswordHuntmaster(Weapon::Type::Melee, "Kurnoth Greatsword", 1, 4, 2, 3, -1, 2),
            m_scythe(Weapon::Type::Melee, "Kurnoth Scythe", 2, 3, 3, 3, -2, RAND_D3),
            m_scytheHuntmaster(Weapon::Type::Melee, "Kurnoth Scythe", 2, 3, 2, 3, -2, RAND_D3),
            m_viciousClaws(Weapon::Type::Melee, "Vicious Claws", 1, 3, 4, 4, 0, 1) {
        m_keywords = {ORDER, SYLVANETH, FREE_SPIRITS, KURNOTH_HUNTERS};
        m_weapons = {&m_greatbow, &m_greatbowHuntmaster, &m_greatsword, &m_greatswordHuntmaster, &m_scythe,
                     &m_scytheHuntmaster, &m_viciousClaws};

        setGlade(glade);

        m_weaponOption = weapons;

        auto huntmaster = new Model(g_basesize, wounds());
        if (m_weaponOption == Greatbows) {
            huntmaster->addMissileWeapon(&m_greatbowHuntmaster);
            huntmaster->addMeleeWeapon(&m_viciousClaws);
        } else if (m_weaponOption == Greatswords) {
            huntmaster->addMeleeWeapon(&m_greatswordHuntmaster);
        } else if (m_weaponOption == Scythes) {
            huntmaster->addMeleeWeapon(&m_scytheHuntmaster);
        }
        addModel(huntmaster);

        for (int i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (m_weaponOption == Greatbows) {
                model->addMissileWeapon(&m_greatbow);
                model->addMeleeWeapon(&m_viciousClaws);
            } else if (m_weaponOption == Greatswords) {
                model->addMeleeWeapon(&m_greatsword);
            } else if (m_weaponOption == Scythes) {
                model->addMeleeWeapon(&m_scythe);
            }
            addModel(model);
        }
    }

    Unit *KurnothHunters::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, KurnothHunters::Greatswords);
        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        return new KurnothHunters(glade, numModels, weapons, ComputePoints(parameters));
    }

    void KurnothHunters::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Greatswords, Scythes, Greatbows};
            static FactoryMethod factoryMethod = {
                    KurnothHunters::Create,
                    KurnothHunters::ValueToString,
                    KurnothHunters::EnumStringToInt,
                    KurnothHunters::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Greatswords, weapons),
                            EnumParameter("Glade", g_glade[0], g_glade),
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Kurnoth Hunters", factoryMethod);
        }
    }

    std::string KurnothHunters::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Greatswords) {
                return "Greatswords";
            } else if (parameter.intValue == Scythes) {
                return "Scythes";
            } else if (parameter.intValue == Greatbows) {
                return "Greatbows";
            }
        }
        return SylvanethBase::ValueToString(parameter);
    }

    int KurnothHunters::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Greatswords") {
            return Greatswords;
        } else if (enumString == "Scythes") {
            return Scythes;
        } else if (enumString == "Greatbows") {
            return Greatbows;
        }
        return SylvanethBase::EnumStringToInt(enumString);
    }

    Wounds KurnothHunters::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Sundering Strike
        if ((weapon->name() == m_greatsword.name()) && (woundRoll == 6)) {
            return {weapon->damage(), 1};
        }
        return Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void KurnothHunters::onEndCombat(PlayerId player) {
        Unit::onEndCombat(player);

        // Trample Underfoot
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        if (!units.empty()) {
            auto unit = units.front();
            Dice::RollResult result;
            Dice::RollD6(unit->remainingModels(), result);
            Wounds trampleWounds = {0, result.rollsGE(4)};
            unit->applyDamage(trampleWounds, this);
        }
    }

    int KurnothHunters::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Sylvaneth
