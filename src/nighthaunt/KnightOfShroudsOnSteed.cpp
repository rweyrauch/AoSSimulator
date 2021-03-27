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
        auto unit = new KnightOfShroudsOnEtherealSteed();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    void KnightOfShroudsOnEtherealSteed::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    KnightOfShroudsOnEtherealSteed::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    KnightOfShroudsOnEtherealSteed::ComputePoints,
                    {
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

    KnightOfShroudsOnEtherealSteed::KnightOfShroudsOnEtherealSteed() :
            Nighthaunt("Knight of Shrouds on Ethereal Steed", 12, g_wounds, 10, 4, true),
            m_sword(Weapon::Type::Melee, "Sword of Stolen Hours", 1, 4, 3, 3, -1, 2),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Ghostly Hooves and Teeth", 1, 2, 4, 5, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, KNIGHT_OF_SHROUDS};
        m_weapons = {&m_sword, &m_hoovesAndTeeth};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_hoovesAndTeeth.setMount(true);
    }

    void KnightOfShroudsOnEtherealSteed::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_sword);
        model->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(model);

        m_points = ComputePoints(1);
    }

    int KnightOfShroudsOnEtherealSteed::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nighthaunt
