#include "Scene.hpp"

class Tutorial : public Scene {
public:    
    ~Tutorial();
    
    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram *program) override;
};
