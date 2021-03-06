/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "nighthaunt/KnightOfShroudsOnSteed.h"
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int g_basesize = 75; // x42 oval
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 120;

    bool KnightOfShroudsOnEtherealSteed::s_registered = false;

    Unit *KnightOfShroudsOnEtherealSteed::Create(const ParameterList &parameters) {
        auto procession = (Procession) GetEnumParam("Procession", parameters, g_processions[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new KnightOfShroudsOnEtherealSteed(procession, trait, artefact, general);
    }

    void KnightOfShroudsOnEtherealSteed::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    KnightOfShroudsOnEtherealSteed::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    KnightOfShroudsOnEtherealSteed::ComputePoints,
                    {
                            EnumParameter("Procession", g_processions[0], g_processions),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Knight of Shrouds on Ethereal Steed", factoryMethod);
        }
    }

    KnightOfShroudsOnEtherealSteed::KnightOfShroudsOnEtherealSteed(Procession procession, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Nighthaunt(procession, "Knight of Shrouds on Ethereal Steed", 12, g_wounds, 10, 4, true, g_pointsPerUnit) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, KNIGHT_OF_SHROUDS};
        m_weapons = {&m_sword, &m_hoovesAndTeeth};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_hoovesAndTeeth.setMount(true);

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_sword);
        model->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(model);
    }

    int KnightOfShroudsOnEtherealSteed::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void KnightOfShroudsOnEtherealSteed::onEnemyModelSlainWithWeapon(int numSlain, Unit *enemyUnit, const Weapon *weapon,
                                                                     const Wounds &weaponDamage) {
        Nighthaunt::onEnemyModelSlainWithWeapon(numSlain, enemyUnit, weapon, weaponDamage);

        // Stolen Hours
        if (enemyUnit->hasKeyword(HERO) && (weapon->name() == m_sword.name())) {
            heal(1);
        }
    }

} // namespace Nighthaunt
