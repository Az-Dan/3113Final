/**
* Author: Dani Kim
* Assignment: AAAAAA (Gravity Platformer)
* Date due: 2024-08-15, 1:00pm (extended to submit by 2024-08-18)
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "LevelA.hpp"
#include "Utility.hpp"
//#include "Effects.hpp"

#define LEVEL_WIDTH 15
#define LEVEL_HEIGHT 12
std::string m_texty;
//glm::mat4 m_text_matrix;
//glm::vec3 m_text_position;
GLuint m_font_texture_id1;
//int m_timer1 = 60;



constexpr char SPRITESHEET_FILEPATH[] = "assets/bis.png",
                ALT_FILEPATH[] = "assets/sib.png",
           ENEMY_FILEPATH[]       = "assets/NO.png";
constexpr char m_FONT_FILEPATH[] = "assets/font1.png";

unsigned int LEVELA_DATA[] =
{
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3,
    3, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 3,
    3, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 3,
    3, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 3,
    3, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 3,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 3,
    3, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 3,
    3, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 3,
    3, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 3
};

LevelA::~LevelA()
{
    delete [] m_game_state.enemies;
    delete    m_game_state.player;
    delete    m_game_state.map;
    Mix_FreeChunk(m_game_state.jump_sfx);
    Mix_FreeChunk(m_game_state.die_sfx);
    Mix_FreeMusic(m_game_state.bgm);
}

void LevelA::player_init() // needed
{
    /*
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

   glm::vec3 acceleration = glm::vec3(0.0f, -8.0f, 0.0f);
   
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
         m_timer
       );

   m_game_state.player->set_position(glm::vec3(2.0f, -4.0f, 0.0f));

   // Jumping
   m_game_state.player->set_jumping_power(3.0f);
}

void LevelA::initialise()
{
    m_game_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("assets/tile195.png");
    m_game_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_DATA, map_texture_id, 1.0f, 1, 1);
    
    // Code from main.cpp's initialise()
    
    player_init();
    
    /**
     
    Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture(ENEMY_FILEPATH);

    m_game_state.enemies = new Entity[ENEMY_COUNT];

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
    m_game_state.enemies[i] =  Entity(enemy_texture_id, 1.0f, 1.0f, 1.0f, ENEMY, GUARD, IDLE);
    }

    m_game_state.enemies[0].set_jumping_power(0.1f);
    m_game_state.enemies[0].set_ai_type(JUMPER);
   // m_game_state.enemies[0].set_ai_state(JUMPING);
    m_game_state.enemies[0].set_position(glm::vec3(11.0f, -4.0f, 0.0f));
    m_game_state.enemies[0].set_width(0.5f);
    m_game_state.enemies[0].set_height(0.5f);
 //   m_game_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[0].set_acceleration(glm::vec3(0.0f, -4.905f, 0.0f));
    
    m_game_state.enemies[1].set_jumping_power(0.1f);
    m_game_state.enemies[1].set_ai_type(JUMPER);
  //  m_game_state.enemies[1].set_ai_state(JUMPING);
    m_game_state.enemies[1].set_position(glm::vec3(1.0f, -11.0f, 0.0f));
    m_game_state.enemies[1].set_width(0.5f);
    m_game_state.enemies[1].set_height(0.5f);
  //  m_game_state.enemies[1].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[1].set_acceleration(glm::vec3(0.0f, -4.905f, 0.0f));
    
    m_game_state.enemies[2].set_jumping_power(0.1f);
    m_game_state.enemies[2].set_ai_type(JUMPER);
 //   m_game_state.enemies[2].set_ai_state(JUMPING);
    m_game_state.enemies[2].set_position(glm::vec3(10.0f, -6.0f, 0.0f));
    m_game_state.enemies[2].set_width(0.5f);
    m_game_state.enemies[2].set_height(0.5f);
  //  m_game_state.enemies[2].set_movement(glm::vec3(0.0f));
    m_game_state.enemies[2].set_acceleration(glm::vec3(0.0f, -4.905f, 0.0f));
    
    m_font_texture_id1 = Utility::load_texture(m_FONT_FILEPATH);

    
    /** 
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_game_state.bgm = Mix_LoadMUS("assets/lvl1.mp3");
    Mix_PlayMusic(m_game_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 16.0f);
    
    m_game_state.jump_sfx = Mix_LoadWAV("assets/whoosh.mp3");
    m_game_state.die_sfx = Mix_LoadWAV("assets/augh.mp3");
    m_game_state.win_sfx = Mix_LoadWAV("assets/YIPPEE.mp3");
    //glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    
   // m_timer = 64.0f;
    
}

void LevelA::update(float delta_time)
{
    std::cout << "Delta time: " << delta_time << std::endl;
    std::cout << "Timer before update: " << m_timer << std::endl;
    

    if (m_timer > 0.0f) {
        if (m_timer < 300.0f) {
            if (winner) {
                m_game_state.player->deactivate();
            }
            m_timer -= delta_time; // Decrease the timer by the elapsed time
            m_game_state.player->m_lives -= delta_time;
        }
        if (m_timer < 0.0f) {
            m_timer = 0.0f; // Ensure timer doesn't go below zero
            // Handle timeout event, if necessary
            m_game_state.player->set_lives(0.0f);
            m_game_state.player->deactivate();
            m_game_state.player->set_movement(glm::vec3(0.0f));
            m_game_state.player->set_acceleration(glm::vec3(0.0f, 0.0f, 0.0f));
            
        }
        if (m_timer != m_game_state.player->get_lives()) {
            m_timer = m_game_state.player->get_lives();
        }

        std::cout << "Timer after update: " << m_timer << std::endl;
        std::cout << "Lives (also timer) after update: " << m_game_state.player->get_lives() << std::endl;


    }
    m_game_state.player->update(delta_time, m_game_state.player, m_game_state.enemies, ENEMY_COUNT, m_game_state.map);
    if (!m_game_state.player->get_gravity()) {
        m_game_state.player->set_tid(Utility::load_texture(ALT_FILEPATH));
    } else {
        m_game_state.player->set_tid(Utility::load_texture(SPRITESHEET_FILEPATH));
    }
    if (m_game_state.player->get_position().y < -12.0f) m_game_state.next_scene_id = 3;


    for (int i = 0; i < ENEMY_COUNT; i++) {
        //    void update(float delta_time, Entity *player, Entity *collidable_entities, int collidable_entity_count, Map *map);
        m_game_state.enemies[i].update(0.0166666f, m_game_state.player, m_game_state.player, 1, m_game_state.map);
    }
    
    if ((!m_game_state.player->get_activity() && !winner) && m_timer > 0.0f) { // player is dead
        Mix_PlayChannel(-1,  get_state().die_sfx, 0);
        player_init();
    }
    
//    const Uint8 *key_state = SDL_GetKeyboardState(NULL);
//
//    if (key_state[SDL_SCANCODE_RETURN]) {
//        Mix_PlayChannel(-1, get_state().win_sfx, 0);
//        winner = true;
//    }
    std::cout << "Enemy Position: " << m_game_state.enemies[0].get_position().x << ", " << m_game_state.enemies[0].get_position().y << std::endl;
    std::cout << "Enemy Active: " << m_game_state.enemies[0].get_activity() << std::endl;

}

void LevelA::render(ShaderProgram *program)
{
    m_game_state.map->render(program);
    m_game_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++) {
         m_game_state.enemies[i].render(program);
     }
    m_texty = "TIME LEFT: " + std::to_string(static_cast<int>(m_timer));
    if (winner) {
        Utility::draw_text(program, m_font_texture_id1, "W", 0.5f, -0.05f,
            glm::vec3((m_game_state.player->get_position().x), (m_game_state.player->get_position().y), 0.0f));
    }
    else if (m_game_state.player->get_lives() > 0.0f) {
        Utility::draw_text(program, m_font_texture_id1, m_texty, 0.4f, 0.000001f, glm::vec3((m_game_state.player->get_position().x - 2.0f), (m_game_state.player->get_position().y - 2.0f), 0.0f));
    }
    else {
        Utility::draw_text(program, m_font_texture_id1, "L", 0.5f, -0.05f,
            glm::vec3((m_game_state.player->get_position().x), (m_game_state.player->get_position().y), 0.0f));
    }
    
}
