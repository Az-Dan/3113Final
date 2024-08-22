#include "Scene.hpp"

class Tutorial : public Scene {
public:
    int ENEMY_COUNT = 0;
    
    ~Tutorial();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
