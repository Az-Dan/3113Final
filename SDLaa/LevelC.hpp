#include "Scene.hpp"

class LevelC : public Scene {
public:
    int ENEMY_COUNT = 1;
    bool winner = 0;
    
    ~LevelC();
    
    void player_init();
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
