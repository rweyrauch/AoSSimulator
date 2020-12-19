/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "mawtribes/Butcher.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 50;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 140;

    bool Butcher::s_registered = false;

    Unit *Butcher::Create(const ParameterList &parameters) {
        auto unit = new Butcher();

        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Cleaver);

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
        unit->setMawtribe(tribe);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_butcherTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_butcherArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_butcherLore[0]);

        bool ok = unit->configure(weapon, lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }


    void Butcher::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Tenderiser, Cleaver};
            static FactoryMethod factoryMethod = {
                    Butcher::Create,
                    Butcher::ValueToString,
                    Butcher::EnumStringToInt,
                    Butcher::ComputePoints,
                    {
                            EnumParameter("Weapon", Cleaver, weapons),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe),
                            EnumParameter("Command Trait", g_butcherTraits[0], g_butcherTraits),
                            EnumParameter("Artefact", g_butcherArtefacts[0], g_butcherArtefacts),
                            EnumParameter("Lore", g_butcherLore[0], g_butcherLore),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Butcher", factoryMethod);
        }
    }

    Butcher::Butcher() :
            MawtribesBase("Butcher", 6, g_wounds, 8, 5, false),
            m_tenderizer(Weapon::Type::Melee, "Tenderizer", 1, 3, 3, 3, -1, 3),
            m_cleaver(Weapon::Type::Melee, "Cleaver", 1, 3, 3, 3, -2, 2),
            m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, HERO, WIZARD, BUTCHER};
        m_weapons = {&m_tenderizer, &m_cleaver, &m_bite};
        m_battleFieldRole = Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool Butcher::configure(WeaponOption weaponOption, Lore lore) {
        auto model = new Model(g_basesize, wounds());

        if (weaponOption == Tenderiser) {
            model->addMeleeWeapon(&m_tenderizer);
        } else if (weaponOption == Cleaver) {
            model->addMeleeWeapon(&m_cleaver);
        }
        model->addMeleeWeapon(&m_bite);

        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        //m_knownSpells.push_back(std::make_unique<VoraciousMaw>(this));

        if (hasKeyword(BLOODGULLET)) {
            m_totalSpells++;
            m_totalUnbinds++;
        }

        m_points = g_pointsPerUnit;

        return true;
    }

    std::string Butcher::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Tenderiser) return "Tenderiser";
            else if (parameter.intValue == Cleaver) return "Cleaver";
        }
        return MawtribesBase::ValueToString(parameter);
    }

    int Butcher::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Tenderiser") return Tenderiser;
        else if (enumString == "Cleaver") return Cleaver;

        return MawtribesBase::EnumStringToInt(enumString);
    }

    void Butcher::onCastSpell(const Spell *spell, const Unit *target) {
        Unit::onCastSpell(spell, target);

        // Bloodgruel
        const auto roll = Dice::RollD6();
        if (roll == 1) applyDamage({0, 1}, this);
        else heal(1);
    }

    void Butcher::onUnboundSpell(Unit *caster, int castRoll) {
        Unit::onUnboundSpell(caster, castRoll);

        // Bloodgruel
        const auto roll = Dice::RollD6();
        if (roll == 1) applyDamage({0, 1}, this);
        else heal(1);
    }

    int Butcher::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OgorMawtribes
