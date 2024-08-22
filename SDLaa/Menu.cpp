/**
* Author: Dani Kim
* Assignment: AAAAAA (Gravity Platformer)
* Date due: 2024-08-15, 1:00pm (extended to submit by 2024-08-18)
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Menu.hpp"
#include "Utility.hpp"

std::string m_title;
std::string m_text;
glm::mat4 m_title_matrix;
glm::mat4 m_text_matrix;
glm::vec3 m_title_position;
glm::vec3 m_text_position;
GLuint m_font_texture_id;


constexpr char m_FONT_FILEPATH[] = "assets/font1.png";

Menu::~Menu()
{
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeMusic(m_game_state.bgm);
}

void Menu::initialise()
{
    
       m_game_state.map = new Map(0, 0, nullptr, 0, 1.0f, 1, 1);

       m_game_state.player = new Entity();
    
    m_font_texture_id = Utility::load_texture(m_FONT_FILEPATH);

    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
//    m_game_state.bgm = Mix_LoadMUS("assets/menu.mp3");
//    Mix_PlayMusic(m_game_state.bgm, -1);
//    Mix_VolumeMusic(MIX_MAX_VOLUME / 16.0f);
    
    /// ##BGM AINT WORKING DANG IT

}

void Menu::update(float delta_time)
{

}

void Menu::render(ShaderProgram *program)
{
    m_game_state.map->render(program);
    m_title = "AAAAAA";
    m_text = "Press ENTER to Start";
    m_title_position = glm::vec3(-1.8f, 1.0f, 0.0f);
    m_text_position = glm::vec3(-3.6f, 0.0f, 0.0f);
    Utility::draw_text(program, m_font_texture_id, m_title, 0.7f, 0.000001f, m_title_position);
    Utility::draw_text(program, m_font_texture_id, m_text, 0.4f, 0.000001f, m_text_position);
}
