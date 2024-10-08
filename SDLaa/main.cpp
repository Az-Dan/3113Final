/**
* Author: Dani Kim
* Assignment: AAAAAA (Gravity Platformer)
* Date due: 2024-08-15, 1:00pm (extended to submit by 2024-08-18)
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
//#define LEVEL1_WIDTH 14
//#define LEVEL1_HEIGHT 8
#define LEVEL1_LEFT_EDGE 5.0f
#define LEVEL1_RIGHT_EDGE 10.0f

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "Entity.hpp"
#include "Map.hpp"
#include "Utility.hpp"
#include "Scene.hpp"
#include "Menu.hpp"
#include "Tutorial.hpp"
#include "LevelA.hpp"
#include "LevelB.hpp"
#include "LevelC.hpp"
#include "Final.hpp"
#include "Effects.hpp"


// ––––– CONSTANTS ––––– //
constexpr int WINDOW_WIDTH  = 640 * 1.5,
          WINDOW_HEIGHT = 480 * 1.5;

constexpr float BG_RED     = 0.7f,
            BG_BLUE    = 0.79f,
            BG_GREEN   = 0.7f,
            BG_OPACITY = 0.3f;

constexpr int VIEWPORT_X = 0,
          VIEWPORT_Y = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

constexpr char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
           F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

constexpr char BAD_SHADER_PATH[] = "shaders/effects_textured.glsl";


constexpr float MILLISECONDS_IN_SECOND = 1000.0;

enum AppStatus { RUNNING, TERMINATED };

// ––––– GLOBAL VARIABLES ––––– //
Scene  *g_current_scene;
Menu *g_menu;
Tutorial *g_tutorial;
LevelA *g_levelA;
LevelB *g_levelB;
LevelC *g_levelC;
Final *g_final;

Effects *g_effects;
Scene   *g_levels[5];

SDL_Window* g_display_window;
Mix_Chunk *ahh;

ShaderProgram g_shader_program;
ShaderProgram g_shader_program_die;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;

bool g_is_colliding_bottom = false;
bool g_grav_right = true;
bool g_outta_lives = true;
float g_life_count = 999.0f;

AppStatus g_app_status = RUNNING;

void swtich_to_scene(Scene *scene);
void initialise();
void process_input();
void update();
void render();
void shutdown();

// ––––– GENERAL FUNCTIONS ––––– //
void switch_to_scene(Scene *scene)
{
    g_current_scene = scene;
    g_current_scene->initialise(); // DON'T FORGET THIS STEP!
}

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow("AAAAAA",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
    g_shader_program_die.load(V_SHADER_PATH, BAD_SHADER_PATH);

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);
    
    glUseProgram(g_shader_program.get_program_id());

    glClearColor(BG_RED, BG_GREEN, BG_BLUE, BG_OPACITY);
    
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    g_menu = new Menu();
    g_levelA = new LevelA();
    g_levelB = new LevelB();
    g_tutorial = new Tutorial();
    g_levelC = new LevelC();
    g_final = new Final();
    
    g_levels[0] = g_menu;
    g_levels[1] = g_tutorial;
    g_levels[2] = g_levelA;
    g_levels[3] = g_levelB;
    /// ## SCREW IT. 4 LEVELS TOPS. ##
    g_levels[4] = g_levelC;
  //  g_levels[5] = g_final;
    
    // Start at level A
    switch_to_scene(g_levels[0]);
    
    g_effects = new Effects(g_projection_matrix, g_view_matrix);
    g_effects->start(FADEIN, 2.0f);
    
   // g_enter_sfx = Mix_LoadWAV("assets/horridjump.mp3");
}

void process_input()
{
    // VERY IMPORTANT: If nothing is pressed, we don't want to go anywhere
    g_current_scene->get_state().player->set_movement(glm::vec3(0.0f));
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            // End game
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_app_status = TERMINATED;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_BACKSPACE:
                        // Quit the game with a keystroke
                        g_app_status = TERMINATED;
                        break;
                        
                    
                    case SDLK_SPACE:
                        // Jump
                        if ((g_current_scene->get_state().player->get_map_collided_bottom() || g_current_scene->get_state().player->get_map_collided_top()) && g_current_scene->get_state().player->get_activity())
                                                {
                                                    //g_current_scene->get_state().player->jump();
                                                    Mix_PlayChannel(-1,  g_current_scene->get_state().jump_sfx, 0);
                                                    if (g_grav_right) {
                                                        g_grav_right = false;
                                                    } else {
                                                        g_grav_right = true;
                                                    }
                                                    g_current_scene->get_state().player->set_gravity(g_grav_right);
                                                    
                                                }
                                                 break;
                        
                    case SDLK_8: // LOSS CON
                        g_current_scene->get_state().player->set_lives(5.0f); // don't instafail but give it HORRIBLE TIME
                        break;
                        
                    case SDLK_9: // basically godmode
                        g_current_scene->get_state().player->m_lives = 999.0f;
                        break;
                        
                    case SDLK_0: // final level cheat
                      //  g_current_scene->get_state().next_scene_id = 4;
                        break;
                        
                    case SDLK_p: // exit tutorial, enter level 1
                        break;
                        
                    case SDLK_BACKSLASH: // kill cheat
                        g_current_scene->get_state().player->deactivate();
                        break;
                        
                    case SDLK_RETURN:
                       // switch_to_scene(g_levels[0]);
                        if (g_current_scene == g_menu) {
                            ahh = Mix_LoadWAV("assets/horridjump.mp3");
                            Mix_PlayChannel(-1, ahh, 0);
                            switch_to_scene(g_levels[1]);
                            Mix_FreeChunk(ahh);  // Free the allocated memory
                        } else if (g_current_scene == g_tutorial) {
                            switch_to_scene(g_levels[2]);
                        }
                        break;
                    
                    default:
                        break;
                }
                
            default:
                break;
        }
    }
    
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);

//    if (key_state[SDL_SCANCODE_RETURN]) {
//        g_game_state.next_scene_id = 0;
//    }
    
    if (key_state[SDL_SCANCODE_LEFT] || key_state[SDL_SCANCODE_A])        g_current_scene->get_state().player->move_left();
        else if (key_state[SDL_SCANCODE_RIGHT] || key_state[SDL_SCANCODE_D])  g_current_scene->get_state().player->move_right();
         
    if (glm::length( g_current_scene->get_state().player->get_movement()) > 1.0f)
        g_current_scene->get_state().player->normalise_movement();
    
}

void update()
{
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;
    
    delta_time += g_accumulator;
    
    if (delta_time < FIXED_TIMESTEP)
    {
        g_accumulator = delta_time;
        return;
    }
    
    while (delta_time >= FIXED_TIMESTEP) {
        g_current_scene->update(FIXED_TIMESTEP);
        g_effects->update(FIXED_TIMESTEP);
        
        if (g_is_colliding_bottom == false && g_current_scene->get_state().player->get_map_collided_bottom()) g_effects->start(SHAKE, 1.0f);
        
        g_is_colliding_bottom = g_current_scene->get_state().player->get_collided_bottom();
        
        
        
        delta_time -= FIXED_TIMESTEP;
    }
    
    g_accumulator = delta_time;
    
    // Prevent the camera from showing anything outside of the "edge" of the level???
    g_view_matrix = glm::mat4(1.0f);

    g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-g_current_scene->get_state().player->get_position().x, -g_current_scene->get_state().player->get_position().y, 0));
    
    if (g_current_scene == g_levelA && g_current_scene->get_state().player->get_position().y < -10.0f) switch_to_scene(g_levelB);
    
    if (g_current_scene->get_state().player->get_lives() <= 0.0f) {
        std::cout << "Timer reached zero! Changing color." << std::endl;
        //g_shader_program.set_colour(2.0f, 0.5f, 0.0f, 0.0f);
        g_shader_program = g_shader_program_die;
        g_shader_program.set_projection_matrix(g_projection_matrix);
        g_shader_program.set_view_matrix(g_view_matrix);

    }
    
    g_view_matrix = glm::translate(g_view_matrix, g_effects->get_view_offset());
}

void render()
{
    g_shader_program.set_view_matrix(g_view_matrix);
       
    glClear(GL_COLOR_BUFFER_BIT);
       
    glUseProgram(g_shader_program.get_program_id());
    // ————— RENDERING THE SCENE (i.e. map, character, enemies...) ————— //
    g_current_scene->render(&g_shader_program);

    
       
    g_effects->render();

    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();
    
    delete g_menu;
    delete g_tutorial;
    delete g_levelA;
    delete g_levelB;
    delete g_levelC;
    delete g_final;
    delete g_effects;
   // Mix_FreeChunk(ahh);
    Mix_CloseAudio();
}

// ––––– DRIVER GAME LOOP ––––– //
int main(int argc, char* argv[])
{
    initialise();
    
    while (g_app_status == RUNNING)
    {
        process_input();
        update();
        
        if (g_current_scene->get_state().next_scene_id >= 0) switch_to_scene(g_levels[g_current_scene->get_state().next_scene_id]);
        
        render();
    }
    
    shutdown();
    return 0;
}
