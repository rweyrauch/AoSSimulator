/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordCelestantOnDracoth.h>
#include <iostream>
#include <Board.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 200;

    bool LordCelestantOnDracoth::s_registered = false;

    LordCelestantOnDracoth::LordCelestantOnDracoth() :
            StormcastEternal("Lord-Celestant on Dracoth", 10, WOUNDS, 9, 3, false),
            m_stormstrikeGlaive(Weapon::Type::Melee, "Stormstrike Glaive", 2, 4, 3, 4, -1, 1),
            m_lightningHammer(Weapon::Type::Melee, "Lightning Hammer", 1, 3, 3, 3, -1, 2),
            m_thunderaxe(Weapon::Type::Melee, "Thunderaxe", 2, 3, 3, 3, -1, 2),
            m_tempestosHammer(Weapon::Type::Melee, "Tempestos Hammer", 2, 3, 3, 2, -1, RAND_D3),
            m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, DRACOTH, STORMCAST_ETERNAL, HERO, LORD_CELESTANT};
        m_weapons = {&m_stormstrikeGlaive, &m_lightningHammer, &m_thunderaxe, &m_tempestosHammer, &m_clawsAndFangs};
        m_battleFieldRole = Leader;
        m_hasMount = true;
    }

    bool LordCelestantOnDracoth::configure(WeaponOption weapons, bool sigmariteThundershield) {
        m_weapon = weapons;
        m_sigmariteThundershield = sigmariteThundershield;

        auto model = new Model(BASESIZE, wounds());
        if (m_weapon == StormstrikeGlaive) {
            model->addMeleeWeapon(&m_stormstrikeGlaive);
        } else if (m_weapon == LightningHammer) {
            model->addMeleeWeapon(&m_lightningHammer);
        } else if (m_weapon == Thunderaxe) {
            model->addMeleeWeapon(&m_thunderaxe);
        } else if (m_weapon == TempestosHammer) {
            model->addMeleeWeapon(&m_tempestosHammer);
        }

        model->addMeleeWeapon(&m_clawsAndFangs);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *LordCelestantOnDracoth::Create(const ParameterList &parameters) {
        auto unit = new LordCelestantOnDracoth();
        auto weapons = (WeaponOption) GetEnumParam("Weapon", parameters, LightningHammer);
        bool sigmariteThundershield = GetBoolParam("Sigmarite Thundershield", parameters, false);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(weapons, sigmariteThundershield);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LordCelestantOnDracoth::Init() {
        if (!s_registered) {
            static const std::array<int, 4> weapons = {TempestosHammer, Thunderaxe, LightningHammer, StormstrikeGlaive};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Weapon", TempestosHammer, weapons),
                            BoolParameter("Sigmarite Thundershield"),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };
            s_registered = UnitFactory::Register("Lord-Celestant on Dracoth", factoryMethod);
        }
    }

    std::string LordCelestantOnDracoth::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == TempestosHammer) {
                return "Tempestos Hammer";
            } else if (parameter.intValue == Thunderaxe) {
                return "Thunderaxe";
            } else if (parameter.intValue == LightningHammer) {
                return "Lightning Hammer";
            } else if (parameter.intValue == StormstrikeGlaive) {
                return "Stormstrike Glaive";
            }
        }
        return StormcastEternal::ValueToString(parameter);
    }

    int
    LordCelestantOnDracoth::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = StormcastEternal::extraAttacks(attackingModel, weapon, target);

        // Tempestos Hammer
        if (m_charged && weapon->name() == m_tempestosHammer.name()) {
            attacks += Dice::rollD3();
        }

        // Thunderaxe
        if ((weapon->name() == m_thunderaxe.name())) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 9.0);
            for (auto unit : units) {
                if (unit->hasKeyword(STORMCAST_ETERNAL)) attacks++;
            }
        }

        return attacks;
    }

    Rerolls LordCelestantOnDracoth::toSaveRerolls(const Weapon *weapon) const {
        // Sigmarite Thundershield
        if (m_sigmariteThundershield) {
            return RerollOnes;
        }
        return StormcastEternal::toSaveRerolls(weapon);
    }

    Wounds LordCelestantOnDracoth::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = StormcastEternal::computeReturnedDamage(weapon, saveRoll);
        // Sigmarite Thundershield
        if (m_sigmariteThundershield) {
            // 1 mortal wound for each save of a 6
            if (saveRoll == 6) {
                wounds += {0, 1};
            }
        }
        return wounds;
    }

    int LordCelestantOnDracoth::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Tempestos Hammer") {
            return TempestosHammer;
        } else if (enumString == "Thunderaxe") {
            return Thunderaxe;
        } else if (enumString == "Lightning Hammer") {
            return LightningHammer;
        } else if (enumString == "Stormstrike Glaive") {
            return StormstrikeGlaive;
        }
        return StormcastEternal::EnumStringToInt(enumString);
    }

    Wounds
    LordCelestantOnDracoth::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Intolerable Damage
        if ((woundRoll == 6) && (weapon->name() == m_clawsAndFangs.name())) {
            // D6 instead of 1
            return {Dice::rollD6(), 0};
        }

        // Lightning Hammer
        if ((hitRoll == 6) && (weapon->name() == m_lightningHammer.name())) {
            return {weapon->damage(), 2};
        }

        if (m_charged && (m_weapon == StormstrikeGlaive)) {
            return {weapon->damage() + 2, 0};
        }

        return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int LordCelestantOnDracoth::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void LordCelestantOnDracoth::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        // Storm Breath
        if (owningPlayer() == player) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) <= 12.0)) {
                if (Dice::rollD6() >= 4) {
                    unit->applyDamage({0, Dice::rollD3()});
                }
                auto units = Board::Instance()->getUnitsWithin(unit, GetEnemyId(owningPlayer()), 2.0);
                for (auto target : units) {
                    if (Dice::rollD6() >= 4) {
                        target->applyDamage({0, Dice::rollD3()});
                    }
                }
            }
        }
    }

} // namespace StormcastEternals