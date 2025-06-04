#pragma once

#include "../Generated/Item/ItemObject.h"
#include "../Resolvers/ItemResolver.hpp"
#include <memory>
#include "../../Models/ItemModel.hpp"

std::shared_ptr<graphql::item::object::Item> wrapItemModel(const ItemModel& model);
