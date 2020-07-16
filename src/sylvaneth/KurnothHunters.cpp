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
    static const int BASESIZE = 50;
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 190;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    bool KurnothHunters::s_registered = false;

    KurnothHunters::KurnothHunters() :
            SylvanethBase("Kurnoth Hunters", 5, WOUNDS, 7, 4, false),
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
    }

    bool KurnothHunters::configure(int numModels, WeaponOption weapons) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_weaponOption = weapons;

        auto huntmaster = new Model(BASESIZE, wounds());
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
            auto model = new Model(BASESIZE, wounds());
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

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *KurnothHunters::Create(const ParameterList &parameters) {
        auto unit = new KurnothHunters();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, KurnothHunters::Greatswords);

        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        unit->setGlade(glade);

        bool ok = unit->configure(numModels, weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
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

    Wounds KurnothHunters::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Sundering Strike
        if ((weapon->name() == m_greatsword.name()) && (woundRoll == 6)) {
            return {weapon->damage(), 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Wounds KurnothHunters::onEndCombat(PlayerId player) {
        auto wounds = Unit::onEndCombat(player);

        // Trample Underfoot
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        if (!units.empty()) {
            auto unit = units.front();
            Dice::RollResult result;
            Dice::rollD6(unit->remainingModels(), result);
            Wounds trampleWounds = {0, result.rollsGE(4)};
            unit->applyDamage(trampleWounds);
            wounds += trampleWounds;
        }
        return wounds;
    }

    int KurnothHunters::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Sylvaneth
