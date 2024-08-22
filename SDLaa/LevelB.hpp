#include "Scene.hpp"

class LevelB : public Scene {
public:
    int ENEMY_COUNT = 2;
    float m_timer = 60.0;
    //float m_timer;
    
    ~LevelB();
    
    void player_init();
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
