/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/Skywardens.h>
#include <UnitFactory.h>
#include "KharadronPrivate.h"

namespace KharadronOverlords {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 400;

    bool Skywardens::s_registered = false;

    Unit *Skywardens::Create(const ParameterList &parameters) {
        auto unit = new Skywardens(ComputePoints(parameters));
        int numModel = GetIntParam("Models", parameters, g_minUnitSize);
        int numVolleyGuns = GetIntParam("Volley Guns", parameters, 1);
        int numDrills = GetIntParam("Drill Launchers", parameters, 0);
        int numGrapnels = GetIntParam("Grapnel Launchers", parameters, 1);
        int numSkyhooks = GetIntParam("Skyhooks", parameters, 0);

        auto port = (Skyport) GetEnumParam("Skyport", parameters, g_skyport[0]);
        unit->setSkyport(port);

        auto artycle = (Artycle) GetEnumParam("Artycle", parameters, g_artycles[0]);
        auto amendment = (Amendment) GetEnumParam("Amendment", parameters, g_amendments[0]);
        auto footnote = (Footnote) GetEnumParam("Footnote", parameters, g_footnotes[0]);
        unit->setCode(artycle, amendment, footnote);

        bool ok = unit->configure(numModel, numVolleyGuns, numDrills, numGrapnels, numSkyhooks);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string Skywardens::ValueToString(const Parameter &parameter) {
        return KharadronBase::ValueToString(parameter);
    }

    int Skywardens::EnumStringToInt(const std::string &enumString) {
        return KharadronBase::EnumStringToInt(enumString);
    }

    void Skywardens::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Skywardens::Create,
                    Skywardens::ValueToString,
                    Skywardens::EnumStringToInt,
                    Skywardens::ComputePoints,
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
            s_registered = UnitFactory::Register("Skywardens", factoryMethod);
        }
    }

    Skywardens::Skywardens(int points) :
            KharadronBase("Skywardens", 4, g_wounds, 6, 4, false, points),
            m_volleyGun(Weapon::Type::Missile, "Aethermatic Volley Gun", 24, 6, 4, 4, -1, 1),
            m_skyhook(Weapon::Type::Missile, "Grapnel Launcher or Skyhook", 24, 1, 4, 3, -2, 3),
            m_drillLauncher(Weapon::Type::Missile, "Drill Launcher", 24, 1, 4, 3, -3, RAND_D3),
            m_pistol(Weapon::Type::Missile, "Vulcaniser Pistol", 9, 2, 3, 3, -1, 1),
            m_gunButt(Weapon::Type::Melee, "Gun Butt", 1, 1, 4, 5, 0, 1),
            m_skypike(Weapon::Type::Melee, "Skypike", 2, 2, 4, 3, -1, RAND_D3),
            m_gunButtCustodian(Weapon::Type::Melee, "Gun Butt", 1, 2, 4, 5, 0, 1),
            m_skypikeCustodian(Weapon::Type::Melee, "Skypike", 2, 3, 4, 3, -1, RAND_D3) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, SKYFARERS, SKYWARDENS};
        m_weapons = {&m_volleyGun, &m_skyhook, &m_drillLauncher, &m_pistol, &m_gunButt, &m_skypike,
                     &m_gunButtCustodian, &m_skypikeCustodian};
    }

    bool Skywardens::configure(int numModels, int numVolleyGuns, int numDrills, int numGrapnels, int numSkyhooks) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }
        const int maxSpecials = numModels / g_minUnitSize;
        if (numVolleyGuns > maxSpecials || (numDrills + numSkyhooks + numGrapnels) > maxSpecials) {
            return false;
        }

        auto master = new Model(g_basesize, wounds());
        if (numVolleyGuns > 0) {
            master->addMissileWeapon(&m_volleyGun);
            master->addMeleeWeapon(&m_gunButtCustodian);
            numVolleyGuns--;
        } else if (numDrills > 0) {
            master->addMissileWeapon(&m_drillLauncher);
            master->addMeleeWeapon(&m_gunButtCustodian);
            numDrills--;
        } else if (numSkyhooks > 0) {
            master->addMissileWeapon(&m_skyhook);
            master->addMeleeWeapon(&m_gunButtCustodian);
            numSkyhooks--;
        } else if (numGrapnels > 0) {
            master->addMissileWeapon(&m_skyhook);
            master->addMeleeWeapon(&m_gunButtCustodian);
            numGrapnels--;
        } else {
            master->addMissileWeapon(&m_pistol);
            master->addMeleeWeapon(&m_skypikeCustodian);
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
                model->addMissileWeapon(&m_pistol);
                model->addMeleeWeapon(&m_skypike);
            }
            addModel(model);
        }

        return true;
    }

    Wounds Skywardens::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Drill Launcher
        if ((hitRoll == 6) && (weapon->name() == m_drillLauncher.name())) {
            return {0, 3};
        }
        return Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int Skywardens::chargeModifier() const {
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

    int Skywardens::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //KharadronOverlords
