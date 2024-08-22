/**
* Author: Dani Kim
* Assignment: AAAAAA (Gravity Platformer)
* Date due: 2024-08-15, 1:00pm (extended to submit by 2024-08-18)
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Final.hpp"
#include "Utility.hpp"

#define LEVEL_WIDTH 14
#define LEVEL_HEIGHT 8

GLuint m_font_texture_id0;


constexpr char SPRITESHEET_FILEPATH[] = "assets/bis.png",
           ENEMY_FILEPATH[]          = "assets/soph.png",
           CUSTODY_FILEPATH[]     = "assets/custody.png";
constexpr char m_FONT_FILEPATH[] = "assets/font1.png";



unsigned int FINAL_DATA[] =
{
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

Final::~Final()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void Final::initialise()
{
    m_game_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("assets/tile161b.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, FINAL_DATA, map_texture_id, 1.0f, 1, 1);
    
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

        
    m_game_state.player->set_position(glm::vec3(5.0f, 0.0f, 0.0f));

    // Jumping
    m_game_state.player->set_jumping_power(3.0f);
    
    /**
    Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(CUSTODY_FILEPATH);

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
    
    m_game_state.bgm = Mix_LoadMUS("assets/hum.mp3");
    m_game_state.win_sfx = Mix_LoadWAV("assets/YIPPEE.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(0.0f);
    
  //  m_game_state.jump_sfx = Mix_LoadWAV("assets/whoosh.mp3");
}

void Final::update(float delta_time)
{
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    
    if (m_game_state.player->get_position().y < -10.0f) m_game_state.next_scene_id = 1;
}

void Final::render(ShaderProgram *program)
{
    m_game_state.map->render(program);
    m_game_state.player->render(program);
//    Utility::draw_text(program, m_font_texture_id0, "Welcome to Tutorial Land!", 0.4f, 0.000001f, glm::vec3((m_game_state.player->get_position().x - 2.0f), -2.0f, 0.0f));
//    Utility::draw_text(program, m_font_texture_id0, "Arrow keys or A/D to move, SPACE to switch gravity", 0.4f, 0.000001f, glm::vec3((m_game_state.player->get_position().x - 2.0f), -2.0f, 0.0f));
//    Utility::draw_text(program, m_font_texture_id0, "Press P when you're ready to game", 0.4f, 0.000001f, glm::vec3((m_game_state.player->get_position().x - 2.0f), -2.0f, 0.0f));
}
