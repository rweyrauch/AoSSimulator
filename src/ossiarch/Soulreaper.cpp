/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "ossiarch/Soulreaper.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool MortisanSoulreaper::s_registered = false;

    Unit *MortisanSoulreaper::Create(const ParameterList &parameters) {
        auto unit = new MortisanSoulreaper();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        unit->setLegion(legion);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortisanCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_soulreaperArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string MortisanSoulreaper::ValueToString(const Parameter &parameter) {
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int MortisanSoulreaper::EnumStringToInt(const std::string &enumString) {
        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void MortisanSoulreaper::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MortisanSoulreaper::Create,
                    MortisanSoulreaper::ValueToString,
                    MortisanSoulreaper::EnumStringToInt,
                    MortisanSoulreaper::ComputePoints,
                    {
                            EnumParameter("Legion", g_legion[0], g_legion),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Command Trait", g_mortisanCommandTraits[0], g_mortisanCommandTraits),
                            EnumParameter("Artefact", g_soulreaperArtefacts[0], g_soulreaperArtefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Mortisan Soulreaper", factoryMethod);
        }
    }

    MortisanSoulreaper::MortisanSoulreaper() :
            OssiarchBonereaperBase("Mortisan Soulreaper", 5, g_wounds, 10, 4, false),
            m_scythe(Weapon::Type::Melee, "Soulreaper Scythe", 2, 3, 3, 3, -1, 2) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTISAN, HERO, WIZARD, MORTISAN_SOULREAPER};
        m_weapons = {&m_scythe};
        m_battleFieldRole = Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool MortisanSoulreaper::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_scythe);
        addModel(model);

        m_lore = lore;

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds
    MortisanSoulreaper::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Deathly Touch
        if ((hitRoll == 6) && (weapon->name() == m_scythe.name())) {
            return {0, 2};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls MortisanSoulreaper::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Soulreaper
        if ((target->remainingModels() >= 5) && (weapon->name() == m_scythe.name())) {
            return Reroll_Failed;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int MortisanSoulreaper::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OssiarchBonereapers
