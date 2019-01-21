/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <vector>
#include <algorithm>
#include <cairommconfig.h>
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <Board.h>
#include <Roster.h>
#include <sstream>

Board* Board::s_pInstance = nullptr;

void Board::addObjective(Objective* objective)
{
    m_objectives.push_back(objective);
}

void Board::moveObjective(int id, float x, float y)
{
    auto matchId = [id](const Objective* obj)->bool { return (obj->m_id == id); };
    auto obj = std::find_if(m_objectives.begin(), m_objectives.end(), matchId);
    if (obj != m_objectives.end())
    {
        (*obj)->m_x = x;
        (*obj)->m_y = y;
    }
}

void Board::addFeature(TerrainFeature* feature)
{
    m_features.push_back(feature);
}

void Board::addRosters(const Roster *pRedRoster, const Roster *pBlueRoster)
{
    m_rosters[(int)PlayerId::Red] = pRedRoster;
    m_rosters[(int)PlayerId::Blue] = pBlueRoster;
}

Board *Board::Instance()
{
    if (s_pInstance == nullptr)
    {
        s_pInstance = new Board();
    }
    return s_pInstance;
}

void Board::render(const std::string filename) const
{
    // use cairomm to create a raster image of the current board state
    int imageW = m_width * 10; // tenth's of inches
    int imageH = m_depth * 10;

    Cairo::RefPtr<Cairo::ImageSurface> surface =
        Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, imageW, imageH);

    Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);

    Cairo::RefPtr<Cairo::ToyFontFace> font =
        Cairo::ToyFontFace::create("Bitstream Charter", Cairo::FONT_SLANT_ITALIC, Cairo::FONT_WEIGHT_BOLD);

    cr->save(); // save the state of the context
    cr->set_source_rgb(25/255.0, 119/255.0, 48/255.0);
    cr->paint(); // fill image with the color
    cr->restore(); // color is back to black now

    cr->save();
    // draw a border around the image
    cr->set_line_width(2.0); // make the line wider
    cr->rectangle(0.0, 0.0, surface->get_width(), surface->get_height());
    cr->stroke();
    cr->restore();

    // Draw red team units.
    cr->save();
    cr->set_line_width(1.0);
    cr->set_source_rgb(1.0, 0.0, 0.0);

    const Roster* red = m_rosters[0];
    for (auto ip = red->unitBegin(); ip != red->unitEnd(); ++ip)
    {
        const Unit* unit = *ip;
        auto baseSize = unit->basesizeInches();
        auto radiusInches = baseSize*0.5f;

        cr->move_to((unit->position().x()+radiusInches) * 10, (unit->position().y()-radiusInches) * 10);
        cr->set_source_rgb(1, 1, 1);
        cr->set_font_face(font);
        cr->set_font_size(12.0);
        std::stringstream text;
        text << unit->remainingModels();
        cr->show_text(text.str());

        cr->set_source_rgb(1.0, 0.0, 0.0);
        for (auto mip = unit->modelBegin(); mip != unit->modelEnd(); ++mip)
        {
            auto pos = mip->position();
            cr->arc(pos.x() * 10.0, pos.y() * 10.0, radiusInches * 10.0, 0.0, 2.0 * M_PI);
            cr->fill();
        }
    }
    cr->restore();

    // Draw blue team units.
    cr->save();
    cr->set_line_width(1.0);

    const Roster* blue = m_rosters[1];
    for (auto ip = blue->unitBegin(); ip != blue->unitEnd(); ++ip)
    {
        const Unit* unit = *ip;
        auto baseSize = unit->basesizeInches();
        auto radiusInches = baseSize*0.5f;

        cr->move_to((unit->position().x()+radiusInches) * 10, (unit->position().y()-radiusInches) * 10);
        cr->set_source_rgb(1, 1, 1);
        cr->set_font_face(font);
        cr->set_font_size(12.0);
        std::stringstream text;
        text << unit->remainingModels();
        cr->show_text(text.str());

        cr->set_source_rgb(0.0, 0.0, 1.0);
        for (auto mip = unit->modelBegin(); mip != unit->modelEnd(); ++mip)
        {
            auto pos = mip->position();
            cr->arc(pos.x() * 10.0, pos.y() * 10.0, radiusInches * 10.0, 0.0, 2.0 * M_PI);
            cr->fill();
        }
    }
    cr->restore();

    surface->write_to_png(filename);
}
