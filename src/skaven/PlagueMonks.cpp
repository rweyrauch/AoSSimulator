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

    PlagueMonks::PlagueMonks() :
            Skaventide("Plague Monks", 6, g_wounds, 5, 6, false),
            m_foetidBlade(Weapon::Type::Melee, "Foetid Blade", 1, 2, 4, 4, 0, 1),
            m_woeStave(Weapon::Type::Melee, "Woe-stave", 2, 1, 4, 5, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, PLAGUE_MONKS};
        m_weapons = {&m_foetidBlade, &m_woeStave};
    }

    bool PlagueMonks::configure(int numModels, WeaponOptions weapons, int contagionBanners, int iconsOfPestilence,
                                int doomGongs, int baleChimes) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }
        int maxBanners = numModels / g_minUnitSize;
        int maxHarbingers = numModels / g_minUnitSize;
        if (contagionBanners + iconsOfPestilence > maxBanners) {
            return false;
        }
        if (doomGongs + baleChimes > maxHarbingers) {
            return false;
        }

        m_weaponOption = weapons;
        m_numContagionBanners = contagionBanners;
        m_numIconsOfPestilence = iconsOfPestilence;
        m_numDoomGongs = doomGongs;
        m_numBaleChimes = baleChimes;

        auto bringer = new Model(g_basesize, wounds());
        bringer->addMeleeWeapon(&m_foetidBlade);
        addModel(bringer);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Paired_Foetid_Blades) {
                model->addMeleeWeapon(&m_foetidBlade);
            } else if (weapons == Foetid_Blade_And_Woe_Stave) {
                model->addMeleeWeapon(&m_foetidBlade);
                model->addMeleeWeapon(&m_woeStave);
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *PlagueMonks::Create(const ParameterList &parameters) {
        auto unit = new PlagueMonks();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOptions weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, Paired_Foetid_Blades);
        int contagionBanners = GetIntParam("Contagion Banners", parameters, 0);
        int iconsOfPestilence = GetIntParam("Icons Of Pestilence", parameters, 0);
        int doomGongs = GetIntParam("Doom Gongs", parameters, 0);
        int baleChimes = GetIntParam("Bale Chimes", parameters, 0);

        bool ok = unit->configure(numModels, weapons, contagionBanners, iconsOfPestilence, doomGongs, baleChimes);
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
                            IntegerParameter("Contagion Banners", 0, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Icons Of Pestilence", 0, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Doom Gongs", 0, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Bale Chimes", 0, 0, g_maxUnitSize / g_minUnitSize, 1)
                    },
                    CHAOS,
                    {SKAVEN}
            };
            s_registered = UnitFactory::Register("Plague Monks", factoryMethod);
        }
    }

    Wounds PlagueMonks::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Icon of Pestilence
        if ((woundRoll == 6) && (m_numIconsOfPestilence > 0) && !weapon->isMissile()) {
            return {weapon->damage() + 1, 0};
        }
        return Skaventide::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int PlagueMonks::runModifier() const {
        // Doom Gongs
        int modifier = Skaventide::runModifier();
        if (m_numDoomGongs > 0)
            modifier += 1;
        return modifier;
    }

    int PlagueMonks::chargeModifier() const {
        // Doom Gongs
        int modifier = Skaventide::chargeModifier();
        if (m_numDoomGongs > 0)
            modifier += 1;
        return modifier;
    }

    Rerolls PlagueMonks::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Pair of Foetid Blades
        if ((m_weaponOption == Paired_Foetid_Blades) && (weapon->name() == m_foetidBlade.name())) {
            return Reroll_Failed;
        }
        return Skaventide::toHitRerolls(weapon, target);
    }

    int PlagueMonks::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = Skaventide::extraAttacks(attackingModel, weapon, target);
        // Frenzied Assault
        if (m_charged && !weapon->isMissile()) {
            attacks += 1;
        }
        return attacks;
    }

    void PlagueMonks::onStartHero(PlayerId player) {
        if (owningPlayer() == player) {
            // Book of Woes
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(player));
            if (distanceTo(unit) <= 13.0 && !unit->hasKeyword(CLANS_PESTILENS)) {
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

    int PlagueMonks::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int PlagueMonks::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Bale-chime
        if ((m_numBaleChimes > 0) && (hitRoll == 6) && !weapon->isMissile()) {
            return weapon->rend() - 1;
        }
        return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
    }

} // namespace Skaven
