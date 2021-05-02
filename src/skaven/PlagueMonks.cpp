/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/PlagueMonks.h>
#include <Board.h>
#include <UnitFactory.h>
#include <array>

namespace Skaven {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 280;

    bool PlagueMonks::s_registered = false;

    PlagueMonks::PlagueMonks(int points) :
            Skaventide("Plague Monks", 6, g_wounds, 5, 6, false, points),
            m_pairedBlades(Weapon::Type::Melee, "Pair of Foetid Blade", 1, 2, 4, 4, 0, 1),
            m_bladeAndStave(Weapon::Type::Melee, "Foetid Blade Woe-stave", 2, 2, 4, 4, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, PLAGUE_MONKS};
        m_weapons = {&m_pairedBlades, &m_bladeAndStave};
    }

    bool PlagueMonks::configure(int numModels, WeaponOptions weapons, int bannerBearers, int harbingers) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }
        int maxBanners = numModels / g_minUnitSize;
        int maxHarbingers = numModels / g_minUnitSize;
        if (bannerBearers > maxBanners) {
            return false;
        }
        if (harbingers > maxHarbingers) {
            return false;
        }

        m_weaponOption = weapons;
        m_numBanners = bannerBearers;
        m_numHarbingers = harbingers;

        auto bringer = new Model(g_basesize, wounds());
        if (weapons == Paired_Foetid_Blades) {
            bringer->addMeleeWeapon(&m_pairedBlades);
        } else if (weapons == Foetid_Blade_And_Woe_Stave) {
            bringer->addMeleeWeapon(&m_bladeAndStave);
        }
        bringer->setName("Bringer-of-the-Word");
        addModel(bringer);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Paired_Foetid_Blades) {
                model->addMeleeWeapon(&m_pairedBlades);
            } else if (weapons == Foetid_Blade_And_Woe_Stave) {
                model->addMeleeWeapon(&m_bladeAndStave);
            }
            addModel(model);
        }

        return true;
    }

    Unit *PlagueMonks::Create(const ParameterList &parameters) {
        auto unit = new PlagueMonks(ComputePoints(parameters));
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOptions weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, Paired_Foetid_Blades);
        int numBanners = GetIntParam("Standard Bearers", parameters, 1);
        int numHarbingers = GetIntParam("Plague Harbingers", parameters, 1);

        bool ok = unit->configure(numModels, weapons, numBanners, numHarbingers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void PlagueMonks::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Paired_Foetid_Blades, Foetid_Blade_And_Woe_Stave};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Paired_Foetid_Blades, weapons),
                            IntegerParameter("Standard Bearers", 0, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Plague Harbingers", 0, 0, g_maxUnitSize / g_minUnitSize, 1)
                    },
                    CHAOS,
                    {SKAVEN}
            };
            s_registered = UnitFactory::Register("Plague Monks", factoryMethod);
        }
    }

    int PlagueMonks::runModifier() const {
        // Plague Harbingers
        int modifier = Skaventide::runModifier();
        if (m_numHarbingers > 0)
            modifier += 1;
        return modifier;
    }

    int PlagueMonks::chargeModifier() const {
        // Plague Harbingers
        int modifier = Skaventide::chargeModifier();
        if (m_numHarbingers > 0)
            modifier += 1;
        return modifier;
    }


    int PlagueMonks::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = Skaventide::extraAttacks(attackingModel, weapon, target);
        // Frenzied Assault
        if (m_charged && weapon->isMelee()) {
            attacks += 1;
        }
        return attacks;
    }

    void PlagueMonks::onStartHero(PlayerId player) {
        if (owningPlayer() == player) {
            // Book of Woes
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(player));
            if (distanceTo(unit) <= 13.0 && !unit->hasKeyword(NURGLE)) {
                int roll = Dice::RollD6();
                if (roll == 6) {
                    unit->applyDamage({0, Dice::RollD3()}, this);
                } else if (roll >= 4) {
                    unit->applyDamage({0, 1}, this);
                }
            }
        }
    }

    std::string PlagueMonks::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Paired_Foetid_Blades) {
                return "Paired Foetid Blades";
            } else if (parameter.intValue == Foetid_Blade_And_Woe_Stave) {
                return "Foetid Blade And Woe Stave";
            }
        }

        return ParameterValueToString(parameter);
    }

    int PlagueMonks::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Paired Foetid Blades") {
            return Paired_Foetid_Blades;
        } else if (enumString == "Foetid Blade And Woe Stave") {
            return Foetid_Blade_And_Woe_Stave;
        }
        return 0;
    }

    int PlagueMonks::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int PlagueMonks::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        auto hit = Skaventide::generateHits(unmodifiedHitRoll, weapon, unit);
        // Foetid Weapons
        if ((unmodifiedHitRoll == 6) && weapon->isMelee()) {
            hit++;
        }
        return hit;
    }

    void PlagueMonks::onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) {
        Skaventide::onFriendlyModelSlain(numSlain, attacker, source);

        if ((source == Wounds::Source::Weapon_Melee) && (m_numBanners > 0)) {
            Dice::RollResult rolls;
            Dice::RollD6(numSlain, rolls);
            attacker->applyDamage({0, rolls.rollsGE(6), Wounds::Source::Ability}, this);
        }
    }

} // namespace Skaven
