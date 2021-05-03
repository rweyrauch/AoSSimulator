/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/Stabbas.h>
#include <UnitFactory.h>
#include <iostream>

namespace GloomspiteGitz {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 20;
    static const int g_maxUnitSize = 60;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = 360;

    bool Stabbas::s_registered = false;

    Stabbas::Stabbas(int numModels, WeaponOption weapons, WeaponOption bossWeapon, int numBarbedNets,
                     int numGongbashers, int numFlagbearers, int numIconbearers, int points) :
            GloomspiteGitzBase("Stabbas", 5, g_wounds, 4, 6, false, points),
            m_stabba(Weapon::Type::Melee, "Stabba", 1, 1, 4, 4, 0, 1),
            m_stabbaBoss(Weapon::Type::Melee, "Stabba", 1, 1, 3, 4, 0, 1),
            m_pokinSpear(Weapon::Type::Melee, "Pokin' Spear", 2, 1, 5, 4, 0, 1),
            m_pokinSpearBoss(Weapon::Type::Melee, "Pokin' Spear", 2, 1, 4, 4, 0, 1),
            m_barbedNet(Weapon::Type::Melee, "Barbed Net", 2, 3, 4, 5, 0, 1) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, STABBAS};
        m_weapons = {&m_stabba, &m_stabbaBoss, &m_pokinSpear, &m_pokinSpearBoss, &m_barbedNet};
        m_battleFieldRole = Role::Battleline;

        // Add the boss
        auto boss = new Model(g_basesize, wounds());
        if (bossWeapon == Stabba) {
            boss->addMeleeWeapon(&m_stabbaBoss);
            m_ranks = 2;
        } else if (bossWeapon == Pokin_Spear) {
            boss->addMeleeWeapon(&m_pokinSpearBoss);
            m_ranks = 3;
        }
        addModel(boss);

        // add the nets
        for (auto i = 1; i < numBarbedNets; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_barbedNet);
            model->setName("Netter");
            addModel(model);
        }

        // and the rest
        auto currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Stabba) {
                model->addMeleeWeapon(&m_stabba);
            } else if (weapons == Pokin_Spear) {
                model->addMeleeWeapon(&m_pokinSpear);
            }
            if (numFlagbearers > 0) {
                model->setName(Model::FlagBearer);
                numFlagbearers--;
            } else if (numGongbashers > 0) {
                model->setName("Gong Basher");
                numGongbashers--;
            } else if (numIconbearers > 0) {
                model->setName(Model::IconBearer);
                numIconbearers--;
            }
            addModel(model);
        }
    }

    Unit *Stabbas::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Stabba);
        auto bossWeapon = (WeaponOption) GetEnumParam("Boss Weapon", parameters, Stabba);
        int numBarbedNets = GetIntParam("Barbed Nets", parameters, 0);
        int numGongbashers = GetIntParam("Gong Bashers", parameters, 0);
        int numFlagbearers = GetIntParam("Flag Bearers", parameters, 0);
        int numIconbearers = GetIntParam("Icon Bearers", parameters, 0);
        return new Stabbas(numModels, weapons, bossWeapon, numBarbedNets, numGongbashers, numFlagbearers,
                           numIconbearers, ComputePoints(parameters));
    }

    void Stabbas::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Stabbas::Stabba, Stabbas::Pokin_Spear};
            static FactoryMethod factoryMethod = {
                    Stabbas::Create,
                    Stabbas::ValueToString,
                    Stabbas::EnumStringToInt,
                    Stabbas::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Stabbas::Stabba, weapons),
                            EnumParameter("Boss Weapon", Stabbas::Stabba, weapons),
                            IntegerParameter("Barbed Nets", 3, 0, 3 * g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Gong Bashers", 1, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Flag Bearers", 1, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Icon Bearers", 0, 0, g_maxUnitSize / g_minUnitSize, 1),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Stabbas", factoryMethod);
        }
    }

    int Stabbas::toWoundModifier(const Weapon *weapon, const Unit *unit) const {
        int modifier = GloomspiteGitzBase::toWoundModifier(weapon, unit);
        // Backstabbing Mob
        if (!weapon->isMissile()) {
            if (remainingModels() >= 30) {
                modifier += 2;
            } else if (remainingModels() >= 15) {
                modifier += 1;
            }
        }
        return modifier;
    }

    int Stabbas::runModifier() const {
        int modifier = GloomspiteGitzBase::runModifier();
        if (isNamedModelAlive("Gong Basher")) {
            modifier += 2;
        }
        return modifier;
    }

    int Stabbas::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = GloomspiteGitzBase::toSaveModifier(weapon, attacker);
        if (isNamedModelAlive(Model::IconBearer) && weapon->isMissile()) {
            modifier += 1;
        }
        // Moonshields
        if (remainingModels() >= 10) {
            modifier += 1;
        }
        return modifier;
    }

    int Stabbas::braveryModifier() const {
        int modifier = GloomspiteGitzBase::braveryModifier();
        if (isNamedModelAlive(Model::FlagBearer)) {
            modifier += 1;
        }
        return modifier;
    }

    std::string Stabbas::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons" || std::string(parameter.name) == "Boss Weapon") {
            if (parameter.intValue == Stabba) {
                return "Stabba";
            } else if (parameter.intValue == Pokin_Spear) {
                return "Pokin' Spear";
            }
        }
        return GloomspiteGitzBase::ValueToString(parameter);
    }

    int Stabbas::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Stabba") {
            return Stabba;
        } else if (enumString == "Pokin' Spear") {
            return Pokin_Spear;
        }
        return GloomspiteGitzBase::EnumStringToInt(enumString);
    }

    int Stabbas::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = GloomspiteGitzBase::targetHitModifier(weapon, attacker);
        // Netters
        if (distanceTo(attacker) <= 2.0) {
            bool hasNets = false;
            for (const auto &ip : m_models) {
                for (auto wip = ip->meleeWeaponBegin(); wip != ip->meleeWeaponEnd(); ++wip) {
                    const Weapon *w = *wip;

                    if (w->name() == m_barbedNet.name()) {
                        hasNets = true;
                        break;
                    }
                }
            }
            if (hasNets) {
                modifier -= 1;
            }
        }
        return modifier;
    }

    int Stabbas::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace GloomspiteGitz