#include "Scene.hpp"
#include "Effects.hpp"

class LevelA : public Scene {
public:
    int ENEMY_COUNT = 3;
    float m_timer = 90.0;
    bool winner = false;
    //Effects *m_effects;
    
    ~LevelA();
    
    void player_init();
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
