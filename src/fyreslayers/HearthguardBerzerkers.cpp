/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/HearthguardBerzerkers.h>
#include <Board.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 400;

    bool HearthguardBerzerkers::s_registered = false;

    HearthguardBerzerkers::HearthguardBerzerkers() :
            Fyreslayer("Hearthguard Berzerkers", 4, g_wounds, 8, 5, false),
            m_broadaxe(Weapon::Type::Melee, "Berzerker Broadaxe", 2, 2, 3, 3, -1, 2),
            m_broadaxeKarl(Weapon::Type::Melee, "Berzerker Broadaxe", 2, 3, 3, 3, -1, 2),
            m_poleaxe(Weapon::Type::Melee, "Flamestrike Poleaxe", 2, 2, 3, 3, 0, 1),
            m_poleaxeKarl(Weapon::Type::Melee, "Flamestrike Poleaxe", 2, 3, 3, 3, 0, 1),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HEARTHGUARD_BERZERKERS};
        m_weapons = {&m_broadaxe, &m_broadaxeKarl, &m_poleaxe, &m_poleaxeKarl, &m_throwingAxe};
    }

    bool HearthguardBerzerkers::configure(int numModels, WeaponOption weapons) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto karl = new Model(g_basesize, wounds());
        karl->addMissileWeapon(&m_throwingAxe);
        if (weapons == Berzerker_Broadaxe) {
            karl->addMeleeWeapon(&m_broadaxeKarl);
        } else if (weapons == Flamestrike_Poleaxe) {
            karl->addMeleeWeapon(&m_poleaxeKarl);
        }
        addModel(karl);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_throwingAxe);
            if (weapons == Berzerker_Broadaxe) {
                model->addMeleeWeapon(&m_broadaxe);
            } else if (weapons == Flamestrike_Poleaxe) {
                model->addMeleeWeapon(&m_poleaxe);
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *HearthguardBerzerkers::Create(const ParameterList &parameters) {
        auto unit = new HearthguardBerzerkers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Berzerker_Broadaxe);

        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        unit->setLodge(lodge);

        bool ok = unit->configure(numModels, weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void HearthguardBerzerkers::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Berzerker_Broadaxe, Flamestrike_Poleaxe};
            static FactoryMethod factoryMethod = {
                    HearthguardBerzerkers::Create,
                    HearthguardBerzerkers::ValueToString,
                    HearthguardBerzerkers::EnumStringToInt,
                    HearthguardBerzerkers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Berzerker_Broadaxe, weapons),
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Hearthguard Berzerkers", factoryMethod);
        }
    }

    std::string HearthguardBerzerkers::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Berzerker_Broadaxe) { return "Berzerker Broadaxe"; }
            else if (parameter.intValue == Flamestrike_Poleaxe) { return "Flamestrike Poleaxe"; }
        }
        return Fyreslayer::ValueToString(parameter);
    }

    int HearthguardBerzerkers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Berzerker Broadaxe") { return Berzerker_Broadaxe; }
        else if (enumString == "Flamestrike Poleaxe") { return Flamestrike_Poleaxe; }
        return Fyreslayer::EnumStringToInt(enumString);
    }

    Wounds
    HearthguardBerzerkers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Smouldering Boulders
        if ((hitRoll == 6) && (weapon->name() == m_poleaxe.name())) {
            return {weapon->damage(), 2};
        }
        return Fyreslayer::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Wounds HearthguardBerzerkers::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        // Duty Unto Death
        int saveValue = 6;

        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 10.0);
        for (auto unit : units) {
            if (unit->hasKeyword(HERO) && unit->hasKeyword(FYRESLAYERS)) {
                saveValue -= 2;
            }
        }
        return ignoreWounds(wounds, saveValue);
    }

    int HearthguardBerzerkers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Fyreslayers