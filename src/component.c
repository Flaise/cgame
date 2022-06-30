#ifdef _WIN32a
#include "SDL.h"
#else
#include "SDL2/SDL.h"
#endif

#include "entity.h"
#include "constants.h"

Components make_components() {
    Components result;
    result.compgroups[COMPTYPE_POSITION] = compgroup_init(10, sizeof(CPosition));
    result.compgroups[COMPTYPE_AVATAR] = compgroup_init(10, sizeof(CAvatar));
    result.compgroups[COMPTYPE_SELECTABLE] = compgroup_init(10, sizeof(CSelectable));
    return result;
}

CPosition* position_init(Components* components, Entity entity, int x, int y) {
    CompGroup* group = &components->compgroups[COMPTYPE_POSITION];
    CPosition* result = (CPosition*)component_init(group, entity);
    if (result != NULL) {
        result->x = x;
        result->y = y;
    }
    return result;
}

CAvatar* avatar_init(Components* components, Entity entity, Icon icon) {
    CompGroup* group = &components->compgroups[COMPTYPE_AVATAR];
    CAvatar* result = (CAvatar*)component_init(group, entity);
    if (result != NULL) {
        result->icon = icon;
    }
    return result;
}

CSelectable* selectable_init(Components* components, Entity entity) {
    CompGroup* group = &components->compgroups[COMPTYPE_SELECTABLE];
    CSelectable* result = (CSelectable*)component_init(group, entity);
    return result;
}
