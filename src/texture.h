#pragma once

class Texture {
private:
    unsigned int ID;
    int width, height;
public:
    Texture() : ID(0), width(0), height(0) {};
    Texture(int width, int height, void *data);
    void bind() const;
};
