/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/Fateskimmer.h"
#include "TzeentchPrivate.h"
#include "TzeentchSpells.h"

namespace Tzeentch {
    static const int g_basesize = 40;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 140;

    bool Fateskimmer::s_registered = false;

    Unit *Fateskimmer::Create(const ParameterList &parameters) {
        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, g_changeCoven[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfChange[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new Fateskimmer(coven, lore, trait, artefact, general);
    }

    void Fateskimmer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Fateskimmer::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    Fateskimmer::ComputePoints,
                    {
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            EnumParameter("Lore", g_loreOfChange[0], g_loreOfChange),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Fateskimmer", factoryMethod);
        }
    }

    Fateskimmer::Fateskimmer(ChangeCoven coven, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            TzeentchBase("Fateskimmer", 16, g_wounds, 10, 5, true, g_pointsPerUnit) {
        m_keywords = {CHAOS, DAEMON, HORROR, TZEENTCH, HERO, WIZARD, FATESKIMMER};
        m_weapons = {&m_magicalFlames, &m_staff, &m_dagger, &m_bite};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_bite.setMount(true);
        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setChangeCoven(coven);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_magicalFlames);
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_dagger);
        model->addMeleeWeapon(&m_bite);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<AreaOfEffectSpell>(this, "Tzeentch's Firestorm", 8, 0, 9, RAND_D3, 3));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Wounds Fateskimmer::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Sky-sharks
        if (target->hasKeyword(MONSTER) && (weapon->name() == m_bite.name())) {
            return {Dice::RollD3(), 0};
        }
        return TzeentchBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int Fateskimmer::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    int Fateskimmer::rollCasting(UnmodifiedCastingRoll &unmodifiedRoll) const {
        if (!m_usedArcaneTome) {
            std::vector<int> rolls{ Dice::RollD6(), Dice::RollD6(), Dice::RollD6()};
            std::sort(rolls.begin(), rolls.end());
            unmodifiedRoll.d1 = rolls[1];
            unmodifiedRoll.d2 = rolls[2];
            m_usedArcaneTome = true;
            return unmodifiedRoll + castingModifier();
        }
        return TzeentchBase::rollCasting(unmodifiedRoll);
    }

} // Tzeentch