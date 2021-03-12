/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/BloodwrackMedusa.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "DaughterOfKhainePrivate.h"
#include "LoreOfShadows.h"
#include "DoKCommands.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 100;

    bool BloodwrackMedusa::s_registered = false;

    BloodwrackMedusa::BloodwrackMedusa() :
            DaughterOfKhaine("Bloodwrack Medusa", 8, g_wounds, 8, 5, false),
            m_bloodwrackStare(Weapon::Type::Missile, "Bloodwrack Stare", 10, 1, 0, 0, -7, 0),
            m_whisperclaw(Weapon::Type::Melee, "Whisperclaw", 1, 4, 3, 3, 0, 1),
            m_tailOfSerpents(Weapon::Type::Melee, "Tail of Serpents", 2, RAND_D6, 4, 4, 0, 1),
            m_bloodwrackSpear(Weapon::Type::Melee, "Bloodwrack Spear", 2, 2, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, MELUSAI, DAUGHTERS_OF_KHAINE, HERO, WIZARD, BLOODWRACK_MEDUSA};
        m_weapons = {&m_bloodwrackStare, &m_whisperclaw, &m_tailOfSerpents, &m_bloodwrackSpear};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool BloodwrackMedusa::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_bloodwrackStare);
        model->addMeleeWeapon(&m_whisperclaw);
        model->addMeleeWeapon(&m_tailOfSerpents);
        model->addMeleeWeapon(&m_bloodwrackSpear);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<BuffModifierSpell>(this, "Enfeebling Foe", 5, 18, Attribute::To_Wound_Melee, -1,
                                                                    Abilities::Target::Enemy));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        configureCommon();

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *BloodwrackMedusa::Create(const ParameterList &parameters) {
        auto unit = new BloodwrackMedusa();

        auto temple = (Temple)GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_medusaCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_wizardArtefacts[0]);
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

    void BloodwrackMedusa::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Temple", g_temple[0], g_temple),
                            EnumParameter("Command Trait", g_medusaCommandTraits[0], g_medusaCommandTraits),
                            EnumParameter("Artefact", g_wizardArtefacts[0], g_wizardArtefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Bloodwrack Medusa", factoryMethod);
        }
    }

    Wounds BloodwrackMedusa::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if (weapon->name() == m_bloodwrackStare.name()) {
            Dice::RollResult result;
            Dice::RollD6(target->remainingModels(), result);
            return {0, result.rollsGE(5)};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int BloodwrackMedusa::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace DaughtersOfKhaine

