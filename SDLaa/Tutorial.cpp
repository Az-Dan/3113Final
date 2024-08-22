/**
* Author: Dani Kim
* Assignment: AAAAAA (Gravity Platformer)
* Date due: 2024-08-15, 1:00pm (extended to submit by 2024-08-18)
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Tutorial.hpp"
#include "Utility.hpp"

#define LEVEL_WIDTH 15
#define LEVEL_HEIGHT 10

GLuint m_font_texture_idt;


constexpr char SPRITESHEET_FILEPATH[] = "assets/bis.png",
            ALT_FILEPATH[] = "assets/sib.png",
           ENEMY_FILEPATH[]       = "assets/soph.png";
constexpr char m_FONT_FILEPATH[] = "assets/font1.png";



unsigned int TUTORIAL_DATA[] =
{
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3
};

Tutorial::~Tutorial()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void Tutorial::initialise()
{
    m_game_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("assets/bluetile.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, TUTORIAL_DATA, map_texture_id, 1.0f, 1, 1);
    
    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
    // Existing
    int player_walking_animation[4][3] =
    {
        { 6, 7, 8 },  // for George to move to the left,
        { 9, 10, 11 }, // for George to move to the right,
        { 0, 1, 2 }, // for George to move upwards,
        { 3, 4, 5 }   // for George to move downwards
    };

    glm::vec3 acceleration = glm::vec3(0.0f, -4.81f, 0.0f);
    
    GLuint player_texture_id = Utility::load_texture(SPRITESHEET_FILEPATH);
    
    m_game_state.player = new Entity(
          player_texture_id,         // texture id
          5.0f,                      // speed
          acceleration,              // acceleration
          3.0f,                      // jumping power
          player_walking_animation,  // animation index sets
          0.0f,                      // animation time
          3,                         // animation frame amount
          0,                         // current animation index
          3,                         // animation column amount
          4,                         // animation row amount
          0.5f,                      // width
          1.0f,                       // height
          PLAYER,
                                     true,
                                     20
        );

        
    m_game_state.player->set_position(glm::vec3(3.0f, -4.0f, 0.0f));


    // Jumping
    m_game_state.player->set_jumping_power(3.0f);
    
    /**
    Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);
    m_font_texture_idt = Utility::load_texture(m_FONT_FILEPATH);


    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
    m_game_state.enemies[i] =  Entity(enemy_texture_id, 1.0f, 1.0f, 1.0f, ENEMY, GUARD, IDLE);
    }


    m_game_state.enemies[0].set_position(glm::vec3(8.0f, 0.0f, 0.0f));
    m_game_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/tutorial.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 16.0f);
    
    m_game_state.jump_sfx = Mix_LoadWAV("assets/whoosh.mp3");
}

void Tutorial::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    if (!m_game_state.player->get_gravity()) {
        m_game_state.player->set_tid(Utility::load_texture(ALT_FILEPATH));
    } else {
        m_game_state.player->set_tid(Utility::load_texture(SPRITESHEET_FILEPATH));
    }
    
}

void Tutorial::render(ShaderProgram *program)
{
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    Utility::draw_text(program, m_font_texture_idt, "Welcome to Tutorial Land!", 0.39f, 0.000001f,  glm::vec3(m_game_state.player->get_position().x-4.5, m_game_state.player->get_position().y+2.0f, 0.0f));
    Utility::draw_text(program, m_font_texture_idt, "Arrow keys or A/D to move,", 0.34f, 0.000001f,  glm::vec3(m_game_state.player->get_position().x-4.5, m_game_state.player->get_position().y, 0.0f));
    Utility::draw_text(program, m_font_texture_idt, "SPACE to switch gravity", 0.34f, 0.000001f, glm::vec3(m_game_state.player->get_position().x-4.5, m_game_state.player->get_position().y-1.0f, 0.0f));
    Utility::draw_text(program, m_font_texture_idt, "ENTER/RETURN to enter Lvl 1!", 0.34f, 0.000001f, glm::vec3(m_game_state.player->get_position().x-4.5, m_game_state.player->get_position().y-2.0f, 0.0f));
    Utility::draw_text(program, m_font_texture_idt, "Watch the clock :)", 0.34f, 0.000001f,  glm::vec3(m_game_state.player->get_position().x-4.0, m_game_state.player->get_position().y-3.0f, 0.0f));

}
