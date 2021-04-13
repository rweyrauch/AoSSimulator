/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <idonethdeepkin/AkhelianKing.h>
#include <UnitFactory.h>
#include <Board.h>
#include "IdonethDeepkinPrivate.h"

namespace IdonethDeepkin {
    static const int g_basesize = 60;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 230;

    bool AkhelianKing::s_registered = false;

    AkhelianKing::AkhelianKing() :
            IdonethDeepkinBase("Akhelian King", 14, g_wounds, 8, 3, true),
            m_bladedPolearm(Weapon::Type::Melee, "Bladed Polearm", 2, 3, 3, 3, -2, RAND_D3),
            m_greatsword(Weapon::Type::Melee, "Greatsword", 1, 4, 3, 3, -1, RAND_D3),
            m_falchion(Weapon::Type::Melee, "Falchion", 1, 3, 3, 4, 0, 1),
            m_deepmareJawsTalons(Weapon::Type::Melee, "Deepmare's Fanged Jaw and Talons", 2, 3, 3, 3, -1, 1),
            m_deepmareTails(Weapon::Type::Melee, "Deepmare's Lashing Tails", 2, 3, 3, 3, 0, 2) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, HERO, AKHELIAN, AKHELIAN_KING};
        m_weapons = {&m_bladedPolearm, &m_greatsword, &m_falchion, &m_deepmareJawsTalons, &m_deepmareTails};
        m_battleFieldRole = Role::Leader;

        m_hasMount = true;
        m_deepmareJawsTalons.setMount(true);
        m_deepmareTails.setMount(true);
    }

    void AkhelianKing::configure(WeaponOption weapon, MountTrait trait) {
        auto model = new Model(g_basesize, wounds());
        if (weapon == Bladed_Polearm) {
            model->addMeleeWeapon(&m_bladedPolearm);
        } else {
            model->addMeleeWeapon(&m_greatsword);
        }
        model->addMeleeWeapon(&m_falchion);
        model->addMeleeWeapon(&m_deepmareJawsTalons);
        model->addMeleeWeapon(&m_deepmareTails);

        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *AkhelianKing::Create(const ParameterList &parameters) {
        auto unit = new AkhelianKing();
        WeaponOption weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Bladed_Polearm);

        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        unit->setEnclave(enclave);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_akhelianArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto mountTrait = (MountTrait) GetEnumParam("Mount Trait", parameters, g_leviadonTrait[0]);

        unit->configure(weapon, mountTrait);
        return unit;
    }

    void AkhelianKing::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Bladed_Polearm, Greatsword};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Weapon", Bladed_Polearm, weapons),
                            EnumParameter("Enclave", g_enclave[0], g_enclave),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_akhelianArtefacts[0], g_akhelianArtefacts),
                            EnumParameter("Mount Trait", g_deepmareTrait[0], g_deepmareTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {IDONETH_DEEPKIN}
            };

            s_registered = UnitFactory::Register("Akhelian King", factoryMethod);
        }
    }

    std::string AkhelianKing::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Bladed_Polearm) {
                return "Bladed Polearm";
            } else if (parameter.intValue == Greatsword) {
                return "Greatsword";
            }
        }
        return IdonethDeepkinBase::ValueToString(parameter);
    }

    int AkhelianKing::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Bladed Polearm") {
            return Bladed_Polearm;
        } else if (enumString == "Greatsword") {
            return Greatsword;
        }
        return IdonethDeepkinBase::EnumStringToInt(enumString);
    }

    Wounds AkhelianKing::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Wave Rider
        if (m_charged && (weapon->name() == m_bladedPolearm.name())) {
            return {3, 0};
        }
        return Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void AkhelianKing::onCharged() {
        // Deepmare Horn
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        if (!units.empty()) {
            int roll = Dice::RollD6();
            if (roll >= 2) {
                units.front()->applyDamage({0, Dice::RollD3()}, this);
            }
        }

        Unit::onCharged();
    }

    int AkhelianKing::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace IdonethDeepkin
