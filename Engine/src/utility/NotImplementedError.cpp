#include "includes/utility/NotImplementedError.hpp"

NotImplementedError::NotImplementedError()
    : std::logic_error("Method not yet implemented.") {}
