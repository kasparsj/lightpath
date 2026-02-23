#include "Owner.h"

#include "../runtime/RuntimeLight.h"

void Owner::add(RuntimeLight* const light) const {
    light->owner = this;
    light->owner->update(light);
}
