/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/Endrinriggers.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KharadronPrivate.h"

namespace KharadronOverlords {
    static const int g_basesize = 0;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 400;

    bool Endrinriggers::s_registered = false;

    Unit *Endrinriggers::Create(const ParameterList &parameters) {
        auto port = (Skyport) GetEnumParam("Skyport", parameters, g_skyport[0]);
        auto artycle = (Artycle) GetEnumParam("Artycle", parameters, g_artycles[0]);
        auto amendment = (Amendment) GetEnumParam("Amendment", parameters, g_amendments[0]);
        auto footnote = (Footnote) GetEnumParam("Footnote", parameters, g_footnotes[0]);
        int numModel = GetIntParam("Models", parameters, g_minUnitSize);
        int numVolleyGuns = GetIntParam("Volley Guns", parameters, 1);
        int numDrills = GetIntParam("Drill Launchers", parameters, 0);
        int numGrapnels = GetIntParam("Grapnel Launchers", parameters, 1);
        int numSkyhooks = GetIntParam("Skyhooks", parameters, 0);
        return new Endrinriggers(port, artycle, amendment, footnote, numModel, numVolleyGuns, numDrills, numGrapnels, numSkyhooks, ComputePoints(parameters));
    }

    std::string Endrinriggers::ValueToString(const Parameter &parameter) {
        return KharadronBase::ValueToString(parameter);
    }

    int Endrinriggers::EnumStringToInt(const std::string &enumString) {
        return KharadronBase::EnumStringToInt(enumString);
    }

    void Endrinriggers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Endrinriggers::Create,
                    Endrinriggers::ValueToString,
                    Endrinriggers::EnumStringToInt,
                    Endrinriggers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            IntegerParameter("Volley Guns", 1, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Skyhooks", 0, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Grapnel Launchers", 0, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Drill Launchers", 0, 0, g_maxUnitSize / g_minUnitSize, 1),
                            EnumParameter("Skyport", g_skyport[0], g_skyport),
                            EnumParameter("Artycle", g_artycles[0], g_artycles),
                            EnumParameter("Amendment", g_amendments[0], g_amendments),
                            EnumParameter("Footnote", g_footnotes[0], g_footnotes),
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Endrinriggers", factoryMethod);
        }
    }

    Endrinriggers::Endrinriggers(Skyport port, Artycle artycle, Amendment amendment, Footnote footnote, int numModels, int numVolleyGuns,
                                 int numDrills, int numGrapnels, int numSkyhooks, int points) :
            KharadronBase(port, artycle, amendment, footnote, "Endrinriggers", 12, g_wounds, 7, 4, true, points),
            m_volleyGun(Weapon::Type::Missile, "Aeathermatic Volley Gun", 24, 6, 4, 4, -1, 1),
            m_skyhook(Weapon::Type::Missile, "Grapnel Launcher or Skyhook", 24, 1, 4, 3, -2, 3),
            m_drillLauncher(Weapon::Type::Missile, "Drill Launcher", 24, 1, 4, 3, -3, RAND_D3),
            m_rivetGun(Weapon::Type::Missile, "Rapid-fire Rivet Gun", 12, 3, 3, 4, -1, 1),
            m_saw(Weapon::Type::Melee, "Aethermatic Saw", 1, 1, 3, 2, -2, RAND_D3),
            m_gunButt(Weapon::Type::Melee, "Gun Butt", 1, 1, 4, 5, 0, 1),
            m_sawMaster(Weapon::Type::Melee, "Aethermatic Saw", 1, 2, 3, 2, -2, RAND_D3),
            m_gunButtMaster(Weapon::Type::Melee, "Gun Butt", 1, 2, 4, 5, 0, 1) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, SKYFARERS, ENDRINRIGGERS};
        m_weapons = {&m_volleyGun, &m_skyhook, &m_drillLauncher, &m_rivetGun, &m_saw,
                     &m_gunButt, &m_sawMaster, &m_gunButtMaster};

        auto master = new Model(g_basesize, wounds());
        if (numVolleyGuns > 0) {
            master->addMissileWeapon(&m_volleyGun);
            master->addMeleeWeapon(&m_gunButtMaster);
            numVolleyGuns--;
        } else if (numDrills > 0) {
            master->addMissileWeapon(&m_drillLauncher);
            master->addMeleeWeapon(&m_gunButtMaster);
            numDrills--;
        } else if (numSkyhooks > 0) {
            master->addMissileWeapon(&m_skyhook);
            master->addMeleeWeapon(&m_gunButtMaster);
            numSkyhooks--;
        } else if (numGrapnels > 0) {
            master->addMissileWeapon(&m_skyhook);
            master->addMeleeWeapon(&m_gunButtMaster);
            numGrapnels--;
        } else {
            master->addMissileWeapon(&m_rivetGun);
            master->addMeleeWeapon(&m_sawMaster);
        }
        addModel(master);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (numVolleyGuns > 0) {
                model->addMissileWeapon(&m_volleyGun);
                model->addMeleeWeapon(&m_gunButt);
                numVolleyGuns--;
            } else if (numDrills > 0) {
                model->addMissileWeapon(&m_drillLauncher);
                model->addMeleeWeapon(&m_gunButt);
                numDrills--;
            } else if (numSkyhooks > 0) {
                model->addMissileWeapon(&m_skyhook);
                model->addMeleeWeapon(&m_gunButt);
                numSkyhooks--;
            } else if (numGrapnels > 0) {
                model->addMissileWeapon(&m_skyhook);
                model->addMeleeWeapon(&m_gunButt);
                numGrapnels--;
            } else {
                model->addMissileWeapon(&m_rivetGun);
                model->addMeleeWeapon(&m_saw);
            }
            addModel(model);
        }
    }

    Wounds Endrinriggers::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Drill Launcher
        if ((hitRoll == 6) && (weapon->name() == m_drillLauncher.name())) {
            return {0, 3};
        }
        return Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int Endrinriggers::chargeModifier() const {
        auto mod = Unit::chargeModifier();

        // Skyhook
        for (const auto &m : m_models) {
            if (!m->slain() && !m->fled() && m->hasWeapon(m_skyhook.name())) {
                mod++;
                break;
            }
        }

        return mod;
    }

    int Endrinriggers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void Endrinriggers::onStartHero(PlayerId player) {
        KharadronBase::onStartHero(player);

        // Endrincraft
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 1.0);
            for (auto unit : units) {
                if (unit->hasKeyword(SKYVESSEL) && (unit->remainingWounds() < unit->wounds())) {
                    Dice::RollResult rolls;
                    Dice::RollD6(remainingModels(), rolls);
                    unit->heal(rolls.rollsGE(4));
                    break;
                }
            }
        }
    }

} //KharadronOverlords
