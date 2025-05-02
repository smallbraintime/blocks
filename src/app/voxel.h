#pragma once

#include "renderer.h"
#include "scene.h"

class Voxel : IObject {
public:
    Voxel() {}

private:
    QSharedPointer<RenderObject> m_renderObject;
};

