#include "Scene.hpp"

class LevelC : public Scene {
public:
    bool winner = false;
    float m_timer = 30.0;
    
    ~LevelC();
    
    void player_init();
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
