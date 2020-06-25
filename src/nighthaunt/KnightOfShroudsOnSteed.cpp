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
    static const int BASESIZE = 75; // x42 oval
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    bool KnightOfShroudsOnEtherealSteed::s_registered = false;

    Unit *KnightOfShroudsOnEtherealSteed::Create(const ParameterList &parameters) {
        auto unit = new KnightOfShroudsOnEtherealSteed();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
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
            Nighthaunt("Knight of Shrouds on Ethereal Steed", 12, WOUNDS, 10, 4, true),
            m_sword(Weapon::Type::Melee, "Sword of Stolen Hours", 1, 4, 3, 3, -1, 2),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Ghostly Hooves and Teeth", 1, 2, 4, 5, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, KNIGHT_OF_SHROUDS};
        m_weapons = {&m_sword, &m_hoovesAndTeeth};
        m_hasMount = true;
    }

    bool KnightOfShroudsOnEtherealSteed::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_sword);
        model->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    int KnightOfShroudsOnEtherealSteed::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Nighthaunt
